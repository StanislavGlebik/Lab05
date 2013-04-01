#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/moduleparam.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>

#define SUCCESS 0

MODULE_AUTHOR("Glebik Stas!!!");

struct proc_dir_entry *result_proc;
struct proc_dir_entry *first_op_proc;
struct proc_dir_entry *second_op_proc;
struct proc_dir_entry *oper_proc;


static char * result = "result";
module_param(result, charp, 0);

static char * first_operand = "first operand";
module_param(first_operand, charp, 0);

static char * second_operand = "second_operand";
module_param(second_operand, charp, 0);

static char * operation = "operation";
module_param(operation, charp, 0);

static int
 read_proc(struct seq_file *m, void *v) {
 	seq_printf(m, "%s\n", "Hello");
	return 0;
}

static int
jif_open(struct inode *inode, struct file *file) 
{
	return single_open(file, read_proc, NULL);
}

static const struct file_operations proc_file_fops = {
	.owner = THIS_MODULE,
	.open = jif_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init test_init( void )
{
	printk( KERN_ALERT "ker module loaded\n");
	result_proc = proc_create(result, 0, NULL, &proc_file_fops);
	first_op_proc = proc_create(first_operand, 0, NULL, &proc_file_fops);
	second_op_proc = proc_create(second_operand, 0, NULL, &proc_file_fops);
	oper_proc = proc_create(operation, 0, NULL, &proc_file_fops);

	if (result_proc == NULL || first_op_proc == NULL
		|| second_op_proc == NULL || oper_proc == NULL) {
		printk(KERN_INFO "Error: Could not initialize /proc/ker\n");
		return -ENOMEM;
	} else {
		printk(KERN_INFO "Success!\n");
	}

	return 0;
}

static void __exit test_exit( void )
{
	remove_proc_entry(result, NULL);
	remove_proc_entry(first_operand, NULL);	
	remove_proc_entry(second_operand, NULL);		
	remove_proc_entry(operation, NULL);		

	printk(KERN_ALERT "ker module is unloaded\n");
}

module_init( test_init );
module_exit( test_exit );
