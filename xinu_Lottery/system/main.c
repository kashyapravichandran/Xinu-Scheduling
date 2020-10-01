#include <xinu.h>
#include <stdio.h>

void timed_execution(uint32 runtime){
	    while(proctab[currpid].runtime<runtime);
}

int main() {
	pid32 prA, prB, prC, prD;

	kprintf("\n Testcases for fairness analysis");

	kprintf("\n");
		kprintf("=== TESTCASE 1::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 1);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 1);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);
	
	kprintf("=== TESTCASE 2::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 10);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 10);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);
		
	kprintf("=== TESTCASE 3::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 100);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 100);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);
	
	kprintf("=== TESTCASE 4::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 500);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 500);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);

	kprintf("=== TESTCASE 5::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 1000);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 1000);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);

	kprintf("=== TESTCASE 6::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 2000);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 2000);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);

	kprintf("=== TESTCASE 7::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 5000);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 5000);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);

	kprintf("=== TESTCASE 8::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 7000);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 7000);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);
	
	kprintf("=== TESTCASE 9::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 9000);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 9000);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);


	kprintf("=== TESTCASE 10::  CPU-intensive jobs =============================\n");

	prA = create_user_process(timed_execution, 1024, "timed_execution", 1, 100000);
	prB = create_user_process(timed_execution, 1024, "timed_execution", 1, 100000);
	set_tickets(prA, 50);
	set_tickets(prB, 50);
	resume(prA);
	resume(prB);
	receive();	
	receive();	
	
	sleepms(50); // wait for user processes to terminate	

	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prA, proctab[prA].runtime, proctab[prA].turnaroundtime, proctab[prA].num_ctxsw);
	kprintf("process %d:: runtime=%d, turnaround time=%d, ctx=%d\n",prB, proctab[prB].runtime, proctab[prB].turnaroundtime, proctab[prB].num_ctxsw);

	return OK;
}
