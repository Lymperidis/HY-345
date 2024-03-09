csd4813 Lymperidis Lymperis	

1)Define two new system calls in the unistd_32.h 
2)Increase the number of system calls by 2 in unistd_32.h
3)The kernel needs to have a function pointer pointing to the two new system calls
in the syscall_table_32.S
4)Define a function signature in the syscalls.h
5)Go to the struct_task in the sched.h and put two more arguments 
6)Create a header params.h that have a struct with the two added arguments
7)Implement the two system calls in the kernel (set_task_params.c,get_task_params.c)
8)Add the two obj Makefile
9)Make tests and run them in the qemu (first make the bzImage)


