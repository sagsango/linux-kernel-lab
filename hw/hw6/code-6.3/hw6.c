#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/hashtable.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/spinlock.h>

#define PID_HASH_BITS 10 

struct pid_count {
	pid_t pid;
	unsigned long count;
	struct hlist_node node;
};

static DEFINE_HASHTABLE(pid_hash_table, PID_HASH_BITS);
static DEFINE_SPINLOCK(hash_lock);
static struct proc_dir_entry *proc_entry;

static struct kprobe kp = {
	.symbol_name = "pick_next_task_fair",
};

static void __kprobes handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
	struct task_struct *next_task;
	pid_t pid;
	struct pid_count *entry;
	bool found = false;
	unsigned long irqs;

/*TODO:
 *  Picking the tasks from registers giving me panic.
 *  in pre_handler regs->di
 *  in post_handler regs->rax
 *
 */
	next_task = current;  

	printk(KERN_INFO "hw6: next_task = %px\n", next_task);

	if (!next_task) {
		printk(KERN_INFO "hw6: next_task is NULL\n");
		return;
	}

	pid = next_task->pid;
	printk(KERN_INFO "hw6: PID = %d\n", pid);

	spin_lock_irqsave(&hash_lock, irqs);
	hash_for_each_possible(pid_hash_table, entry, node, pid) {
		if (entry->pid == pid) {
			entry->count++;
			found = true;
			break;
		}
	}

	if (!found) {
		entry = kmalloc(sizeof(struct pid_count), GFP_ATOMIC);
		if (entry) {
			entry->pid = pid;
			entry->count = 1;
			hash_add(pid_hash_table, &entry->node, pid);
			printk(KERN_INFO "hw6: Added PID %d to hash table\n", pid);
		} else {
			printk(KERN_ERR "hw6: Memory allocation failed for PID %d\n", pid);
		}
	}
	spin_unlock_irqrestore(&hash_lock, irqs);
}

static int perftop_show(struct seq_file *m, void *v)
{
	struct pid_count *entry;
	int bkt;
	unsigned long irqs;

	seq_printf(m, "PID\t\tScheduled Count\n");
	seq_printf(m, "------------------------\n");

	spin_lock_irqsave(&hash_lock, irqs);
	hash_for_each(pid_hash_table, bkt, entry, node) {
		seq_printf(m, "%d\t\t%lu\n", entry->pid, entry->count);
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

	hash_init(pid_hash_table);

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
	struct pid_count *entry;
	struct hlist_node *tmp;
	int bkt;
	unsigned long irqs;

	if (proc_entry)
		proc_remove(proc_entry);

	unregister_kprobe(&kp);

	spin_lock_irqsave(&hash_lock, irqs);
	hash_for_each_safe(pid_hash_table, bkt, tmp, entry, node) {
		hash_del(&entry->node);
		kfree(entry);
	}
	spin_unlock_irqrestore(&hash_lock, irqs);

	printk(KERN_INFO "hw6: Module unloaded\n");
}

module_init(hw6_init);
module_exit(hw6_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sagar Singh");
MODULE_DESCRIPTION("Track PID scheduling counts with Kprobes");
