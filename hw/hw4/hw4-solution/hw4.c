#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

static char *int_str;

/* [X1: point 1]
 * Defines module metadata:- license, author, and description.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sagar Singh");
MODULE_DESCRIPTION("LKP Homework 4");

/* [X2: point 1]
 * Declares a module parameter int_str that can be set at load time.
 * The parameter is a string (char pointer) and is readable by users.
 */
module_param(int_str, charp, S_IRUSR | S_IRGRP | S_IROTH);

/* [X3: point 1]
 * Provides a description for the module parameter int_str, 
 * indicating that it should contain a comma-separated list of integers.
 */
MODULE_PARM_DESC(int_str, "A comma-separated list of integers");

/* [X4: point 1]
 * Defines a linked list head named mylist.
 */
static LIST_HEAD(mylist);

/* [X5: point 1]
 * Defines a struct entry that represents each node in the linked list.
 * Each entry contains an integer val and a list field for linking.
 */
struct entry {
	int val;
	struct list_head list;
};

static int store_value(int val)
{
	/* [X6: point 5]
	 * Allocates memory for a new entry and stores the given integer value.
	 * Adds the new entry to the tail of mylist.
	 * Returns 0 on success; otherwise, returns an error code for no mem availabe (e.g., -ENOMEM).
	 */
	struct entry *entry_ptr = kmalloc(sizeof(struct entry), GFP_KERNEL);
	if (!entry_ptr)
		return -ENOMEM;
	entry_ptr->val = val;
	INIT_LIST_HEAD(&entry_ptr->list);
	list_add(&entry_ptr->list, &mylist);
	return 0;
}

static void test_linked_list(void)
{
	/* [X7: point 5]
	 * Iterates through mylist and prints the value of each entry.
	 */
	struct entry *entry_ptr = NULL;
	list_for_each_entry(entry_ptr, &mylist, list)
	{
		printk("linked list data: %d\n", entry_ptr->val);
	}
}

static void destroy_linked_list_and_free(void)
{
	/* [X8: point 5]
	 * Frees all dynamically allocated entries in mylist to prevent memory leaks.
	 * First we remove entry from the list and then free the entry's memory.
	 */
	struct entry *entry_ptr, *tmp;
	list_for_each_entry_safe(entry_ptr, tmp, &mylist, list)
	{
		list_del(&entry_ptr->list); 
		kfree(entry_ptr);
	}
}

static int parse_params(void)
{
	int val, err = 0;
	char *p, *orig, *params;

	/* [X9: point 1]
	 * Duplicates the int_str parameter using kstrdup() to avoid modifying the original string.
	 * If memory allocation fails, returns -ENOMEM (out of memory error).
	 */
	params = kstrdup(int_str, GFP_KERNEL);
	if (!params)
		return -ENOMEM;
	orig = params;

	/* [X10: point 1]
	 * Splits the params string into tokens using strsep(), separating by commas.
	 * Iterates through each extracted token.
	 */
	while ((p = strsep(&params, ",")) != NULL) {
		if (!*p)
			continue;
		
		/* [X11: point 1]
		 * Converts the extracted string token into an integer using kstrtoint().
		 * If the conversion fails, exits the loop.
		 * kstrtoint - convert a string to an int
		 *      int kstrtoint(const char * s, unsigned int base, int * res);
		 */
		err = kstrtoint(p, 0, &val);
		if (err)
			break;

		/* [X12: point 1]
		 * Stores the converted integer value into the linked list.
		 * If storing fails, exits the loop.
		 */
		err = store_value(val);
		if (err)
			break;
	}

	/* [X13: point 1]
	 * Frees the duplicated parameter string to avoid memory leaks.
	 */
	kfree(orig);
	return err;
}

static void run_tests(void)
{
	/* [X14: point 1]
	 * Calls test_linked_list() to display the stored values in the linked list.
	 */
	test_linked_list();
}

static void cleanup(void)
{
	/* [X15: point 1]
	 * Logs a cleanup message and calls destroy_linked_list_and_free()
	 * to free all dynamically allocated memory for the linked list.
	 */
	printk(KERN_INFO "\nCleaning up...\n");
	destroy_linked_list_and_free();
}

static int __init hw4_init(void)
{
	int err = 0;

	/* [X16: point 1]
	 * Checks if the int_str parameter is missing.
	 * If it is not provided, prints an error message and exits.
	 */
	if (!int_str) {
		printk(KERN_INFO "Missing 'int_str' parameter, exiting\n");
		return -1;
	}

	/* [X17: point 1]
	 * Parses the int_str parameter to extract and store integer values.
	 * If parsing fails, exits early.
	 */
	err = parse_params();
	if (err)
		goto out;

	/* [X18: point 1]
	 * Runs tests to verify the linked list contents.
	 */
	run_tests();

out:
	/* [X19: point 1]
	 * Cleans up allocated resources before exiting.
	 */
	cleanup();
	return err;
}

static void __exit hw4_exit(void)
{
	/* [X20: point 1]
	 * Kernel module exit function. Currently, it does nothing.
	 */
	return;
}

/* [X21: point 1]
 * Registers hw4_init as the module's initialization function.
 */
module_init(hw4_init);

/* [X22: point 1]
 * Registers hw4_exit as the module's cleanup function.
 */
module_exit(hw4_exit);
