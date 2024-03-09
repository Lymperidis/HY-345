#include <stdio.h>
#include "d_params.h"

    #define set_task_params(a,b) syscall(342, a, b)
        #define get_task_params(a) syscall(343, a)
        
        
        int main(void){
        
                   struct d_params *params = NULL;
                   char group_name = 'Z';
                   int member_id = 25;
                   printf("Setting the params of struct_task : group_name=%c, member_id=%d\n",group_name,member_id);
                   set_task_params(group_name , member_id);
                   get_task_params(params);
             
                                                          
                   return 0;
                   
}                                       
                                                             
