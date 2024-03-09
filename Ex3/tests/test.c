#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#define __NR_dummy_sys 341
int main(void){
	printf("Trap to kernel level\n");
	syscall(__NR_dummy_sys,42); /*Check*/
	printf("Back to user level\n");
	return 0;
}
