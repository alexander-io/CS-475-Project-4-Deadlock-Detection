#include <xinu.h>

local	lid32	newlock(void);

/**
 * Create a new semaphore and return the ID to the caller
 * @return ID of the mutex to caller, or SYSERR
 */
lid32	lock_create()
{
	intmask	mask;		/* saved interrupt mask	*/
	lid32	lockid;		/* lock ID to return	*/

	mask = disable();

	if ((lockid = newlock()) == SYSERR)
	{
		restore(mask);
		return SYSERR;
	}

	restore(mask);
	return lockid;
}

/**
 * Allocate an unused lock and return its index
 * @return	ID of free lock
 */
local	lid32	newlock(void)
{
	static	lid32	nextlock = 0;	/* next lockid to try	*/
	lid32	lockid;			/* ID to return	*/
	int32	i;			/* iterate through # entries	*/

	// START
	// loop through each element in the lock table.
	for(i=0;i<NLOCK;i++){

		// find a lock that is free to use
		if (locktab[i].lock == 0) {
			// set its state to used, and reset the mutex to FALSE
			locktab[i].state = LOCK_USED;
			// return its lockid
			return i;
		}
	}
	// if there is no such lock, return SYSERR
	return SYSERR; // END
}


/**
 * Delete a lock by releasing its table entry
 * @param ID of lock to release
 */
syscall	lock_delete(lid32 lockid)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}

	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	// START
	// reset lock's state to free and the mutex to FALSE
	locktab[lockid].state = LOCK_FREE;
	mutex_unlock(locktab[lockid].lock);

	// traverse the queue entries at index lockid in the lock-table
	struct qentry *curr_entry = locktab[lockid].wait_queue->head;
	while (curr_entry!=NULL){
		// remove all processes waiting on its queue, and send them to the ready queue
		enqueue(remove(curr_entry->process_id, locktab[lockid].wait_queue), readyqueue, curr_entry->key);

		// move to the next entry
		curr_entry = curr_entry->next;
	}

	//TODO (RAG) - remove all RAG edges to and from this lock

	// END

	resched();
	restore(mask);
	return OK;
}


/**
 * Acquires the given lock, or cause current process to wait until acquired
 * @param lockid	lock on which to wait
 * @return status
 */
syscall	acquire(lid32 lockid)
{
	intmask mask;			// saved interrupt mask
	struct	lockentry *lptr;	// ptr to sempahore table entry

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}

	lptr = &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	// START
	// enqueue the current process ID on the lock's wait queue
	enqueue(currpid, locktab[lockid].wait_queue, 69); // arbitrary priority value

	//TODO (RAG) - add a request edge in the RAG
	// END

	restore(mask);				//reenable interrupts

	// START
	//lock the mutex!
	mutex_lock(locktab[lockid].lock);
	// END

	mask = disable();			//disable interrupts

	// START
	//TODO (RAG) - we reache this point. Must've gotten the lock! Transform request edge to allocation edge
	// END

	restore(mask);				//reenable interrupts
	return OK;
}

/**
 * Unlock, releasing a process if one is waiting
 * @param lockid ID of lock to unlock
 * @return status
 */
syscall	release(lid32 lockid)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	lockentry *lptr;	/* ptr to lock table entry	*/

	mask = disable();
	if (isbadlock(lockid)) {
		restore(mask);
		return SYSERR;
	}
	lptr= &locktab[lockid];
	if (lptr->state == LOCK_FREE) {
		restore(mask);
		return SYSERR;
	}

	// START
	// remove current process' ID from the lock's queue
	remove(currpid, locktab[lockid].wait_queue);
	// unlock the mutex
	mutex_lock(locktab[lockid].lock);

	//TODO (RAG) - remove allocation edge from RAG
	// END

	restore(mask);
	return OK;
}
