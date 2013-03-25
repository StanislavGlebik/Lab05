#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("Glebik Stas!!!");

static int __init test_init( void )
{
	printk( KERN_ALERT "ker module loaded\n");

	return 0;
}

static void __exit test_exit( void )
{
	printk(KERN_ALERT "ker module is unloaded\n");
}

module_init( test_init );
module_exit( test_exit );
