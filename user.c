#include<stdio.h>
#include <sys/shm.h>
#include <semaphore.h>


void main(int argc, char *argv[])
{
	int i,random;
	int id = atoi(argv[1]);
	int *shared_mem = shmat(id,0,0);

	int id2 = atoi(argv[2]);
        int *shared_msg = shmat(id2,0,0);

       printf("Nanosec: %d, Miliseconds : %d, Seconds : %d\n",shared_msg[0],shared_msg[1],shared_msg[2]);

	char *semname = argv[3];

	sem_t *sem = sem_open(semname,0);

	random = (rand() % (1000000 -1+1)) + 1;
	
//	fprintf(stderr, "%d \n", random);
	int childtime = (shared_mem[0] + random) + (shared_mem[1] * 1000000000); 
 //	fprintf(stderr, "%d", childtime);
//	printf("Miliseconds : %d, Seconds : %d\n",shared_mem[0],shared_mem[1]);
	while(1)
	{
// fprintf(stderr,"%d \n", shared_mem[2]);
	
	//fprintf(stderr, "Before Sem Wait \n");
		sem_wait(sem);
		//fprintf(stderr, " %d  Passed Wait \n",getpid());
		//if((shared_mem[0] + (shared_mem[1] * 1000000000)) >= childtime)
		//{
	//	fprintf(stderr, "Passed \n");
	//	shared_msg[0] = shared_mem[1];
	//	shared_mem[1] = shared_mem[0];
		shared_mem[2] = getpid();
//		fprintf(stderr,"%d \n", shared_mem[2]);
		//sem_post(sem);
		//break;
	//	}
		sem_post(sem);
		//fprintf(stderr, "After Sem Post \n");
	}
}
