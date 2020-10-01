/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/
qid16	readylist_user_high;
qid16	readylist_user_med;
qid16	readylist_user_low;

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */

status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr, *prptr1;
	uint32 start_low, start_high, stop_low, stop_high;
	uint64 start,stop;	
	prptr1 = &proctab[currpid];
	prptr1->ready_c++;
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t":"=r" (start_high),"=r" (start_low)::"%rax","%rbx","%rcx","%rdx");

	if (isbadpid(pid)) {
		
		asm volatile ("RDTSCP\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t""CPUID\n\t":"=r" (stop_high),"=r" (stop_low)::"%rax","%rbx","%rcx","%rdx");
	
		start=((uint64)(start_high)<<32)|start_low;
		stop=((uint64)(stop_high)<<32)|stop_low;
		prptr1->ready_cycle=prptr1->ready_cycle+stop-start;
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	if(prptr->flag_user)
	{
		//if(prptr->tickets>0)
		user_process = user_process + 1;		
		insert_user(pid);
		//kprintf("here1");
		//print_ready_list();
	}	
	else 
		insert(pid, readylist, prptr->prprio);
	async_sync = 1;
	resched();
	
	asm volatile ("RDTSCP\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t""CPUID\n\t":"=r" (stop_high),"=r" (stop_low)::"%rax","%rbx","%rcx","%rdx");
	
	start=((uint64)(start_high)<<32)|start_low;
	stop=((uint64)(stop_high)<<32)|stop_low;
	prptr1->ready_cycle=prptr1->ready_cycle+stop-start;

	return OK;
}


// Stuff added for the second project

syscall print_ready_list()
{
	int16	tail;			
	int16	head;	
	if (!isbadqid(readylist)) {
		head = firstid(readylist);
		tail = queuetail(readylist);
		printf("\n System Call\n");	
		printf("\n%3s %13s\n","pid","Process Name");
		printf("%3s %13s\n","---","------------");

		while(head!=tail)
		{
			// Print stuff here 
			kprintf("%3d %-16s \n",head,proctab[head].prname);

			// Update the head pointer 
			head=queuetab[head].qnext;	
		}

		printf("\n -------------------------------- ");
	}

	printf("\n\n User Function \n");
		printf("\n%3s %13s\n","pid","Process Name");
		printf("%3s %13s\n","---","------------");

	if(!isbadqid(readylist_user_high)){
		
		head = firstid(readylist_user_high);
		tail = queuetail(readylist_user_high);
		while(head!=tail)
		{
			// Print stuff here 
			kprintf("%3d %-16s \n",head,proctab[head].prname);

			// Update the head pointer 
			head=queuetab[head].qnext;	
		}
	}
	if(!isbadqid(readylist_user_med)){
		
		head = firstid(readylist_user_med);
		tail = queuetail(readylist_user_med);
		while(head!=tail)
		{
			// Print stuff here 
			kprintf("%3d %-16s \n",head,proctab[head].prname);

			// Update the head pointer 
			head=queuetab[head].qnext;	
		}
	}
	if(!isbadqid(readylist_user_low)){
		
		head = firstid(readylist_user_low);
		tail = queuetail(readylist_user_low);
		while(head!=tail)
		{
			// Print stuff here 
			kprintf("%3d %-16s \n",head,proctab[head].prname);

			// Update the head pointer 
			head=queuetab[head].qnext;	
		}
	}
	return OK;
}
