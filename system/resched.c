/* resched.c - resched */

#include <xinu.h>

/**
 * Reschedule processor to next ready process
 *
 */
void	resched(void)		// assumes interrupts are disabled
{
	// kprintf("RESCHED CALL id : %d\n", currpid);
	struct procent *ptold;	// ptr to table entry for old process
	struct procent *ptnew;	// ptr to table entry for new process

	// If rescheduling is deferred, record attempt and return
	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}
	// kprintf("res fun dead: %d\n", deadlock);
	// kprintf("before condition %d\n", deadlock);
	if(deadlock >= 50){
		deadlock = 0;
		kprintf("deadlock: %d\n", deadlock);
		// kprintf("xinusucks\n");
		intmask mask = disable();   //disable interrupts
		kprintf("before detect\n");
		if(deadlock_detect()) {
			kprintf("stop\n");
			restartLinkedList(whiteList);
			restartLinkedList(blackList);
			restartLinkedList(greyList);
			kill(deadlock_recover());
		}
		lockedLock = NULL;
		kprintf("fter detect\n");

		//other code with interrupt disabled
		restore(mask);          //reenable interrupts
	}
	deadlock++;

	// Point to process table entry for the current (old) process
	ptold = &proctab[currpid];
	// kprintf("currpid: %d\n",currpid);
	//  check ptold's state. If it's running, put it on the ready queue and change state to ready
	if (ptold->prstate == PR_CURR){
		ptold->prstate = PR_READY;
		enqueue(currpid, readyqueue, ptold->iniprprio);
	}
	// kprintf("dequeue\n");

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
	// kprintf("after aging\n");
	// printqueue(readyqueue);
	//  set currpid to reflect new running process' PID
	currpid = id;

	// part 2.7
	preempt = QUANTUM;

	// Context switch to next ready process
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);



	// Old process returns here when resumed
	return;
}
