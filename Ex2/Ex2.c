/*CSD4813 LYMPERIS LYMPERIDIS*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <time.h> 

pthread_mutex_t mutex;
int current_capacity = 0;
sem_t semaphore[40];
int classroom[40];
int waitingroom[40];
int numberofstudents;
int temp2;
int j=0;

typedef struct thread {
    int id;
    int time;
    int i;
}thread;

int waitingroom_front = -1;
int waitingroom_rear = -1;
/*Implementation of waiting room with a queue, function to join the waiting room*/
void enqueue(int student_id) {
    if (waitingroom_rear == 39) {
        printf("Waiting room is full. Student %d can't enter.\n", student_id);
    } else {
        // Increment rear and add the student to the waiting room
        waitingroom_rear++;
        waitingroom[waitingroom_rear] = student_id;
    }
}
/*function to exit the waiting room*/
int dequeue() {
    if (waitingroom_front > waitingroom_rear) {
        // Queue is empty
        return -1;
    } else {
        waitingroom_front++;
        int student_id = waitingroom[waitingroom_front];
        waitingroom[waitingroom_front] = 0; // Clear the position
        return student_id;
    }
}

void remove_student(int id){
    for(int i=0;i<40;i++){
        if(id == classroom[i]){
            classroom[i] = 0 ;              /*Remove a student from the classroom[i]*/
            break;
        }
    }
}

void printclassroom(){
    int counter = 0 ;
    printf("The classroom is :\t| ");
    for(int i=0; i<numberofstudents ; i++){
        if(classroom[i] != 0 ){ 
            printf("%d | ",classroom[i]);
            counter++;
        }else if(classroom[i] == 0 && counter < 8 ){        /*Print the classroom*/
            printf(" | ");
            counter++;
        }
    }
    printf("\n");
    printf("The waiting room is:\t| ");
    for(int i=0; i<numberofstudents ; i++){
        if(waitingroom[i] != 0 ){ 
             printf("%d | ",waitingroom[i]);  /*Print the waiting room*/
        }
    }
    printf("\n");
    printf("\n");
}



void *threadfun(void *args) {
    pthread_mutex_lock(&mutex);             /*Locking the mutex so every thread can print themselves*/
    thread *temp = (thread *)args;
    printf("Student %d was born\n", temp->id); 
    if(current_capacity > 7) {          /*Case that if the classroom is full*/
        //The waiting room
        printf("Student %d cannot enter the study hall. It is full\n",temp->id);
        enqueue(temp->id);
        printclassroom();  /*Unlock the mutex so starvation doesnt happen*/
        pthread_mutex_unlock(&mutex);

        sem_wait(&semaphore[temp->id-1]);
    }

    classroom[current_capacity] = temp->id;             /*Get the id in the classroom*/

    printf("Student %d entered the study room\n",temp->id);
    current_capacity++;                     /*Capacity of the classroom*/
    printclassroom();
    pthread_mutex_unlock(&mutex);           /*Unlock the mutex*/

    sleep(temp->time);          /*reading time*/
    
    pthread_mutex_lock(&mutex);             /*Lock the mutex so every thread can leave without interaption */
    
    printf("Student %d exited the study hall after studying %d seconds\n",temp->id,temp->time);
    current_capacity--;

    remove_student(temp->id);               /*Find the id in the classroom and remove it*/
    
    printclassroom();
    if(current_capacity == 0 ){
        while(j < 8 && waitingroom_rear != waitingroom_front){
            temp2 = dequeue();
            sem_post(&semaphore[temp2-1]);              /*If there is no one in class open up 8 threads to go into the classroom*/
            sleep(1);
            j++;
        }

        j=0;
    }

    pthread_mutex_unlock(&mutex);  /*unlock the mutex again*/

    return NULL;
}



int main(){
   if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("XASAME\n");
   }
    printf("Please give a number of students : ");
    scanf("%d",&numberofstudents);
    while(numberofstudents < 20 || numberofstudents > 40){
        printf("Give a number between 20 and 40 please\n");
        printf("Please give a number of students : ");              /*If the number of the user is less than 20 or higher 40 ERROR*/
        scanf("%d",&numberofstudents);

   }
    pthread_t *threads=(pthread_t*)malloc(numberofstudents*sizeof(pthread_t)); /*Init of the two arrays*/
    thread *table = (thread*)malloc(numberofstudents*sizeof(thread));
    for(int i=0;i<40;i++){  
        waitingroom[i]=0;
        classroom[i] = 0; 
        sem_init(&semaphore[i], 0, 0);          /*Initiliaziation of waiting room classroom and semaphore array with init_value 0 so that every thread stops at once*/
    }
    for(int i=0 ; i < numberofstudents ; i++){
        table[i].id = i+1;
        table[i].time = (rand() % (15 - 5 + 1)) + 5;        /*call the thread function every time*/
        table[i].i = i;
        int result = pthread_create(&threads[i],NULL,threadfun,(void*)&table[i]);
        
        if(result){
            fprintf(stderr,"Error creating thread %d : %d\n",i,result);
            exit(-1);       /*if the thread couldnt be created*/
        }

    }
    for(int i=0;i<numberofstudents;i++){
        pthread_join(threads[i], NULL);     /*Terminate all threads*/
    }

    for(int i=0;i<40;i++){
        sem_destroy(&semaphore[i]);   /*terminate the semaphore array*/
    }

    free(threads);
    free(table);

   
return 0;   
}