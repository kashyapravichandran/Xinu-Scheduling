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
	/* Decrement the ms counter, and see if a second has passed */

	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;
		//ctr1000++;
		/* Reset the local ms counter for the next second */

		count1000 = 1000;
	}
	// Place to add turnaround time and runtime 
	proctab[currpid].runtime = proctab[currpid].runtime + 1;	
	for(i = 0;i < NPROC;i = i+1)
	{
		if(proctab[i].prstate != PR_FREE)
			proctab[i].turnaround = proctab[i].turnaround + 1;
	}

	// for burst execution 

	if(proctab[currpid].prstate == PR_CURR && proctab[currpid].flag_user == 1)
		proctab[currpid].burst_duration = proctab[currpid].burst_duration + 1;

	ctr1000++;	
	

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
		resched();
	}
}
