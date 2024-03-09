csd4813 Lymperidis Lymperis 

Ex4: Scheduler of Kernel 

General Solution : Im going to try to split the persentage of CPU running time of each task of each group.
In the previous excercise we added 2 more variables in the tast_struct which will split the tasks to group_names.
Each group will have their own tasks.I have to split the 100% of the CPU running time devided by each group_name and then devide 
each group with the number of their tasks. So, i will have to find how many groups there are and then count how many tasks there are 
in all of the groups, and put them in a data structure (linked list).Then i have to calculate each task the priority so the scheduler know
which task has to run next .  


Steps for implementation :

1)First of all count how many processes there are in each group .


2)Calculate the number of all the groups. 


3)To take all of the tasks and groups that is necessary for this excercise i have to implement an if that will take all task and groups that will have a 
group_name and the member_id is greater than 0. 


4)For each process in a group, update the sliced time(of the running time of the CPU) of each process.


5)Find the priority of each task 


6)Then find the task with the highest priority and pass the task to the target. So the CPU can run this process 


7)Find the next node in the red black tree 


8)Check if the target is NULL . If the scheduler has nothing to run there is no more task to run .



CODE :


/*CSD4813 LYMPERIDIS LYMPERIS EX4*/

float priority(struct task_struct *task) { 
    // Lower value loweer priority,hight value higher priority 
    float group_cpu_share = get_group_cpu_share(task->group_name); // Get the CPU sliced time for this group
    float task_priority = static_cast<float>(task->prio); //Get the priorith for the task
    float time_slice_used_ratio = (task->time_slice_used / task->time_slice_total);//Get the percentage 

    // Example formula - this can be adjusted based on requirements
    return (group_cpu_share * task_priority) / (1 + time_slice_used_ratio);
}

void update_time_in_each_process(struct group_info *group, float time_slice) {
    // Go throught each process and update the time to run 
    struct task_struct *p;
    for each process p in group {
        set_time(p, time_slice); // Set the time for the process
    }
}


// This code is written into the function pick_next_task()  in the file sched.c

// First get the first node of the rb tree.
struct rb_node* current = rb_first(&rq->cfs.tasks_timeline);
// Then we get the task with highest priority.
struct sched_entity* run_node;
struct task_struct* task;

float highest_priority_value = -1;
struct task_struct* target_s = NULL;

do{
	run_node = container_of(current, struct sched_entity, run_node);
	task = task_of(run_node);

	if ( task->group_name != '' && task->member_id > 0 ) {

        // Go throught the rb tree and find how many groups there are 
        int total_groups = count_groups(); 


        // Go throught all the rb tree and see how many processes there are in each group 
        int num_processes = count_processes(task->group_name); 

        //this is the time for each process 
        float time_per_process = 100.0 / total_groups / num_processes; 

        // Update the time for each process in this group
        update_time_in_each_process(group, time_per_process);

        float priority = priority(task);
        // Select the task with the highest calculated priority
        if (priority > highest_priority) {
            highest_priority = priority;
            target_struct = task;
        }

  }

  current = rb_next(current);
}while(current != NULL) 

if ( target_struct != NULL ) {
		return target_struct;
}


/*More code ....*/ 