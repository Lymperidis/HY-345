#include <linux/kernel.h>
#include <linux/d_params.h>
#include <asm-generic/errno-base.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>

asmlinkage long sys_get_task_params(struct d_params* params){
    printk("\nGet_task_params is being called\n");
	printk("\nLymperidis Lymperis csd4813\n");
    if(params == NULL){
        printk("\nParameters is NULL\n");
        return EINVAL;
    }

    if(!access_ok(VERIFY_WRITE, params, sizeof(struct task_params*))) {
                printk("\nPointer space is not valid!\n");
                return EINVAL;
    }

    params->group_name = get_current()->group_name;
    params->member_id = get_current()->member_id;

    copy_to_user(params, current, sizeof(struct d_params*));
    
    return 0;
}
