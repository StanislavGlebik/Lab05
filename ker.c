#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>

#define SUCCESS 0

MODULE_AUTHOR("Glebik Stas!!!");

struct proc_dir_entry *My_Proc_File;

static int
 read_proc(struct seq_file *m, void *v) {
 	seq_printf(m, "proc func");
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
	My_Proc_File = proc_create("ker", 0, NULL, &proc_file_fops);

	if (My_Proc_File == NULL) {
		printk(KERN_INFO "Error: Could not initialize /proc/ker\n");
		return -ENOMEM;
	} else {
		printk(KERN_INFO "SUccess!\n");
	}

	return 0;
}

static void __exit test_exit( void )
{
	remove_proc_entry("ker", NULL);
	printk(KERN_ALERT "ker module is unloaded\n");
}

module_init( test_init );
module_exit( test_exit );
