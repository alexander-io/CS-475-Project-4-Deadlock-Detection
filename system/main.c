/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5	//number of philosophers and forks

// locks must be declared and initialized here
// mutex_t locks[N]; // c initializes global scope array index values to 0, FALSE is an alias to 0
lid32 locks[N];
lid32 print_lock; //
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
 * Eat for a random amount of time
 */
void	eat()
{
	holdup(10000);
}

/**
 * Think for a random amount of time
 */
void	think()
{
	holdup(1000);
}

/**
 * Philosopher's code
 * @param phil_id philosopher's id
 */
void	philosopher(uint32 phil_id)
{

	srand(phil_id); //seed
	uint32 right = (phil_id+N-1)%N; // right fork
	uint32 left = phil_id;	//  left fork
	int r; // declare random integer variable used for 30/70% comparison



	while (TRUE)
	{
		// 70/30
		r = rand()%10;
		if (r<3){
			acquire(print_lock);
			printf("Philosopher %d decided to eat\n", phil_id);
			release(print_lock);

			// if (FALSE){
			// acquire the left fork


			acquire(locks[left]);


			// acquire(print_lock);
			// // printf("2\n");
			// release(print_lock);
			acquire(print_lock);

			kprintf("philosopher %d acquired their left fork\n", phil_id);
			release(print_lock);


			// test the right lock, set it if its not acquired
			if (!test_and_set(locks[right])){ // FIXME ? 

				// kprintf("philosopher %d acquired their", phil_id);

				acquire(print_lock);
				printf("Philosopher %d eating : nom nom nom\n", phil_id);
				release(print_lock);

				


				eat();

				acquire(print_lock);
				kprintf("phil %d about to release RIGHT fork\n", phil_id);
				release(print_lock);
				

				release(locks[right]);

				acquire(print_lock);
				kprintf("phil %d has released RIGHT fork\n", phil_id);
				release(print_lock);

			}
			acquire(print_lock);
			kprintf("phil %d about to release LEFT fork\n", phil_id);
			release(print_lock);
			

			release(locks[left]);

			acquire(print_lock);
			kprintf("phil %d has released LEFT fork\n", phil_id);
			release(print_lock);

		} else {

			acquire(print_lock);
			kprintf("Philosopher %d thinking : zzzzzZZZz\n", phil_id);
			release(print_lock);

			think();
		} //think 70% of the time
	}
}

int	main(uint32 argc, uint32 *argv)
{
	// kprintf("hello world\n");
	int i;
	for(i=0;i<N;i++){
		locks[i] = lock_create();
	}
	print_lock = lock_create();

	//do not change
	ready(create((void*) philosopher, INITSTK, 15, "Ph1", 1, 0), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph2", 1, 1), FALSE);
	// ready(create((void*) philosopher, INITSTK, 15, "Ph3", 1, 2), FALSE);
	// ready(create((void*) philosopher, INITSTK, 15, "Ph4", 1, 3), FALSE);
	// ready(create((void*) philosopher, INITSTK, 15, "Ph5", 1, 4), FALSE);
	return 0;
}
