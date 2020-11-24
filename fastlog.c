#include "fastlog.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
struct entry {
    pid_t pid;
    struct timespec time;
    LEVEL lvl; 
    char message[MAX_MSG_LENGTH];
};

// function to check the buffer end
void buffEnd(void);
struct entry *buffptr;
struct entry buffer[MAX_LOG_ENTRY];
int counter;
struct sigaction sa;
void handler (int sig);
void fastlog_init(void)
{
    //initialize sigsction struct
    memset(&sa,'\0',sizeof(sa));
    // Initialize the counter
	counter = 0;
	// set the pointer to point on the array
	buffptr = buffer;
    //using handler we need to handle one parameter
    sa.sa_handler = &handler;
    //setting the handler
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
}


void fastlog_write(LEVEL lvl, char *text)
{
    int diff;
  if (lvl<0 || lvl>3){
      printf("level needs to be between 1 and 3 \n");
      return;
  }
   // assigning the level to the buffer
    buffptr[counter].lvl = lvl; 

	// checking if the string is not null
    if (text != NULL) {
        // making sure that the string is in the norme
        if(strlen(text) > MAX_MSG_LENGTH) {
            diff = strlen(text) - MAX_MSG_LENGTH;
            //coppy the string in the buffer and making sure is same lenght MAX_MSG_LENGTH 
            strncpy(buffptr[counter].message, text, strlen(text) - diff);
        } else {
            //coppy the string in the buffer
            strcpy(buffptr[counter].message, text); 
        }
    } else {
        //adding an empty string if text is null
		strcpy(buffptr[counter].message, "");
	}
    //storing the pid in the buffer
    buffptr[counter].pid = getpid();
    //storing the time in the buffer
	clock_gettime(CLOCK_REALTIME, &buffptr[counter].time);
    counter++;
      // Check if the buffer is overflowed 
    if (counter < 0) {
        return;
    }
    // checking if we need to reset the counter
      if (counter == MAX_LOG_ENTRY) {
        counter = 0;
    }
}



void fastlog_dump(void) 
{
    char* buffMessage;
    char* buffTime = malloc(sizeof(char) * MAX_MSG_LENGTH);
    struct tm *mytm;
    int c = 0;
    int buffLevel;
    long buffPid;
    char str[1024];

    while (c < MAX_LOG_ENTRY) {
        //clearing the string before using it
        memset(str,0,sizeof(str));
        buffPid = (long) buffptr[c].pid;
         // getting the time from the buffer
        mytm = localtime(&buffptr[c].time.tv_sec);
        //formating the time 
        strftime(buffTime, MAX_MSG_LENGTH, "%F %I:%M:%S", mytm);
        //geting level from the buffer
        buffLevel = buffptr[c].lvl;
        // getting the string from the buffer
         buffMessage = buffptr[c].message;
        //building the string
        snprintf(str,sizeof(str),"[%ld]-[%s.%.9ld]-[%d]-<%s>\n", buffPid, buffTime, buffptr[c].time.tv_nsec, buffLevel, buffMessage);
        //writing the string in stderr
        write(2,str,1024);
        c++;
    }
   

	// free the time
    free(buffTime);
    buffTime = NULL;
}
void handler (int sig){
    char sigstr [1024];

    switch (sig)
    {
    case SIGINT:
        //catch SIGINT and make sure that CTRL-C was caught and ignored
       fastlog_write(ERROR, "CTRL-C and ignored");
        break; 
    case SIGUSR1:
        //catch SIGUSR1 and dump the buffer
        fastlog_dump();
        fflush(stdout);
        break;
      
    case SIGSEGV:
       //clearing the string before using 
       memset(sigstr,0, sizeof(sigstr));
       //building the message
       snprintf(sigstr,sizeof(sigstr),"\nSeg fault was encountered and dumping the buffer \n");
       //writing the string in stderr
       write(2,sigstr,1024);
        //dump the buffer
        fastlog_dump();
        exit(-1);
        break;      
    
    default:
        break;
    }
}