/* resched.c - resched, resched_cntl */

#include <xinu.h>
#include <stdlib.h>

//#define DEBUG_CTXSW 0

struct	defer	Defer;


/* function for lottery sheduling */ 

uint32 total()
{

	int16 tail,head;
	uint32 total_num = 0;
	head = firstid(readylist_user);
	tail = queuetail(readylist_user);
	while(head !=tail)
	{
		total_num = total_num + 1;
		head = queuetab[head].qnext;	
	}
	return total_num;
}

pid32 first_element()
{
	int16 head = firstid(readylist_user);
	if(proctab[head].tickets > 0)
		return head;
	return 0;
}

uint32 total_tickets()
{
	int16	tail;			
	int16	head;
	uint32 ticket_count = 0;	
	head = firstid(readylist_user);
	tail = queuetail(readylist_user);
	//kprintf("\n here 12342323");
	while(head!=tail)
	{
		ticket_count = queuetab[head].qkey + ticket_count;
		head = queuetab[head].qnext;
	}
	//kprintf("\n ticket count %d\n",ticket_count); 
	return ticket_count;
	
}

pid32 lottery ()
{
	uint32 counter, ticket_count = 0, ticket;
	int16 tail,head;	
	ticket_count = total_tickets();	
	//kprintf("ticket count %d ",ticket_count);
	ticket = rand() % ticket_count;
    	head = firstid(readylist_user);
	tail = queuetail(readylist_user);
	counter = 0;
	//kprintf("\n here 12342323");
	while(head!=tail)
	{
		counter = counter + queuetab[head].qkey;
		if(counter > ticket && queuetab[head].qkey!=0)
			return head;
		head = queuetab[head].qnext;
	}
	
	return 0;
}


/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/
	uint32 old_pid, new_pid;
	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];
	old_pid = currpid; // added for CT4
	
	if(currpid == 0) // Null process
	{
		if(nonempty(readylist))
		{
			ptold->prstate = PR_READY;			
			insert(currpid,readylist, ptold->prprio);
			currpid = dequeue(readylist);
		}
		else if(nonempty(readylist_user))
		{
			ptold->prstate = PR_READY;
			insert(currpid,readylist, ptold->prprio);
			if(total()==1)
					currpid = first_element();
				else				
					currpid = lottery();
			if(currpid != 0)
				dequeue_user(currpid);
			else
				dequeue(readylist);
		}
		else 
		{
			return;
		}
	}
	else if(ptold->flag_user == 0) // System process other than null process 
	{
		//kprintf("here0111\n");		
		if(ptold->prstate == PR_CURR)
		{
			if(ptold->prprio > firstkey(readylist))
			{
				return;
			}
			ptold->prstate = PR_READY;
			insert(currpid, readylist, ptold->prprio);
			currpid = dequeue(readylist);
		}
		else if(firstid(readylist)!=0) // some system process
		{
			currpid = dequeue(readylist);
		}
		else // its null process
		{
			//kprintf("here1234");			
			if(nonempty(readylist_user))	
			{	
				if(total()==1)
					currpid = first_element();
				else				
					currpid = lottery();
				if(currpid != 0)
					dequeue_user(currpid);
				else
					dequeue(readylist);
			}			
			else 
				currpid = dequeue(readylist);
			//kprintf("%d",currpid);	
		}

	}
	else // User process
	{
		//kprintf("here22222\n");
		if(ptold->prstate == PR_CURR)
		{	
			ptold->prstate = PR_READY;
			insert_user(currpid,readylist_user,ptold->tickets);
		}	
		if(firstid(readylist)!=0)
		{
			currpid = dequeue(readylist);
		}
		else
		{
			if(nonempty(readylist_user))
			{	
				if(total()==1) // condition to make sure that a process with zero ticket is not executed. 
					currpid = first_element();
				else				
					currpid = lottery();
				if(currpid != 0)
					dequeue_user(currpid);
				else
					dequeue(readylist);
			}			
			else 
				currpid = dequeue(readylist);
		}
	}


	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	new_pid = currpid;
	
	// Added to keep track of runtime 
	//ptold->runtime = ptold->runtime + ctr1000-ptold->runtime_start;
	//ptnew->runtime_start = ctr1000;
	
	// Context switching to the new function 

	#ifdef DEBUG_CTXSW
		if(old_pid != new_pid)
			kprintf("\n ctxsw:: %d - %d",old_pid,new_pid);
	#endif 	
	if(old_pid != new_pid)	
	{
		ptnew->num_ctxsw = ptnew->num_ctxsw + 1;
		ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
	}
	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
