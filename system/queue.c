/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
// #include "../include/xinu.h"
// #include "../include/stdio.h"
#include <stdlib.h>

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void	printqueue(struct queue *q)
{
	// print all contents from head to tail
	// format should be [(pid=p1), (pid=p2), ...
	if(q->head == NULL){
		kprintf("[]\n");
		return;
	}
	struct qentry *toPrint = q->head;

	kprintf("[(pid=p%d,%d)",toPrint->process_id, toPrint->key);

	toPrint = toPrint->next;
	while(toPrint != NULL){

		// kprintf(", (pid=p%d)",toPrint->process_id);
		kprintf("[(pid=p%d,%d)",toPrint->process_id, toPrint->key);

		toPrint = toPrint->next;
	}
	kprintf("]\n");
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isempty(struct queue *q)
{
	if (q->size == 0 || q == NULL){
		return 1;	
	} 
	return 0;	
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	nonempty(struct queue *q)
{
	// don't just check q's size because q could be NULL
	if (q->size == 0 || q == NULL){
		return 0;	
	} 
	return 1;
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isfull(struct queue *q)
{
	// check if there are at least NPROC processes in the queue'
	if(q->size >= NPROC){
		return 1;
	}
	return 0;
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 *
 * @return pid on success, SYSERR otherwise
 */
pid32 enqueue(pid32 pid, struct queue *q, int32 key)
{	
    // check if queue is full and if pid is illegal, and return SYSERR if either is true
	if (isfull(q) || isbadpid(pid)){
		return SYSERR;
	}

    // allocate space on heap for a new QEntry
	struct qentry *new_qentry = malloc(sizeof(struct qentry));
    // initialize the new QEntry
	new_qentry->process_id = pid;
	new_qentry->key = key;

	// insert into tail of queue
	if(q->size == 0){
		q->tail = new_qentry;
		q->head = new_qentry;
	}
	else{
		// compare keys
		struct qentry *toCompare = q->head;

		// loop through the entries in the queue, compare the keys
		while(toCompare!=NULL){
			// if we've found a key with a higher priority, let's insert it
			if (toCompare->key < key) {

				// we've found that they key needs to be inserted...
				// check if toCompare is head
				if (toCompare == q->head){
					// insert new_qentry into head
					new_qentry->next = q->head;
					q->head->prev = new_qentry;
					q->head = new_qentry; 
				} else {
					// insert new_qentry into body
					new_qentry->next = toCompare;
					new_qentry->prev = toCompare->prev;
					toCompare->prev->next = new_qentry;
					toCompare->prev = new_qentry;
				}
				break;
			}
			// if we're at the last entry
			if (toCompare == q-> tail && key <= q->tail->key) {
				// new_qentry becomes new tail
				new_qentry->prev = q->tail;
				q->tail->next = new_qentry;
				q->tail = new_qentry;
				toCompare = toCompare->next;
			} 
			toCompare = toCompare->next;
		}
	}
	q->size++;
	return pid;
}


/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{
    // return EMPTY if queue is empty
	if (isempty(q)){
		return EMPTY;	
	}

        // get the head entry of the queue
	struct qentry *old_head = q->head;
	pid32 return_pid = old_head->process_id;
			
    // unlink the head entry from the rest
	if (q->size == 1){
		q->tail = NULL;
		q->head = NULL;
	}
	else{
		q->head = old_head->next;
		q->head->prev = NULL;
	}

    // free up the space on the heap
	free(old_head, sizeof(old_head));
	q->size--;
	return return_pid;
}


/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{
	// return NULL if queue is empty or if an illegal pid is given
	if (isempty(q) || isbadpid(pid)){
		return NULL;
	}

	struct qentry *temp = q->head;

	// find the qentry with the given pid
	while(temp != NULL){
		if (temp->process_id == pid){
			return temp;
		}	
		temp = temp->next;
	}
	// return a pointer to it
	return NULL;
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32	getfirst(struct queue *q)
{
	//return EMPTY if queue is empty
	//remove process from head of queue and return its pid
	return dequeue(q);
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32	getlast(struct queue *q)
{
	//return EMPTY if queue is empty
	if (isempty(q)){
		return EMPTY;	
	}

	//remove process from tail of queue and return its pid
	struct qentry *old_tail = q->tail;
	pid32 return_pid = old_tail->process_id;
			
    //unlink the tail entry from the rest
	if (q->size == 1){
		q->tail = NULL;
		q->head = NULL;
	}
	else{
		q->tail = old_tail->prev;
		q->tail->next = NULL;
	}

    //free up the space on the heap
	free(old_tail, sizeof(old_tail));
	q->size--;
	return return_pid;
}



/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32	remove(pid32 pid, struct queue *q)
{
	//return EMPTY if queue is empty
	if (isempty(q)){
		return EMPTY;	
	}
	// return SYSERR if pid is illegal
	if(isbadpid(pid)){
		return SYSERR;
	}	
	
	//remove process identified by pid parameter from the queue and return its pid
	if(q->head->process_id == pid){
		return dequeue(q);
	}
	if(q->tail->process_id == pid){
		return getlast(q);
	}
	if(q->size > 2){
		struct qentry *checking = q->head->next;
		while(checking != NULL){
			if(checking->process_id == pid){
				checking->prev->next = checking->next;
				checking->next->prev = checking->prev;
				free(checking, sizeof(checking));
				return pid;
			}
			checking = checking->next;	
		}
	}

	//if pid does not exist in the queue, return SYSERR
	return SYSERR;
}

/*
int main(){
	struct queue my_queue = {NULL, NULL, 0};

	//enqueue(pid32 pid, struct queue *q)
	
	printqueue(&my_queue);
	enqueue(1, &my_queue, 1);
	printqueue(&my_queue);
	enqueue(3, &my_queue, 2);
	printqueue(&my_queue);
	// dequeue(&my_queue);
	// printqueue(&my_queue);
	// dequeue(&my_queue);
	// dequeue(&my_queue);
	// dequeue(&my_queue);
	// printqueue(&my_queue);
	
	


	enqueue(1, &my_queue, 3);
	printqueue(&my_queue);
	// remove(1,&my_queue);
	// printqueue(&my_queue);

	// enqueue(1, &my_queue);
	// enqueue(2, &my_queue);
	// printqueue(&my_queue);

	// remove(1,&my_queue);
	// printqueue(&my_queue);

	// enqueue(1, &my_queue);
	// enqueue(3, &my_queue);
	// printqueue(&my_queue);

	// remove(2,&my_queue);
	// printqueue(&my_queue);

	//struct qentry *returned_qentry = getbypid(4, &my_queue);
	//printf("\nreturned qentry pid : %p\n", returned_qentry);

	
	
	// dequeue(&my_queue);
	
	// enqueue(2, &my_queue);
	// //enqueue(3, &my_queue);
	// printqueue(&my_queue);
	// dequeue(&my_queue);
	// dequeue(&my_queue);
	// dequeue(&my_queue);

	
	// //printqueue(&my_queue);

	// //printf("\nhead : %d\n", my_queue.head->process_id);
	// // call printqueue()
	// //printqueue(&my_queue);
	
	// dequeue(&my_queue);
	// printqueue(&my_queue);
	// dequeue(&my_queue);
	// printqueue(&my_queue);
	// dequeue(&my_queue);
	// printqueue(&my_queue);
	//dequeue(&my_queue);
	//dequeue(&my_queue);
	//printf("\nhead : %d\n", my_queue.head->process_id);


	//qentry.process_id = 1;
	
	//struct queue = {.};
	
	return 0;
}
*/
