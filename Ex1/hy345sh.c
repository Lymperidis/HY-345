#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include <fcntl.h>
#include<readline/readline.h> 
#include<readline/history.h>
#include "structs.h"
#define MAX_INPUT_LENGTH 256

void printDir() 
{ 
    char dir[1024]; 
    getcwd(dir,sizeof(dir));    /*Just print a directory*/
    printf("%s ", dir); 
} 


void type_prompt(){
    char *user;
    user = getenv("USER");      /*Get the user using the computer*/
    printf("<csd4813>-hy345sh@<%s>:",user);
    printDir();
}



void readInput(buffer *input){

    input->userin = NULL;
    input->size = 0;
    getline(&input->userin,&input->size,stdin);
    input->size = sizeof(input->userin);    /*just read a line of text and return it to main function*/

    
} 

/*In this function the line of text will be parsed and each command will be given each attributes*/
void declare_the_commands(buffer input,commandarray *commarr){
    int i,size,j,k,flag;
    int arg_counter,append;
    int commandsarray_counter;
    size = 0 ;
    char *array[100];
    char *token = strtok(input.userin,"\n" "\t" " ");
    i=0;
    while(token != NULL){ 
        array[i] = token;                           /*parse the string with delimeters*/
        token = strtok(NULL,"\n" "\t" " ");         /*save everything else in a string*/
        i++;
    }
    size = i ; 
    /*starting to seperate every command and arguments*/
    i=0;
    arg_counter = 0 ; 
    commandsarray_counter = 0 ; 
    commarr->comm_size = 0;
    command tempcomm;
    tempcomm.pipe = 0 ;
    tempcomm.sizeofargs = 0;            /*Initialize a temp that will take everything from the array in each loop*/
    tempcomm.redirectin = 0;
    tempcomm.redirectout =0;
    tempcomm.append = 0;
    tempcomm.file = NULL;
    flag = 0;
    int redirout = 0;
    int redirin = 0;
    append = 0;
    for(int i = 0; i < size ; i++){
        tempcomm.args[arg_counter] = array[i];      /*temp will be initialized with every word on the string array */
        arg_counter++;
        tempcomm.sizeofargs++;
        tempcomm.comm = tempcomm.args[0];
        flag = 2;

        if(tempcomm.redirectout == 1 && redirout == 1){
            tempcomm.file = array[i];
            redirout = 0;
        }

        if(tempcomm.redirectin == 1 && redirin == 1){           /*CASES OF REDIRECTION AND APPEND*/
            tempcomm.file = array[i];
            redirin = 0;
        }

        if(tempcomm.append == 1 && append == 1){
            tempcomm.file = array[i];
            append = 0 ;
        }
        
    
         if(strcmp(array[i],"|") == 0 ){
             command temp2;
             temp2.pipe = 1 ;
             temp2.file = tempcomm.file;
             temp2.append = tempcomm.append;
             temp2.redirectout = tempcomm.redirectout;
             temp2.redirectin = tempcomm.redirectin;
             temp2.comm = tempcomm.args[0];                 /*This is the handling when the loop finds a |*/
             temp2.sizeofargs = arg_counter-1;
             j=0;
             k=0;
            while(j < temp2.sizeofargs){
                if(strcmp(tempcomm.args[j],">") == 0){
                    break;
                }
                if(strcmp(tempcomm.args[j],"<") == 0){      /*cases of redirection again*/
                    break;
                }
                if(strcmp(tempcomm.args[j],">>") == 0){
                     break;
                }
                 temp2.args[k] = tempcomm.args[j];          /*input the args in the temp that will be inside the commandsarray*/
                k++;
                j++; 
            }
             temp2.args[k] = NULL;                  /*put NULL at the end of the args*/
             arg_counter = 0 ;
             commarr->commands[commandsarray_counter] = temp2;  /*insert temp in the commands array*/
             commandsarray_counter++;
             commarr->comm_size++;
             tempcomm.sizeofargs = 0 ;              
             flag = 1;
            if(tempcomm.redirectout == 1){
                tempcomm.redirectout = 0;
            }
            if(tempcomm.redirectin == 1){           /*return the flags of the redirect and append in the normal state */
                tempcomm.redirectin = 0;
            }
            if(tempcomm.append == 1 ){
                tempcomm.append = 0;
            }

        }else if(strcmp(array[i],";") == 0 ){
            command temp3;
            temp3.pipe = 0;
            temp3.file = tempcomm.file;
            temp3.redirectout = tempcomm.redirectout;
            temp3.redirectin = tempcomm.redirectin;     /*This is handling when the loop finds the ; */
            temp3.append = tempcomm.append;
            temp3.comm = tempcomm.args[0];
            temp3.sizeofargs = arg_counter -1 ;
            j=0;
            k=0;
            while(j < temp3.sizeofargs){
                if(strcmp(tempcomm.args[j],">") == 0){
                    break;
                }
                if(strcmp(tempcomm.args[j],"<") == 0){      /*CASES OF REDIRECTION AND APPEND*/
                    break;
                }
                if(strcmp(tempcomm.args[j],">>") == 0){
                    break;
                }
                temp3.args[k] = tempcomm.args[j];
                k++;
                j++;
             
            }
            temp3.args[k] = NULL;
            arg_counter = 0;
            commarr->commands[commandsarray_counter] = temp3;
            commandsarray_counter++;
            commarr->comm_size++;
            tempcomm.sizeofargs = 0 ;
            
            flag = 1;
            if(tempcomm.redirectout == 1){
                tempcomm.redirectout = 0;
            }
            if(tempcomm.redirectin == 1){           /*return the flags in the normal state */
                tempcomm.redirectin = 0;
            }
            if(tempcomm.append == 1){
                tempcomm.append = 0;
            }
        }

        if(strcmp(array[i],">") == 0 ){ 
            tempcomm.redirectout = 1;
            redirout = 1;
        }

        if(strcmp(array[i],"<") == 0){      /*CASES OF REDIRECT AND APPEND FLAGS!*/
            tempcomm.redirectin = 1;
            redirin = 1 ;

        }
        if(strcmp(array[i],">>") == 0){
            tempcomm.append = 1;
            append = 1 ;
        }
    }
    if(flag == 2){
        if(strcmp(tempcomm.args[arg_counter-2],">") == 0){
            tempcomm.sizeofargs = tempcomm.sizeofargs - 2;
            tempcomm.args[arg_counter-2] = NULL;
            commarr->commands[commandsarray_counter] = tempcomm;
            commarr->comm_size++; 
        }else if(strcmp(tempcomm.args[arg_counter-2],"<") == 0){
            tempcomm.sizeofargs = tempcomm.sizeofargs - 2;
            tempcomm.args[arg_counter-2] = NULL;
             commarr->commands[commandsarray_counter] = tempcomm;       /*If there is nor ; nor | at the end redirect cases*/
            commarr->comm_size++; 
        }else if(strcmp(tempcomm.args[arg_counter-2],">>") == 0){
            tempcomm.sizeofargs = tempcomm.sizeofargs - 2;
            tempcomm.args[arg_counter-2] = NULL;
            commarr->commands[commandsarray_counter] = tempcomm;
            commarr->comm_size++; 
        }
        else{
            tempcomm.args[arg_counter] = NULL;
            commarr->commands[commandsarray_counter] = tempcomm;        /*if there is nor ; nor | normal case*/
            commarr->comm_size++;   
        }
    }

 }

 void exec_quit(command command){
    if(strcmp(command.comm,"quit")==0){ /*exec quit */
        exit(1);
    }
 }

