#include <linux/kernel.h>
#include <linux/d_params.h>
#include <asm-generic/errno-base.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>

asmlinkage long sys_set_task_params(char group_name,int member_id){
    printk("\nCalling set_parms\n");
    printk("\nLymperidis Lymperis csd4813\n");
 	/*Group_name check*/   
    if(group_name < 'A' || group_name > 'Z'){
        printk("\nInvalid group_name,must be English and Capital letter\n");
        return EINVAL;
    }
	/*member_id check*/
    if(member_id < 0 ){
        printk("\nInvalid member_id,must be a positive integer\n");
        return EINVAL;
    }

    /*set the current pointer with current values*/
    current->group_name = group_name;
    current->member_id = member_id;

    return 0;
}
