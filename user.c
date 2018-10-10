#include<stdio.h>
#include <sys/shm.h>


void main(int argc, char *argv[])
{
	int i;
	int id = atoi(argv[1]);
	int *shared_mem = shmat(id,0,0);
	printf("Miliseconds : %d, Seconds : %d\n",shared_mem[0],shared_mem[1]);

	int id2 = atoi(argv[2]);
        int *shared_msg = shmat(id2,0,0);
        printf("Nanosec: %d, Miliseconds : %d, Seconds : %d\n",shared_msg[0],shared_msg[1],shared_msg[2]);

}
