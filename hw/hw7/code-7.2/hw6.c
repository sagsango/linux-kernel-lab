#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/hashtable.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/stacktrace.h>
#include <linux/jhash.h>
#include <linux/percpu.h>

#define __HASH_BITS 10
#define MAX_STACK_DEPTH 32

struct task_count {
    u32 hash_key;                    /* Hash of the stack trace (32-bit from jhash2) */
    unsigned long *stack_entries;    /* Store stack trace entries */
    unsigned int nr_entries;         /* Number of stack entries */
    u64 total_time;                  /* Cumulative time spent on CPU (rdtsc ticks) */
    struct hlist_node node;
};

/* Per-CPU data to track the last scheduled task and its start time */
struct cpu_task_info {
    u32 last_hash_key;               /* Hash of the last task’s stack trace */
    u64 last_start_time;             /* rdtsc timestamp when it started */
};

static DEFINE_HASHTABLE(task_hash_table, __HASH_BITS);
static DEFINE_SPINLOCK(hash_lock);
static DEFINE_PER_CPU(struct cpu_task_info, hw6_cpu_info);  /* Renamed to avoid conflict */
static struct proc_dir_entry *proc_entry;

static struct kprobe kp = {
    .symbol_name = "pick_next_task_fair",
};

/* Inline function to read the TSC (rdtsc) */
static inline u64 read_tsc(void)
{
    unsigned int low, high;
    asm volatile("rdtsc" : "=a" (low), "=d" (high));
    return ((u64)high << 32) | low;
}

static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    struct task_count *entry;
    unsigned long stack_entries[MAX_STACK_DEPTH];
    unsigned int nr_entries;
    u32 hash_key;
    bool found = false;
    unsigned long irqs;
    struct task_struct *prev_task = (struct task_struct *)regs->si;  /* Task being scheduled out */
    u64 now = read_tsc();
    struct cpu_task_info *info = this_cpu_ptr(&hw6_cpu_info);

    if (!prev_task)
        return 0;

    /* Capture stack trace for prev task */
    nr_entries = stack_trace_save(stack_entries, MAX_STACK_DEPTH, 1);
    if (nr_entries == 0)
        return 0;

    hash_key = jhash2((u32 *)stack_entries, nr_entries * sizeof(unsigned long) / sizeof(u32), 0xdeadbeef);

    spin_lock_irqsave(&hash_lock, irqs);

    /* Update time for the task being scheduled out (if it was the last one on this CPU) */
    if (info->last_hash_key != 0 && info->last_hash_key == hash_key) {
        hash_for_each_possible(task_hash_table, entry, node, hash_key) {
            if (entry->hash_key == hash_key && entry->nr_entries == nr_entries &&
                !memcmp(entry->stack_entries, stack_entries, nr_entries * sizeof(unsigned long))) {
                entry->total_time += now - info->last_start_time;
                found = true;
                break;
            }
        }
    }

    /* If not found, initialize it (first time this task is scheduled out) */
    if (!found) {
        entry = kmalloc(sizeof(struct task_count), GFP_ATOMIC);
        if (!entry) {
            spin_unlock_irqrestore(&hash_lock, irqs);
            return 0;
        }
        entry->stack_entries = kmalloc_array(nr_entries, sizeof(unsigned long), GFP_ATOMIC);
        if (!entry->stack_entries) {
            kfree(entry);
            spin_unlock_irqrestore(&hash_lock, irqs);
            return 0;
        }
        entry->hash_key = hash_key;
        entry->nr_entries = nr_entries;
        memcpy(entry->stack_entries, stack_entries, nr_entries * sizeof(unsigned long));
        entry->total_time = 0;  /* No time yet, as it’s just being scheduled out */
        hash_add(task_hash_table, &entry->node, hash_key);
    }

    spin_unlock_irqrestore(&hash_lock, irqs);
    return 0;
}

static void __kprobes handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
    struct task_struct *next_task = (struct task_struct *)regs->ax;  /* Task being scheduled in */
    unsigned long stack_entries[MAX_STACK_DEPTH];
    unsigned int nr_entries;
    u32 hash_key;
    u64 now = read_tsc();
    struct cpu_task_info *info = this_cpu_ptr(&hw6_cpu_info);

    if (!next_task)
        return;

    /* Capture stack trace for next task */
    nr_entries = stack_trace_save(stack_entries, MAX_STACK_DEPTH, 1);
    if (nr_entries == 0)
        return;

    hash_key = jhash2((u32 *)stack_entries, nr_entries * sizeof(unsigned long) / sizeof(u32), 0xdeadbeef);

    /* Record the start time and hash key for the next task on this CPU */
    info->last_hash_key = hash_key;
    info->last_start_time = now;
}

static int perftop_show(struct seq_file *m, void *v)
{
    struct task_count *entry;
    int bkt;
    unsigned long irqs;
    int i;

    seq_printf(m, "Stack Trace Hash\tTime Spent (rdtsc ticks)\tStack Trace\n");

    spin_lock_irqsave(&hash_lock, irqs);
    hash_for_each(task_hash_table, bkt, entry, node) {
        seq_printf(m, "%u\t\t%llu\t\t\t", entry->hash_key, entry->total_time);
        for (i = 0; i < entry->nr_entries; i++) {
            seq_printf(m, "[%d] %pS ", i, (void *)entry->stack_entries[i]);
        }
        seq_printf(m, "\n");
    }
    spin_unlock_irqrestore(&hash_lock, irqs);

    return 0;
}

static int perftop_open(struct inode *inode, struct file *file)
{
    return single_open(file, perftop_show, NULL);
}

static const struct proc_ops perftop_fops = {
    .proc_open = perftop_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init hw6_init(void)
{
    int ret;

    hash_init(task_hash_table);

    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;
    ret = register_kprobe(&kp);
    if (ret < 0) {
        printk(KERN_ERR "hw6: Failed to register kprobe: %d\n", ret);
        return ret;
    }

    proc_entry = proc_create("perftop", 0, NULL, &perftop_fops);
    if (!proc_entry) {
        unregister_kprobe(&kp);
        printk(KERN_ERR "hw6: Failed to create proc entry\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "hw6: Module loaded, kprobe at %p\n", kp.addr);
    return 0;
}

static void __exit hw6_exit(void)
{
    struct task_count *entry;
    struct hlist_node *tmp;
    int bkt;
    unsigned long irqs;

    if (proc_entry)
        proc_remove(proc_entry);

    unregister_kprobe(&kp);

    spin_lock_irqsave(&hash_lock, irqs);
    hash_for_each_safe(task_hash_table, bkt, tmp, entry, node) {
        hash_del(&entry->node);
        kfree(entry->stack_entries);
        kfree(entry);
    }
    spin_unlock_irqrestore(&hash_lock, irqs);

    printk(KERN_INFO "hw6: Module unloaded\n");
}

module_init(hw6_init);
module_exit(hw6_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sagar Singh");
MODULE_DESCRIPTION("Track task scheduling time with stack traces using Kprobes");