char* previousDirectory = NULL;

int changeDirectory(command command) {

    if (strcmp(command.comm, "chdr") && strcmp(command.args[1],"..") == 0) {
        // Handle "<chdr>" to go back to the previous directory
        chdir(getenv("OLDPWD"));
    } else {
        // Handle "cd" with a directory argument
        char *current_directory = getcwd(NULL, 0);          /*change directory code with chdr */
        if (current_directory == NULL) {
            perror("getcwd");
            return 1;
        }

        if (setenv("OLDPWD", current_directory, 1) == -1) {
            perror("setenv");
            return 1;
        }

        free(current_directory);

        if (chdir(command.args[1]) != 0) {
            perror("chdir");
            return 1;
        }
    }

    return 0;
}

 void translate_commands(command command){
    int status;
    int pid;
    pid = fork();
    if (pid == 0)
    { // Child process
        execvp(command.comm, command.args);
        perror("ERROR : Cant execute commands\n");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    { // Error
        perror("ERROR: Forking went wrong.\n");             /*execute normal commands*/
        exit(EXIT_FAILURE);
    }
    else
    { // Parent process
        waitpid(pid, &status, WUNTRACED);
    }
}
void redirect_commandsout(command command){
    int pid;
    int status;
    pid = fork();
    if (pid == 0)
    { // Child process
        int file = open(command.file, O_WRONLY | O_CREAT | O_TRUNC,0644);
        if(file == -1){
            perror("ERROR : Couldn't open file\n");
            exit(EXIT_FAILURE);
        }

        dup2(file,STDOUT_FILENO);

        execvp(command.comm,command.args);
        close(file);
        perror("ERROR: Can't execute command.\n");      /*redirect out case */
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    { // Error
        perror("ERROR: Forking went wrong.\n");
        exit(EXIT_FAILURE);
    }
    else
    { // Parent process
        waitpid(pid, &status, WUNTRACED);
    }

 }
void redirect_commandsin(command command){
    int pid;
    int status;
    pid = fork();
     if (pid == 0)
    {   int file = open(command.file, O_RDONLY);
        if(file == -1){
            perror("ERROR: Couldn't open file\n");  /*redirect in case */
            exit(EXIT_FAILURE);
        }

        dup2(file,STDIN_FILENO);

        execvp(command.comm,command.args);
        close(file);
        perror("ERROR: Can't execute command.\n");
        exit(EXIT_FAILURE);
       
    }
    else if (pid < 0)
    { // Error
        perror("ERROR: Forking went wrong.\n");
        exit(EXIT_FAILURE);
    }
     else
    { // Parent process
        waitpid(pid, &status, WUNTRACED);
    }
}

void append(command command){
    int pid;
    int status;
    pid = fork();
    if(pid==0){
        int file = open(command.file, O_WRONLY | O_CREAT | O_APPEND,0644);
        if(file == -1){
            printf("Couldn't open file\n");
            return;
        }

        dup2(file,STDOUT_FILENO);

        execvp(command.comm,command.args);  /*append case */
        close(file);
    }
    else if (pid < 0)
    { // Error
        perror("ERROR: Forking went wrong.\n");
        exit(EXIT_FAILURE);
    }
    else
    { // Parent process
        waitpid(pid, &status, WUNTRACED);
    }
} 
void pipes(commandarray *array,int *i){ 
    int fd[2]; // one fd for output one for input 
    int fd_store = 0;
    int status; 
    int num_pipes = array->comm_size - 1;
    int pid;
    // For every pipe fork and execute processes.
    for (; *i < array->comm_size; (*i)++)
    {
        command command = array->commands[*i];
        pipe(fd);
        pid = fork();

        if (pid == 0)
        { // Child process
            //redirect in
            if (command.redirectin == 1 && command.redirectout == 0) {
                int file = open(command.file, O_RDONLY);
                if (file < 0) {
                    perror("ERROR: Input redirection failed");  /*pipes handling and also redirection inside the pipe */
                    exit(EXIT_FAILURE);
                }
                
                dup2(file, STDIN_FILENO);


                close(file);
            }
            dup2(fd_store, STDIN_FILENO);               /*Have the dup2 change the stdin*/
            if (*i < num_pipes) {
                 dup2(fd[1], STDOUT_FILENO);
            }                                       /*now change the sdout as well with a pipe  */
            close(fd[0]);               /*close file discriptor*/
            if(command.pipe == 0){
                //redirect out
                if (command.redirectout == 1 && command.redirectin == 0) {
                    int file1 = open(command.file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (file1 < 0) {
                        perror("ERROR: Output redirection failed");
                        exit(EXIT_FAILURE);
                    }
                    dup2(file1, STDOUT_FILENO);             /*if it is normal command change file discriptor the stdout probably the last command in a pipe*/
                    close(file1);                       /*close file*/
                }

                //output append  
                if(command.append == 1){
                    int file2 = open(command.file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (file2 < 0) {
                        perror("ERROR: Output redirection failed");  /*append inside the pipe commands*/
                        exit(EXIT_FAILURE);
                    }
                    dup2(file2, STDOUT_FILENO);         /*change the stdout inside the file and just write inside the file */
                    close(file2);
                }
            }
            if (execvp(command.comm, command.args) == -1)
            {
                perror("ERROR: Problem with pipes.");       /*if the execvp command didnt execute correctly */
                exit(EXIT_FAILURE);
            }
        
            
        }
        else if (pid < 0)
        { // Error
            perror("ERROR: Forking went wrong.\n");     /*if the is a problem creating a fork */
            exit(EXIT_FAILURE);
        }
         else
         { // Parent process
             waitpid(pid, &status, WUNTRACED);      /*wait for kid to finish so the parent must wait */
             close(fd[1]);
             fd_store = fd[0];
         }
    }
}
void execute_commands(command command,commandarray *array,int *i){

    if(strcmp(command.comm,"chdr")==0){             /*change directory*/
        changeDirectory(command);
    }else if(strcmp(command.comm,"quit")==0){               /*quit command*/
        exec_quit(command);
    }else{
        if(command.pipe == 1 ){                 /*pipes*/
            pipes(array,i);
        }
        else if(command.pipe == 0 && command.redirectout == 0 && command.redirectin == 0 && command.append == 0){
            translate_commands(command); /*normal commands execute */
        }else if(command.redirectout == 1 && command.redirectin == 0 ){
            redirect_commandsout(command);  /*redirect out */
        }else if(command.redirectin == 1 && command.redirectout == 0){
            redirect_commandsin(command);   /*redirect in */
        }else if(command.append == 1){
            append(command);    /*append */
        }
    }
    


}
int main(){
    buffer input;
    commandarray array;
    command temp;
    while(1){ 
        type_prompt();                  /*!! CALL EACH FUNCTION TO DO EACH DUTY !!*/
        readInput(&input);
        declare_the_commands(input,&array);
        for(int i=0;i<array.comm_size;i++){
            temp = array.commands[i];
            execute_commands(temp,&array,&i);
        } 
    }
   return 0; 
}