#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>

#define sem_name "/bolla21"
sem_t *sem;
void signalHandler(int);

void signalHandler(int SIGVAL) {
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);

	if(SIGVAL == SIGINT) {
		fprintf(stderr, "%sCTRL- Signal interrupt is initiated.\n");
	}

	if(SIGVAL == SIGALRM) {
		fprintf(stderr, "%sMaster time out. Terminating remaining processes.\n");
	}
	//sem_unlink(SEMNAME);
	kill(-getpgrp(), SIGQUIT);/*kills all the children of process*/
	exit(1);
	
}

int main (int argc, char *argv[]) {
	pid_t childpid = 0;
	int i,j;
	if (argc <1){ /* check for valid number of command-line arguments */
		fprintf(stderr, "Usage: %s processes\n", argv[0]);
		return 1;
	}	
	int t,l,s,h,c;
	signal(SIGALRM,signalHandler);	
	alarm(2);
	
	while ((c = getopt (argc, argv, "hs:l:t:")) != -1)
    	switch (c)
      	{
		 case 'h':
       			fprintf(stderr, "Usage: -h[HELP] -s [processspawned] -l [filename] -t[time in sec]\n", argv[0]);
        		return 1;
	
      		case 's':
        		s = atoi(optarg);
        		break;
      		case 'l':
        		l = atoi(optarg);
        		break;
		case 't':
			t=atoi(optarg);
			break;	
			
      		default:
      		fprintf(stderr, "Usage:\n", argv[0]);
      	}	
	
	// handle SIGNALS callback attached
		
 	signal(SIGINT, signalHandler);

  	int segment_id;
  	int segment_size;
	int buf[1];
 
  	/* Allocate a shared memory segment.  */
        key_t key1 = ftok(".", 'x');
  	segment_id = shmget (key1, sizeof(buf[1]), 0777| IPC_CREAT);
  	/* Attach the shared memory segment.  */
  	//printf("Shared memory segment ID is %d\n", segment_id);
  	int *shared_mem =  shmat (segment_id, 0, 0);
	shared_mem[0] = 0;
        shared_mem[1] = 0;
        //fprintf(stderr, "Shared Memory copied \n"); 


	int segment_id2;
        int segment_size2;	
        int buf2[1];

        /* Allocate a shared memory segment.  */
        key_t key2 = ftok(".", 'y');
        segment_id2 = shmget (key2, sizeof(buf2[1]), 0777| IPC_CREAT);
           
	int *shared_msg =  shmat (segment_id2, 0, 0);
	sem = sem_open(sem_name,O_CREAT,0666,1);
        shared_msg[0] = 0;
        shared_msg[1] = 0;
	shared_msg[2] = 0;
	   		
	char arg2[50];
 	char arg3[50];
	char arg4[50];
	snprintf(arg2,50,"%d",segment_id);
	snprintf(arg3,50,"%d",segment_id2);
	snprintf(arg4,50,"%s",sem_name);
	int k;
	for(k = 0; k < s ; k++)
	{
		fprintf(stderr, "child Executed %d \n", k);	
  		if (fork() == 0) 
   		{
			execlp("./user","./user",arg2,arg3,arg4,(char *)NULL);// If we get here, exec failed
			fprintf(stderr,"%s failed to exec worker!\n",argv[0]);		
			exit(-1); 
		}	
	}
	
	while(1)
	{
		if(shared_mem[0] >= 1000000000)
		{
			shared_mem[0] = 0;
  			shared_mem[1] += 1; 
		}
		shared_mem[0] += 1;
	
		//fprintf(stderr, "%d \n", shared_msg[2]);	
		if(shared_msg[2] > 0)
		{
		fprintf(stderr, "Child Pid \n ");
		fprintf(stderr, "OSS: child pid %d is terminating at my time %d.%d, beacuse it reached %d.%d in user", shared_msg[2], shared_mem[1],shared_mem[0],shared_msg[0], shared_msg[1]);	
		shared_msg[2] = 0;
       		}
	 }



	int childrenSpawned = k;

	while(wait(NULL)>0)
	{
	};

	fprintf(stderr,"Master : Milliseconds = %d, Seconds = %d \n",shared_mem[0],shared_mem[1]);
	/* Detach the shared memory segment.  */
  	shmdt ((void *) shared_mem);
  	shmctl (segment_id, IPC_RMID, NULL);
	sem_unlink(sem_name);	
	return 0;
}

