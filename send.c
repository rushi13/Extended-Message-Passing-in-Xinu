/* send.c - send */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */

syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

syscall	sendMsg(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if(prptr->msg_count == 10)
	{
//		kprintf("Buffer full");
		restore(mask);	
		return SYSERR;
	}
	else
	{
		if (prptr->tail == 10)
			prptr->tail = 0;
		prptr->msg_buf[prptr->tail] = msg;
		prptr->tail++;
		prptr->msg_count++;
//		kprintf("\nSender%d has sent the following message: %d to Receiver%d",currpid,msg,pid);
	}
	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return msg;
}

uint32	sendMsgs(pid32 pid,      umsg32* msgs,      uint32 msg_count)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	int i;

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	for(i=0;i<msg_count;i++)
	{		
		if(prptr->msg_count == 10)
		{
//			kprintf("Buffer full");
			restore(mask);	
			return i;
		}
		else{
			if (prptr->tail == 10)
				prptr->tail = 0;
			prptr->msg_buf[prptr->tail] = msgs[i];
			prptr->tail++;
			prptr->msg_count++;
//			kprintf("\nSender%d has sent the following message: %d to Receiver%d",currpid,msgs[i],pid);
		}
	}	
	/* If recipient waiting or in timed-wait make it ready */
	
	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return i;
}

uint32 sendnMsg(uint32 pid_count,      pid32* pids,      umsg32 msg )
{
	int i;
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	for(i=0;i<pid_count;i++)
	{
		if (isbadpid(pids[i])) {
			restore(mask);
			return SYSERR;
		}
		
		prptr = &proctab[pids[i]];
		if(prptr->msg_count == 10)
		{
//			kprintf("Buffer full");
			restore(mask);	
			return SYSERR;
		}
		else{
			if (prptr->tail == 10)
				prptr->tail = 0;
			prptr->msg_buf[prptr->tail] = msg;
			prptr->tail++;
			prptr->msg_count++;
//			kprintf("\nSender%d has sent the following message: %d to Receiver%d",currpid,msg,pids[i]);
		}
		/* If recipient waiting or in timed-wait make it ready */

		if (prptr->prstate == PR_RECV) {
			ready(pids[i]);
		} else if (prptr->prstate == PR_RECTIM) {
			unsleep(pids[i]);
			ready(pids[i]);
		}
	}
	restore(mask);		/* Restore interrupts */
	return OK;

}


