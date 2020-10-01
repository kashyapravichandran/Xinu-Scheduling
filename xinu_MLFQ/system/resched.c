/* resched.c - resched, resched_cntl */

#include <xinu.h>
#include <stdlib.h>

//#define DEBUG_CTXSW 0

struct	defer	Defer;

uint32 async_sync = 0;

uint32 timeslice = 0;

// MLFQ 

pid32 mlfq_schedule(uint32 slice)
{
	//pid32 return_value;
	//kprintf("Time Slice Value: %d",slice);
	if(nonempty(readylist_user_high))
	{
		return dequeue(readylist_user_high);
	}
	else if(nonempty(readylist_user_med))
	{
		return dequeue(readylist_user_med);
	}
	else if(nonempty(readylist_user_low))
	{
		return dequeue(readylist_user_low);
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
	uint32 old_pid, new_pid; uint32 slice;
	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];
	old_pid = currpid; // added for CT4
	
	// Might have to call priority boost here
	//kprintf("boost val:%d",boost_period); 
	if(boost_period >= PRIORITY_BOOST_PERIOD )
	{
		priority_boost();
		//timeslice = 0;	
	}
	
	if(async_sync==1)
		timeslice = 0;
	async_sync = 0;
	slice = timeslice;
	if(timeslice == 3)
	{
		timeslice = 0;
	}
	if(currpid == 0) // Null process
	{
		if(nonempty(readylist))
		{
			ptold->prstate = PR_READY;			
			insert(currpid,readylist, ptold->prprio);
			currpid = dequeue(readylist);
		}
		else 
		{
			ptold->prstate = PR_READY;
			insert(currpid,readylist, ptold->prprio);
			currpid=mlfq_schedule(slice);
			if(currpid == 0)
				dequeue(readylist);
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
			currpid=mlfq_schedule(slice);
			if(currpid == 0)
				dequeue(readylist);
				
		}

	}
	else // User process
	{
		//kprintf("here22222\n");
		
		
		if(ptold->prstate == PR_CURR)
		{	
			ptold->prstate = PR_READY;
			insert_mlfq(currpid);
			//kprintf("Here");
		}
		if(firstid(readylist)!=0)
		{
			currpid = dequeue(readylist);
		}
		else
		{
			currpid = mlfq_schedule(slice);			
			if(currpid == 0)
				dequeue(readylist);
						
			
		}
	}


	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;	
	switch(ptnew->flag_user)		/* Reset time slice for process */
	{
		case 0: preempt = QUANTUM;break;
		case 1: preempt = QUANTUM;break;
		case 2: preempt = 2*QUANTUM; break;
		case 3: preempt	= 4*QUANTUM;break;
		default: preempt = QUANTUM; 
	} 
	
	//preempt = QUANTUM;

	new_pid = currpid;
	
	// Added to keep track of runtime 
	//ptold->runtime = ptold->runtime + ctr1000-ptold->runtime_start;
	//ptnew->runtime_start = ctr1000;
	
	// Context switching to the new function 

	#ifdef DEBUG_CTXSW
		if(old_pid != new_pid)
			kprintf("\n ctxsw:: %d - %d %d",old_pid,new_pid,ctr1000);
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
			async_sync = 1;
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
