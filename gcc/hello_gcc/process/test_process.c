#include <stdio.h>
//#include "add.h"
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


struct thread_vals
{
	int start;
	int count;
	int *volatile sync_mem;
	int proc_state;
};

void print_numbers(int start, int count, volatile int *sync_mem, int proc_state);
void wrapper_print_nums(struct thread_vals *x);

void print_numbers(int start, int count, int *sync_mem, int proc_state)
{
	int i;

	

	for(i = 0; i< count; i++)
	{	
		
		while(*sync_mem != proc_state)	
		{
			;//printf("sync_mem= %d proc_state = %d\n", *sync_mem, proc_state);
			//sleep(1);
		}

		printf("number = %d\n", start);
		start += 2;

		*sync_mem = (proc_state + 1)& 1;
		if(start & 1)
		{
			sleep(1);
		}
	}
}

void wrapper_print_nums(struct thread_vals *x)
{
	print_numbers(x->start, x->count, x->sync_mem, x->proc_state);
}

volatile int sync_mem = 0;

#if 0
void main()
{
	
	if(fork()==0)
	{
		print_numbers(0, 5, &sync_mem, 0);
	}
	else
	{
		print_numbers(1, 5, &sync_mem, 1);
	}
	printf(" Program executed successfully\n");
}
#else
void main()
{
	struct thread_vals even, odd;

	even.start = 0;
	even.count = 5;
	even.sync_mem = &sync_mem;
	even.proc_state = 0;

	odd.start = 1;
	odd.count = 5;
	odd.sync_mem = &sync_mem;
	odd.proc_state = 1;

	pthread_t odd_thread;

	if(pthread_create(&odd_thread, NULL, wrapper_print_nums, &odd))
	{
		printf("Error Creating threads\n");
	}	

	wrapper_print_nums(&even);

	//print_numbers(0, 5, &sync_mem, 0);

	if(pthread_join(odd_thread, NULL))
	{
		printf("Error\n");		
	}
	printf(" Program executed successfully\n");
}
#endif
