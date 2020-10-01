#include <xinu.h> 

void burst_execution(uint32 number_bursts, uint32 burst_duration, uint32 sleep_duration)
{
	//uint32 start_time = ctr1000;
	struct	procent *prptr = &proctab[currpid];
	int i = 0;	
	for(i=0;i<number_bursts;i++)
	{	
		prptr->burst_duration = 0;
		while(prptr->burst_duration < burst_duration)
			;
		//kprintf("Reached here");
		sleepms(sleep_duration);
		//kprintf("R");
		//kprintf("Reached Here");
	}
	
}
