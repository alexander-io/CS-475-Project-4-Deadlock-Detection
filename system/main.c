/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define N 6

lid32	printer_lock;
lid32	mylock[N];


/**
 * Delay for a random amount of time
 * @param alpha delay factor
 */
void	holdup(int32 alpha)
{
	long delay = rand() * alpha;
	while (delay-- > 0)
		;	//no op
}

/**
 * Work for a random amount of time
 * @param id ID of worker
 */
void	work(uint32 id)
{
	acquire(printer_lock);
	kprintf("Worker %d: Buzz buzz buzz\n", id);
	release(printer_lock);
	holdup(10000);
}


/**
 * Worker code
 * @param id ID of worker
 */
void	worker(uint32 id)
{
	if (id == 0)
	{
		acquire(mylock[0]);
		work(id);
		acquire(mylock[1]);
		work(id);
		acquire(mylock[4]);
		work(id);
		acquire(mylock[5]);
		work(id);
		release(mylock[5]);
		release(mylock[4]);
		release(mylock[1]);
		release(mylock[0]);
	}
	else if (id == 1)
	{
		acquire(mylock[1]);
		work(id);
		acquire(mylock[0]);
		work(id);
		acquire(mylock[4]);
		work(id);
		acquire(mylock[5]);
		work(id);
		release(mylock[5]);
		release(mylock[4]);
		release(mylock[0]);
		release(mylock[1]);
	}
	else if (id == 2)
	{
		acquire(mylock[2]);
		work(id);
		acquire(mylock[3]);
		work(id);
		acquire(mylock[5]);
		work(id);
		acquire(mylock[4]);
		work(id);
		release(mylock[4]);
		release(mylock[5]);
		release(mylock[3]);
		release(mylock[2]);
	}
	else if (id == 3)
	{
		acquire(mylock[3]);
		work(id);
		acquire(mylock[2]);
		work(id);
		acquire(mylock[5]);
		work(id);
		acquire(mylock[4]);
		work(id);
		release(mylock[4]);
		release(mylock[5]);
		release(mylock[2]);
		release(mylock[3]);
	}
}

int	main(uint32 argc, uint32 *argv)
{
	int i;
	printer_lock = lock_create();
	for (i=0; i<N; i++)
		mylock[i] = lock_create();

	ready(create((void*) worker, INITSTK, 15, "Worker 0", 1, 0), FALSE);
	ready(create((void*) worker, INITSTK, 15, "Worker 1", 1, 1), FALSE);
	ready(create((void*) worker, INITSTK, 15, "Worker 1", 1, 2), FALSE);
	ready(create((void*) worker, INITSTK, 15, "Worker 1", 1, 3), FALSE);


	return 0;
}
