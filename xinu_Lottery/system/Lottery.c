#include<xinu.h>

void set_tickets(pid32 pid, uint32 tickets)
{
	struct procent *prptr;
	if(isbadpid(pid))
		return;
	
	prptr = &proctab[pid];
	prptr->tickets = tickets;
	
	
} 
