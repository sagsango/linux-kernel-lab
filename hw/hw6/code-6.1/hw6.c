#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define PROC_NAME "perftop"

static int perftop_show(struct seq_file *m, void *v) {
    seq_printf(m, "Hello World\n");
    return 0;
}

static int perftop_open(struct inode *inode, struct file *file) {
    return single_open(file, perftop_show, NULL);
}

static const struct proc_ops perftop_fops = {
    .proc_open = perftop_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init perftop_init(void) {
    proc_create(PROC_NAME, 0, NULL, &perftop_fops);
    pr_info("perftop module loaded. Access via /proc/perftop\n");
    return 0;
}

static void __exit perftop_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("perftop module unloaded.\n");
}

module_init(perftop_init);
module_exit(perftop_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sagar Singh");
MODULE_DESCRIPTION("A simple /proc file example for perftop");

