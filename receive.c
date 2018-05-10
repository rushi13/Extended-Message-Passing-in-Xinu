/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */

umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	kprintf("\nReceiver ID: %d\t",currpid);
	restore(mask);
	return msg;
}

umsg32	receiveMsg(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/
	int i;
	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->msg_count == 0) {
//		kprintf("\nBUFFER EMPTY");
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->msg_buf[prptr->head];		/* Retrieve message		*/
	prptr->head++;
	prptr->msg_count--;	
	if (prptr->head == 10)
		prptr->head = 0;
//	kprintf("\nReceiver%d has received the following Message :%d from sender",currpid,msg);
	
    	/* Following was added for lab3 */
	//unblock_a_sender();
	restore(mask);
	return msg;
}


syscall receiveMsgs(umsg32* msgs,      uint32 msg_count)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/
//	umsg32 msgss[10];
	int i;

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->msg_count == 0) {
//		kprintf("BUFFER EMPTY");
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	for(i=0;i<msg_count;i++)
	{
		if (prptr->head == 10)
			prptr->head = 0;		
		msgs[i] = prptr->msg_buf[prptr->head];		/* Retrieve message		*/
		prptr->head++;
		prptr->msg_count--;	
//		kprintf("\nReceiver%d has received the following Message :%d from sender",currpid,msgs[i]);
	}	//prptr->prhasmsg = FALSE;	/* Reset message flag		*/
//	kprintf("\nTotal number of messages received: %d",i);
	restore(mask);
	return i;
}

