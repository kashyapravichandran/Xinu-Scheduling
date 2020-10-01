/* insert.c - insert */

#include <xinu.h>


uint32 user_process = 0;
/*------------------------------------------------------------------------
 *  insert  -  Insert a process into a queue in descending key order
 *------------------------------------------------------------------------
 */
status	insert(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* Key for the inserted process	*/
	)
{
	int16	curr;			/* Runs through items in a queue*/
	int16	prev;			/* Holds previous node index	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	curr = firstid(q);
	while (queuetab[curr].qkey >= key) {
		curr = queuetab[curr].qnext;
	}

	/* Insert process between curr node and previous node */

	prev = queuetab[curr].qprev;	/* Get index of previous node	*/
	queuetab[pid].qnext = curr;
	queuetab[pid].qprev = prev;
	queuetab[pid].qkey = key;
	queuetab[prev].qnext = pid;
	queuetab[curr].qprev = pid;
	return OK;
}

status insert_user(pid32 pid)
{
	int16	curr;			/* Runs through items in a queue*/
	int16	prev;			/* Holds previous node index	*/

	if (isbadpid(pid)) {
		return SYSERR;
	}

	//user_process = user_process + 1;
	// flag user denotes priority level here. 0 is for a system process

	if(proctab[pid].flag_user==1)
	{
		enqueue(pid,readylist_user_high);
	}
	else if(proctab[pid].flag_user==2)
	{
		enqueue(pid,readylist_user_med);
	}
	else if(proctab[pid].flag_user==3)
	{
		enqueue(pid,readylist_user_low);
	}
	
	return OK;
}
