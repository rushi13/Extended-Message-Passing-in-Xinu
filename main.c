/*  main.c  - main */

#include <xinu.h>
#define MAXSIZE 10
#define MAX_PROC 3

pid32 sender_id[MAX_PROC];
pid32 receiver_id[MAX_PROC];
uint32 pid_count = 3;
pid32 pids[3];
uint32 msg_count = 3;
sid32 print_lock;

process sender(void)
{
	umsg32 msgs[12]={1,2,3,4,5,6,7,8,9,10,11,12};
	int sendmsgs;	
	int i,ret,msgcount=0;
	uint32 msg=rand();
	sendnMsg(pid_count,receiver_id,msg);
	for(i=0;i<MAX_PROC;i++)
	{
		wait(print_lock);
//		msg = receiveMsg();
		kprintf("\nSender%d has sent the following Message :%d to receiver%d",currpid,msg,receiver_id[i]);
		signal(print_lock);
		msgcount++;
	}
	wait(print_lock);
	kprintf("\nTotal number of messages sent by sender%d: %d",currpid,msgcount);
	signal(print_lock);
/*	sendmsgs = sendMsgs(receiver_id[0],msgs,msg_count);
	kprintf("\n%d messages were successfully sent to receiver%d",sendmsgs,receiver_id[0]);
	for(i=0;i<sendmsgs;i++)
	{
		wait(print_lock);
		kprintf("\nSender%d has sent the following message: %d to Receiver%d",sender_id[0],msgs[i],receiver_id[0]);
		signal(print_lock);
	}
	ret = sendMsg(receiver_id[0],rand());
	if(ret == SYSERR)
		kprintf("Buffer full");
	else
		kprintf("\nSender%d has sent the following message: %d to Receiver%d",sender_id[0],ret,receiver_id[0]);
*/
	return OK;
}


process receiver(void)
{
	umsg32 msgs[10];
	int32 msg = 500;
	int i,msgcount=0;

//	msg = receiveMsg();
//		kprintf("\nReceiver%d has received the following Message :%d from sender",currpid,msg);
//	msg = receiveMsgs(msgs,3);
	for(i=0;i<3;i++)
	{
		wait(print_lock);
		msg = receiveMsg();
		kprintf("\nReceiver%d has received the following Message :%d from sender",currpid,msg);
		signal(print_lock);
		msgcount++;
	}
	wait(print_lock);
	kprintf("\nTotal number of messages received by receiver%d: %d",currpid,msgcount);
	signal(print_lock);
	return OK;
}

process	main(void)
{
	int i;
	print_lock = semcreate(1);
/*	for(i=0;i<pid_count;i++)
	{
		pids[i] = create(receiver, 4096, 50, "receiver", 0);
		kprintf("\ncreated receiver: %d",pids[i]);
	}	
	sender_id[0] = create(sender, 4096, 50, "sender", 0);
	kprintf("\n\ncreated sender: %d\n",sender_id[0]);
	receiver_id[0] = create(receiver, 4096, 50, "receiver", 0);
	kprintf("\n\ncreated receiver: %d\n",receiver_id[0]);
	kprintf("\nresumed sender: %d\n",sender_id[0]);	
	resume(sender_id[0]);
	kprintf("\nresumed receiver: %d\n",receiver_id[0]);
	resume(receiver_id[0]);
	for(i=0;i<pid_count;i++)
	{
		kprintf("\n\nresumed receiver: %d",pids[i]);
		resume(pids[i]);
	}
*/
	for ( i=0; i < MAX_PROC ; i++ ) {
		sender_id[i]   = create(sender, 4096, 50, "sender", 1,i); 
		receiver_id[i] = create(receiver, 4096, 50, "receiver", 1,i); 
	}
	
	resched_cntl(DEFER_START);
	
	for ( i=0; i < MAX_PROC ; i++ ) { 
		resume(sender_id[i]); 
		resume(receiver_id[i]);	
//		sleep(100);
	}
	
//	resume(sender_id[0]);
	resched_cntl(DEFER_STOP);

	return OK;
}
