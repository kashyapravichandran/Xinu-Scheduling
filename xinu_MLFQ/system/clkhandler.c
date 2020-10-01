/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/
	int i;
	uint32 process_count = 0;
	/* Decrement the ms counter, and see if a second has passed */

	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;
		//ctr1000++;
		/* Reset the local ms counter for the next second */

		count1000 = 1000;
	}
	// Place to add turnaround time and runtime 

	if(proctab[currpid].prstate == PR_CURR)
		proctab[currpid].runtime++;	
	
	for(i = 0;i < NPROC;i = i+1)
	{
		if(proctab[i].prstate != PR_FREE)		
			proctab[i].turnaround++;
						
	}

	// for burst execution 

	if(proctab[currpid].prstate == PR_CURR && proctab[currpid].flag_user)
	{
		proctab[currpid].burst_duration++;
		proctab[currpid].time_alloted++;
	}

	ctr1000++;	
	

	boost_period = boost_period + 1;
	
	if(preempt%QUANTUM == 0)
		timeslice++;
	

	/* Handle sleeping processes if any exist */

	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */
		
	
	if((--preempt) <= 0) {
		preempt = QUANTUM;
		//kprintf("\n Timeslice : %d \n",timeslice);
		resched();
	}
}
