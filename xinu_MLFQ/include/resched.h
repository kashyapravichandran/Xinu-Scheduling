/* resched.h */

/* Constants and variables related to deferred rescheduling */

#define	DEFER_START	1	/* Start deferred rescehduling		*/
#define	DEFER_STOP	2	/* Stop  deferred rescehduling		*/

/* Structure that collects items related to deferred rescheduling	*/

// Place to declare TA and Boost Perios - TODO

#define TIME_ALLOTMENT 15

#define PRIORITY_BOOST_PERIOD 100

struct	defer	{
	int32	ndefers;	/* Number of outstanding defers 	*/
	bool8	attempt;	/* Was resched called during the	*/
				/*   deferral period?			*/
};

extern	struct	defer	Defer;

// Stuff added for MLFQ 

extern uint32 boost_period;

extern uint32 async_sync;

extern uint32 timeslice;
