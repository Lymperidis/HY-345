#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 

typedef struct{

    char *comm;
    char *args[10];
    int sizeofargs;
    int pipe,redirectout,redirectin,append; 
    int pipedcomm;
    char *file;

}command;

typedef struct{
    command commands[100];
    int comm_size;
}commandarray;

typedef struct{
    char *userin;
    size_t size;
}buffer;