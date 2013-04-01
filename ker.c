#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/moduleparam.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFER_MAX_SIZE 100
#define SUCCESS 0
MODULE_AUTHOR("Glebik Stas!!!");

struct proc_dir_entry *result_proc;
struct proc_dir_entry *first_op_proc;
struct proc_dir_entry *second_op_proc;
struct proc_dir_entry *oper_proc;

char buff[BUFFER_MAX_SIZE];
static int is_device_open = 0;

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
 	seq_printf(m, "%s\n", buff);
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

static ssize_t
procfs_read(struct file *filp,
			char *buffer,
			size_t length,
			loff_t *offset)
{
	static int finished = 0;

	printk(KERN_INFO "Read");


	if (finished) {
		printk(KERN_INFO "finished");
		finished = 0;
		return 0;
	}

	finished = 1;
	if (copy_to_user(buffer, "Hi", 2)) {
		return -EFAULT;
	}
	printk(KERN_INFO "suc");

	return 2;
}

int procfile_write(struct file *file, const char * buffer, size_t count,
					loff_t *data)
{

	printk(KERN_INFO "write");
	if (count > BUFFER_MAX_SIZE)
	{
		count = BUFFER_MAX_SIZE;
	}
	else 
	{
		if (copy_from_user(buff, buffer, count)) {
			return -EFAULT;
		}
	}

	return count;
}

static ssize_t
procfs_open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int procfs_release(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return SUCCESS;
}

static const struct file_operations write_fops = {
	.owner = THIS_MODULE,
	.open = procfs_open,
	.release = procfs_release,
	.write = procfile_write,
	.read = procfs_read,
};

static int __init test_init( void )
{
	printk( KERN_ALERT "ker module loaded\n");
	result_proc = proc_create(result, 0, NULL, &proc_file_fops);
	first_op_proc = proc_create(first_operand, 0, NULL, &proc_file_fops);
	second_op_proc = proc_create(second_operand, 0, NULL, &proc_file_fops);
	oper_proc = proc_create(operation, 0666, NULL, &write_fops);

	oper_proc->uid = 0;
	oper_proc->gid = 0;

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
