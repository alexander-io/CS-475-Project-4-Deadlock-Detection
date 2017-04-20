#include <xinu.h>

/**
 * Enter a critical section guarded by lock
 * It is asssumed that a mutex_t variable storing FALSE means it is unlocked,
 * and TRUE implies that it is locked.
 * @param lock	pointer to a mutex lock
 */
void mutex_lock(mutex_t *lock)
{
	// write a busy-wait loop using test_and_set
  while(test_and_set(lock)){
    ; // busy wait
  }
}

/**
 * Leave a critical section guarded by lock
 * @param lock	pointer to a mutex lock
 */
void mutex_unlock(mutex_t *lock)
{
	//unlock!
  (*lock) = 0;
}
