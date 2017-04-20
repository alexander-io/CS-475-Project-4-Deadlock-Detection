/* resched.c - resched */

#include <xinu.h>

/**
 * Reschedule processor to next ready process
 *
 */
void	resched(void)		// assumes interrupts are disabled
{
	struct procent *ptold;	// ptr to table entry for old process
	struct procent *ptnew;	// ptr to table entry for new process

	// If rescheduling is deferred, record attempt and return
	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	// Point to process table entry for the current (old) process
	ptold = &proctab[currpid];

	//  check ptold's state. If it's running, put it on the ready queue and change state to ready
	if (ptold->prstate == PR_CURR){
		ptold->prstate = PR_READY;
		enqueue(currpid, readyqueue, ptold->iniprprio);
	}

	// dequeue
	pid32 id = dequeue(readyqueue);

	//  dequeue next process off the ready queue and point ptnew to it
	ptnew = &proctab[id];

	//  change its state to "current" (i.e., running)
	ptnew->prstate = PR_CURR;

	// If the AGING kernel configuration variable is #defined as TRUE
	// then we'll loop through the queue and age each entry accordingly.
	if (AGING){
		// loop through ready queue,
		// if the current queue entry is valid for aging,
		// then modify its priority (age)
		struct qentry *tmp = readyqueue->head;

		while (tmp != NULL){
			// age each queue entry...
			// except for the null process && except for the most recently preempted process &&
			// except for the most recently preempted process
			// except for the process that was most recently selected for scheduling
			if (tmp->process_id != NULLPROC && tmp->process_id != currpid){
				// decrement the priority of of the process entry
				tmp->key++;
			}
			tmp = tmp->next;
		}
	}

	//  set currpid to reflect new running process' PID
	currpid = id;

	// part 2.7
	preempt = QUANTUM;

	// Context switch to next ready process
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);



	// Old process returns here when resumed
	return;
}
