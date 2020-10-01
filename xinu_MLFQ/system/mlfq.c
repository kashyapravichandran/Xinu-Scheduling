#include<xinu.h>
// Macros and global variables

uint32 boost_period = 0;

// Function to add the process back into the list 

void insert_mlfq(pid32 pid)
{
	struct procent *prptr = &proctab[pid];
	//if(prptr->prstate == PR_CURR)
	
		if(prptr->time_alloted >= TIME_ALLOTMENT)
		{
			// increase the user_flag saturate at 3
			// re enter time alloted 
			// insert the function into the list now
			
			prptr->flag_user = prptr->flag_user + 1;
			if(prptr->flag_user > 3)
				prptr->flag_user = 3;
			//kprintf("\nHere-downgrade %d ", prptr->flag_user);
			prptr->time_alloted =0;
		}
		insert_user(pid);	
	

}

// Function for priority boost

void priority_boost()
{
	
	
	pid32 curr;
	int i;	
	boost_period = 0;	
	//kprintf("\nctr100 : %d %d", ctr1000, boost_period);
	// from user list med and low and sleep 
	while(nonempty(readylist_user_med))
	{
		curr = dequeue(readylist_user_med);
		enqueue(curr,readylist_user_high);
	}	
	while(nonempty(readylist_user_low))
	{
		curr = dequeue(readylist_user_low);
		enqueue(curr,readylist_user_high);
	}

	
	
	for(i = 0;i < NPROC; i = i + 1)
	{
		if(proctab[i].flag_user && proctab[i].prstate!=PR_FREE)
		{
			proctab[i].flag_user = 1;
			proctab[i].time_alloted = 0;
		}
	}
}
