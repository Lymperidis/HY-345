#include <stdio.h>
#include "d_params.h"

#define set_task_params(a,b) syscall(342, a, b)
#define get_task_params(a) syscall(343, a)

int main(void){
	/*Wrong group_name */
	    struct d_params params;
	    char group_name = 'A';
	    int member_id = -1;
	            
	    /*call the functions */
	    printf("Now setting the parameters in set_function : id_member : %d , group_name : %c\n",member_id,group_name);
	    set_task_params(group_name,member_id);
	    get_task_params(&params);
	    printf("Additional params are the : id_member %d , group_name : %c\n",params.member_id,params.group_name);
	    /*an error will occure */
	    
	    return 0;
	        
}
	        
