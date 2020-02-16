#include <linux/init.h>
#include <linux/module.h>
 
static int __init hello_init(void) {
    printk(KERN_INFO  "module init success\n");
    return 0;
}
 
static void __exit hello_exit(void) {
    printk(KERN_INFO  "module exit success\n");
}
  
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wuya");
MODULE_DESCRIPTION("driver example");