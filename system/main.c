/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5	//number of philosophers and forks

// locks must be declared and initialized here
mutex_t locks[N]; // c initializes global scope array index values to 0, FALSE is an alias to 0
mutex_t print_lock[1]; //
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
			// acquire the left fork
			mutex_lock(&locks[left]);

			// test the right lock, set it if its not acquired
			if (!test_and_set(&locks[right])){

				mutex_lock(&print_lock[0]);
				printf("Philosopher %d eating : nom nom nom\n", phil_id);
				mutex_unlock(&print_lock[0]);

				eat();

				mutex_unlock(&locks[right]);
			}
			mutex_unlock(&locks[left])
		} else {

			mutex_lock(&print_lock[0]);
			printf("Philosopher %d thinking : zzzzzZZZz\n", phil_id);
			mutex_unlock(&print_lock[0]);

			think();
		} //think 70% of the time
	}
}

int	main(uint32 argc, uint32 *argv)
{
	//do not change
	ready(create((void*) philosopher, INITSTK, 15, "Ph1", 1, 0), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph2", 1, 1), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph3", 1, 2), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph4", 1, 3), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph5", 1, 4), FALSE);
	return 0;
}
