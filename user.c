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


	char *semname = argv[3];
	sem_t *sem = sem_open(semname,0);

	random = (rand() % (1000000 -1+1)) + 1;
	
	int childtime = (shared_mem[0] + random) + (shared_mem[1] * 1000000000); 
	while(1)
	{
	
		sem_wait(sem);
		if((shared_mem[0] + (shared_mem[1] * 1000000000)) >= childtime && shared_msg[2] == 0)
		{
		shared_msg[0] = shared_mem[1];
		shared_msg[1] = shared_mem[0];
		shared_msg[2] = getpid();
		sem_post(sem);
		break;
		}
		sem_post(sem);
	}
}
