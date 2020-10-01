/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/
qid16	readylist_user;

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
		insert_user(pid, readylist_user, prptr->tickets);
		//kprintf("here1");
		//print_ready_list();
	}	
	else 
		insert(pid, readylist, prptr->prprio);
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
	if(!isbadqid(readylist_user)){
		printf("\n\n User Function \n");
		printf("\n%3s %13s\n","pid","Process Name");
		printf("%3s %13s\n","---","------------");
		head = firstid(readylist_user);
		tail = queuetail(readylist_user);
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
