//#define pr_fmt(fmt) "%s: " fmt, KBUILD_MODNAME

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kprobes.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

/*
 * TODO: Experiment with "kretprobe"
 */


/*
 * Counter for number of times proc file is opened
 */
static unsigned long open_count = 0;

/*
 * Proc file entry
 */
static struct proc_dir_entry *perftop_entry;

/* 
 * Kprobe structure
 */
static struct kprobe kp = {
    .symbol_name = "proc_reg_open",
};

/* 
 * Kprobe pre_handler: called when proc_reg_open is executed
 */
static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    open_count++;
    pr_info("[kprobes::handler_pre]proc_reg_open called, counter incremented to %lu\n", open_count);
    return 0;
}

/*
 * Kprobe post_handler: called when proc_reg_open is executed
 */
static void  __kprobes handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flag)
{
    pr_info("[kprobes::handler_post]proc_reg_open call done\n");
    return;
}

/*
 * Proc file read operation
 */
static int perftop_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Hello World\nOpen count: %lu\n", open_count);
    return 0;
}

static int perftop_open(struct inode *inode, struct file *file)
{
    return single_open(file, perftop_show, NULL);
}

static const struct proc_ops perftop_fops = {
    .proc_open    = perftop_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

/*
 * Initialize kprobe
 */
static int __init kprobe_setup(void)
{
    int ret;
    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;
    
    ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    pr_info("Planted kprobe at %p\n", kp.addr);
    return 0;
}

/*
 * Module initialization
 */
static int __init perftop_init(void)
{
    int ret;

    /*
     * Create proc entry
     */
    perftop_entry = proc_create("perftop", 0, NULL, &perftop_fops);
    if (!perftop_entry) {
        pr_err("Failed to create proc file\n");
        return -ENOMEM;
    }

    /*
     * Setup kprobe
     */
    ret = kprobe_setup();
    if (ret < 0) {
        proc_remove(perftop_entry);
        return ret;
    }

    pr_info("perftop module loaded\n");
    return 0;
}

/* 
 * Module cleanup
 */
static void __exit perftop_exit(void)
{
    /*
     * Remove kprobe
     */
    unregister_kprobe(&kp);
    pr_info("kprobe at %p unregistered\n", kp.addr);

    /*
     * Remove proc entry
     */
    proc_remove(perftop_entry);
    pr_info("perftop module unloaded\n");
}

module_init(perftop_init);
module_exit(perftop_exit);

MODULE_DESCRIPTION("Kernel module to count /proc/perftop opens using kprobes");
MODULE_AUTHOR("Sagar Singh");
MODULE_LICENSE("GPL");
