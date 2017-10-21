#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

#define BIDIRECTIONAL 0    /* change to 1 if you're doing extra credit */
                           /* and write a routine called B_output */

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */
struct msg {
  char data[20];
};

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
struct pkt {
   int seqnum;
   int acknum;
   int checksum;
   char payload[20];
};

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#define TRUE 1
#define FALSE 0
#define NACK -1
#define MAX_SEQ_NUM 16
#define WINDOW_SIZE 8
#define TIMER_INTERVAL 50.0
#define MAX_BUFFER_SIZE 50

/* global variables */
int curSeqNum;
int prev_acked_seq_num;

int curAckNum;
int ack_packet_seqnum;

/* Save previous ACK packet for retransmission*/
struct pkt last_ack_pkt;

/* Linkedlist to store in-flight pkt in the window */
typedef struct pkt_node {
	struct pkt packet;
	struct pkt_node* next;
} pkt_node;

// Variables to save the state of the linkedlist
pkt_node* base;
pkt_node* end;
int list_size;

/* Linkedlist to store out-of-range msg from layer 5*/
typedef struct msg_node {
	struct msg message;
	struct msg_node* next;
} msg_node;

// Variables to save the state of the linkedlist
msg_node* buffer_base;
msg_node* buffer_end;
int buffer_size;

// Calculate the checksum field
checksum_calc(packet)
  struct pkt *packet;
{
	int i;
	int seqnum;
	int acknum;
	char data[20];
	int hex_data[14];
	unsigned int sum = 0;
	unsigned int carry = 0;

	for(i = 0; i < 20; i++) {
		data[i] = packet->payload[i];
	}

	// Divide the payload into 16 bits each
	for(i = 0; i < 20; i += 2) {
		hex_data[i / 2] = (data[i] << 8 | data[i + 1]);
	}

	// Divide seqnum into 16 bits each
	seqnum = packet->seqnum;
	hex_data[10] = seqnum >> 16;
	hex_data[11] = seqnum | 0x0000FFFF;

	// Divide acknum into 16 bits each
	acknum = packet->acknum;
	hex_data[12] = acknum >> 16;
	hex_data[13] = acknum | 0x0000FFFF;

	// Sum up each of the 16 bits
	for(i = 0; i < 14; i++) {
		sum += hex_data[i];
	}

	// Fold the results into 16 bits by adding the carry to the result 
	carry = sum >> 16;
	while(carry != 0) {
		sum &= 0x0FFFF;
		sum += carry;
		carry = sum >> 16;
	}

	// Flip the last 16 bits to get the final checksum
	packet->checksum = (~sum & 0x0000FFFF);
}

// Check if the received packet is corrupted through checksum
int checksum_validate(packet)
  struct pkt *packet;
{
	int i;
	int seqnum;
	int acknum;
	int checksum;
	char data[20];
	int hex_data[14];
	unsigned int sum = 0;
	unsigned int carry = 0;

	for(i = 0; i < 20; i++) {
		data[i] = packet->payload[i];
	}

	// Divide the payload into 16 bits each
	for(i = 0; i < 20; i += 2) {
		hex_data[i / 2] = (data[i] << 8 | data[i + 1]);
	}

	// Divide seqnum into 16 bits each
	seqnum = packet->seqnum;
	hex_data[10] = seqnum >> 16;
	hex_data[11] = seqnum | 0x0000FFFF;

	// Divide acknum into 16 bits each
	acknum = packet->acknum;
	hex_data[12] = acknum >> 16;
	hex_data[13] = acknum | 0x0000FFFF;

	// Sum up each of the 16 bits
	for(i = 0; i < 14; i++) {
		sum += hex_data[i];
	}

	// Add up the checksum field
	checksum = packet->checksum;
	sum += checksum;

	// Fold the results into 16 bits by adding the carry to the result 
	carry = sum >> 16;
	while(carry != 0) {
		sum &= 0x0FFFF;
		sum += carry;
		carry = sum >> 16;
	}

	// Flip the last 16 bits to check if the packet is corrupted
	if ((~sum & 0x0000FFFF) != 0) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}


/* called from layer 5, passed the data to be sent to other side */
A_output(message)
  struct msg message;
{
	struct pkt packet;

	// Construct the packet from incoming message
	for(int i = 0; i < 20; i++) {
		packet.payload[i] = message.data[i];
	}

	packet.seqnum = curSeqNum;

	packet.acknum = 0;

	// Calculate checksum
	checksum_calc(&packet);

	if(list_size < WINDOW_SIZE) {
		printf("Info: Sending packet %d to B ...\n", packet.seqnum);

		// Update current sequence number
		curSeqNum = (curSeqNum + 1) % MAX_SEQ_NUM;

		// Insert the new packet into the window linkedlist
		if (base == NULL) {
			// Only do this when the first packet comes in
			base = (pkt_node *)malloc(sizeof(pkt_node));
			base->packet = packet;
			base->next = NULL;
			end = base;
		}
		else {
			pkt_node* new_node = (pkt_node *)malloc(sizeof(pkt_node));
			new_node->packet = packet;
			new_node->next = NULL;
			end->next = new_node;
			end = end->next;
		}

		// Update the size of the buffer list
		list_size++;
		printf("Info: Window size: %d. Current num of unacked pkts: %d\n", WINDOW_SIZE, list_size);

		// Hand over the packet
		tolayer3(0, packet);

		// Start timer when the first packet in the window comes in
		if (end == base) {
			starttimer(0, TIMER_INTERVAL);
		}
	}
	else {
		// If buffer is full, abandon incoming packets, otherwise, buffer the packets
		if (buffer_size >= MAX_BUFFER_SIZE) {
			printf("Info: Buffer full (Maximum 50), abandon packet!\n");
		}
		else {
			// Buffer incoming packet if the new packet falls outside of the window
			printf("Info: Incoming packets fall outside of window, buffer packets!\n");

			// Insert the new packet into the buffer linkedlist
			if (buffer_base == NULL) {
				// Only do this when the first packet comes in
				buffer_base = (msg_node *)malloc(sizeof(msg_node));
				buffer_base->message = message;
				buffer_base->next = NULL;
				buffer_end = buffer_base;
			}
			else {
				msg_node* new_node = (msg_node *)malloc(sizeof(msg_node));
				new_node->message = message;
				new_node->next = NULL;
				buffer_end->next = new_node;
				buffer_end = buffer_end->next;
			}
			// Update buffer size
			buffer_size++;

			printf("Info: Current buffer size: %d\n", buffer_size);
		}
	}
}

B_output(message)  /* need be completed only for extra credit */
  struct msg message;
{

}

/* called from layer 3, when a packet arrives for layer 4 */
A_input(packet)
  struct pkt packet;
{
	int acknum;
	pkt_node* cursor = base;

	// If the packet is corrupted
	if(checksum_validate(&packet) != TRUE) {
		// Abandon the packet, wait for the timer to time out
		printf("Info: Packet received by A is corrupted, abandon!\n");
	}
	else {
		acknum = packet.acknum;

		// If the received packet is a NACK
		if (acknum == -1) {
			printf("Info: Receive NACK packet from B, retransmit all packets in the window ...\n");

			// Stop the awaiting timer
			stoptimer(0);

			// Retransmit all packets in window buffer
			while (cursor != NULL) {
				tolayer3(0, cursor->packet);
				cursor = cursor->next;
			}

			// Restart the timer
			starttimer(0, TIMER_INTERVAL);
		}
		else {
			printf("Info: Receive ACK packet from B with ack num %d\n", acknum);

			// If the new ack# equals the previous one, the first packet
			// in the window is not correctly received, so there is no
			// need to look for the acked packet in the window linkedlist,
			// it has already been deleted
			if (prev_acked_seq_num != acknum) {
				cursor = base;
				while (cursor != NULL) {
					if (cursor->packet.seqnum != acknum) {
						// Delete the node since the packet has already been rcvd (cumulative ack)
						base = base->next;
						list_size--;
					}
					else {
						base = base->next;
						list_size--;
						break;
					}
					cursor = cursor->next;
				}

				printf("Info: ACK received, packets with seq num up to %d have been received\n", acknum);

				// Manage the timer if new ack is received
				if (base == NULL) {
					// Update the end ptr if the buffer list is empty
					end = base;
					// Stop the timer
					stoptimer(0);
					printf("Info: All packets sent, timer A stopped!\n");
				}
				else {
					// Restart the timer if there is still unacked pkt in the list
					stoptimer(0);
					starttimer(0, TIMER_INTERVAL);
					printf("Info: Timer A restarted for packet with seq num %d\n", base->packet.seqnum);
				}
			}
			else {
				printf("Info: Duplicate ACK #%d received, abandon ACK packet!\n", acknum);
			}

			// Save the sequence number of the packet being acked
			prev_acked_seq_num = acknum;

			// If there are packets buffered & window is not full
			// continue sending buffered packets
			while(buffer_size > 0 && list_size < WINDOW_SIZE) {
				printf("Info: Send buffered packet ...\n");
				struct msg buffered_msg = buffer_base->message;
				// Construct & send packet using the buffered message
				A_output(buffered_msg);
				// Delete the buffer msg that have been sent
				buffer_base = buffer_base->next;
				// Update buffer size
				buffer_size--;
			}
		}
	}
}

/* called when A's timer goes off */
A_timerinterrupt()
{
	pkt_node* cursor = base;

	printf("Info: Timer A times out!\n");

	// Retransmit all unacked packets
	while (cursor != NULL) {
		tolayer3(0, cursor->packet);
		cursor = cursor->next;
	}

	// Start the timer
	starttimer(0, TIMER_INTERVAL);

	printf("Info: Retransmit all packets in the window to B\n");
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
A_init()
{
	curSeqNum = 0;
	prev_acked_seq_num = -1;
	base = NULL;
	end = NULL;
	list_size = 0;
	buffer_base = NULL;
	buffer_end = NULL;
	buffer_size = 0;
}


/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
B_input(packet)
  struct pkt packet;
{
	int i;
	int seqnum;
	struct pkt ack_packet;

	// If the packet is corrupted
	if(checksum_validate(&packet) != TRUE) {
		printf("Info: Packet received by B is corrupted, respond with NACK\n");
		
		// Respond A with NACK (Use negative seq num to represent NACK)
		ack_packet.acknum = NACK;
		ack_packet.seqnum = ack_packet_seqnum++;

		for (i = 0; i < 20; i++) {
			ack_packet.payload[i] = NACK;
		}

		// Calculate checksum
		checksum_calc(&ack_packet);

		// Send the ack packet to layer 3
		tolayer3(1, ack_packet);

		printf("Info: Sending NACK packet to A ...\n");
	}
	else {
		seqnum = packet.seqnum;
		printf("Info: Receive packet from A with seq num %d\n", seqnum);
		// If the packet is the one expected
		if(curAckNum == seqnum) {
			// Pass the msg to layer 5
			tolayer5(1, packet.payload);
			printf("Info: Message sent to layer 5 (from B)\n");

			// Respond A with ACK
			ack_packet.acknum = curAckNum;
			ack_packet.seqnum = ack_packet_seqnum++;

			for (i = 0; i < 20; i++) {
				ack_packet.payload[i] = curAckNum;
			}

			// Calculate checksum
			checksum_calc(&ack_packet);

			// Send the ack packet to layer 3
			tolayer3(1, ack_packet);

			printf("Info: Acknowledge A with Ack num %d\n", curAckNum);

			// Update curAckNum
			curAckNum = (curAckNum + 1) % MAX_SEQ_NUM;

			// Save the ack packet in case retransmission is required
			last_ack_pkt = ack_packet;
		}
		else {
			// Resend ack with current rcvd seq# if the packet is not the one expected (out-of-order)
			printf("Info: Packet received has incorrect seq num, expecting #%d, acknowledge with previous ACK packet\n", curAckNum);
			last_ack_pkt.seqnum = ack_packet_seqnum++;

			tolayer3(1, last_ack_pkt);

			printf("Info: Re-acked with ack num %d\n", last_ack_pkt.acknum);
		}
	}
}

/* called when B's timer goes off */
B_timerinterrupt()
{
}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
B_init()
{
	curAckNum = 0;
	ack_packet_seqnum = 0;
}


/*****************************************************************
***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 3 and below network environment:
  - emulates the tranmission and delivery (possibly with bit-level corruption
    and packet loss) of packets across the layer 3/4 interface
  - handles the starting/stopping of a timer, and generates timer
    interrupts (resulting in calling students timer handler).
  - generates message to be sent (passed from later 5 to 4)

THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
******************************************************************/

struct event {
   float evtime;           /* event time */
   int evtype;             /* event type code */
   int eventity;           /* entity where event occurs */
   struct pkt *pktptr;     /* ptr to packet (if any) assoc w/ this event */
   struct event *prev;
   struct event *next;
 };
struct event *evlist = NULL;   /* the event list */

/* possible events: */
#define  TIMER_INTERRUPT 0  
#define  FROM_LAYER5     1
#define  FROM_LAYER3     2

#define  OFF             0
#define  ON              1
#define   A    0
#define   B    1



int TRACE = 1;             /* for my debugging */
int nsim = 0;              /* number of messages from 5 to 4 so far */ 
int nsimmax = 0;           /* number of msgs to generate, then stop */
float time = 0.000;
float lossprob;            /* probability that a packet is dropped  */
float corruptprob;         /* probability that one bit is packet is flipped */
float lambda;              /* arrival rate of messages from layer 5 */   
int   ntolayer3;           /* number sent into layer 3 */
int   nlost;               /* number lost in media */
int ncorrupt;              /* number corrupted by media*/

main()
{
   struct event *eventptr;
   struct msg  msg2give;
   struct pkt  pkt2give;
   
   int i,j;
   char c; 
  
   init();
   A_init();
   B_init();
   
   while (1) {
        eventptr = evlist;            /* get next event to simulate */
        if (eventptr==NULL)
           goto terminate;
        evlist = evlist->next;        /* remove this event from event list */
        if (evlist!=NULL)
           evlist->prev=NULL;
        if (TRACE>=2) {
           printf("\nEVENT time: %f,",eventptr->evtime);
           printf("  type: %d",eventptr->evtype);
           if (eventptr->evtype==0)
	       printf(", timerinterrupt  ");
             else if (eventptr->evtype==1)
               printf(", fromlayer5 ");
             else
	     printf(", fromlayer3 ");
           printf(" entity: %d\n",eventptr->eventity);
           }
        time = eventptr->evtime;        /* update time to next event time */
        if (nsim==nsimmax)
	  break;                        /* all done with simulation */
        if (eventptr->evtype == FROM_LAYER5 ) {
            generate_next_arrival();   /* set up future arrival */
            /* fill in msg to give with string of same letter */    
            j = nsim % 26; 
            for (i=0; i<20; i++)  
               msg2give.data[i] = 97 + j;
            if (TRACE>2) {
               printf("          MAINLOOP: data given to student: ");
                 for (i=0; i<20; i++) 
                  printf("%c", msg2give.data[i]);
               printf("\n");
	     }
            nsim++;
            if (eventptr->eventity == A) 
               A_output(msg2give);  
             else
               B_output(msg2give);  
            }
          else if (eventptr->evtype ==  FROM_LAYER3) {
            pkt2give.seqnum = eventptr->pktptr->seqnum;
            pkt2give.acknum = eventptr->pktptr->acknum;
            pkt2give.checksum = eventptr->pktptr->checksum;
            for (i=0; i<20; i++)  
                pkt2give.payload[i] = eventptr->pktptr->payload[i];
	    if (eventptr->eventity ==A)      /* deliver packet by calling */
   	       A_input(pkt2give);            /* appropriate entity */
            else
   	       B_input(pkt2give);
	    free(eventptr->pktptr);          /* free the memory for packet */
            }
          else if (eventptr->evtype ==  TIMER_INTERRUPT) {
            if (eventptr->eventity == A) 
	       A_timerinterrupt();
             else
	       B_timerinterrupt();
             }
          else  {
	     printf("INTERNAL PANIC: unknown event type \n");
             }
        free(eventptr);
        }

terminate:
   printf(" Simulator terminated at time %f\n after sending %d msgs from layer5\n",time,nsim);

   system("pause");
}



init()                         /* initialize the simulator */
{
  int i;
  float sum, avg;
  float jimsrand();
  
  
   printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");
   printf("Enter the number of messages to simulate: ");
   scanf("%d",&nsimmax);
   printf("Enter packet loss probability [enter 0.0 for no loss]:");
   scanf("%f",&lossprob);
   printf("Enter packet corruption probability [0.0 for no corruption]:");
   scanf("%f",&corruptprob);
   printf("Enter average time between messages from sender's layer5 [ > 0.0]:");
   scanf("%f",&lambda);
   printf("Enter TRACE:");
   scanf("%d",&TRACE);

   srand(9999);              /* init random number generator */
   sum = 0.0;                /* test random number generator for students */
   for (i=0; i<1000; i++)
      sum=sum+jimsrand();    /* jimsrand() should be uniform in [0,1] */
   avg = sum/1000.0;
   if (avg < 0.25 || avg > 0.75) {
    printf("It is likely that random number generation on your machine\n" ); 
    printf("is different from what this emulator expects.  Please take\n");
    printf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
    exit(0);
    }

   ntolayer3 = 0;
   nlost = 0;
   ncorrupt = 0;

   time=0.0;                    /* initialize time to 0.0 */
   generate_next_arrival();     /* initialize event list */
}

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand() 
{
  double mmm = 32767;   /* largest int  - MACHINE DEPENDENT!!!!!!!!   */
  //double mmm = 2147483647;
  float x;                   /* individual students may need to change mmm */ 
  x = (float)(rand()/mmm);            /* x should be uniform in [0,1] */
  return(x);
}  

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/
 
generate_next_arrival()
{
   double x,log(),ceil();
   struct event *evptr;
    char *malloc();
   float ttime;
   int tempint;

   if (TRACE>2)
       printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");
 
   x = lambda*jimsrand()*2;  /* x is uniform on [0,2*lambda] */
                             /* having mean of lambda        */
   evptr = (struct event *)malloc(sizeof(struct event));
   evptr->evtime =  time + x;
   evptr->evtype =  FROM_LAYER5;
   if (BIDIRECTIONAL && (jimsrand()>0.5) )
      evptr->eventity = B;
    else
      evptr->eventity = A;
   insertevent(evptr);
} 


insertevent(p)
   struct event *p;
{
   struct event *q,*qold;

   if (TRACE>2) {
      printf("            INSERTEVENT: time is %lf\n",time);
      printf("            INSERTEVENT: future time will be %lf\n",p->evtime); 
      }
   q = evlist;     /* q points to header of list in which p struct inserted */
   if (q==NULL) {   /* list is empty */
        evlist=p;
        p->next=NULL;
        p->prev=NULL;
        }
     else {
        for (qold = q; q !=NULL && p->evtime > q->evtime; q=q->next)
              qold=q; 
        if (q==NULL) {   /* end of list */
             qold->next = p;
             p->prev = qold;
             p->next = NULL;
             }
           else if (q==evlist) { /* front of list */
             p->next=evlist;
             p->prev=NULL;
             p->next->prev=p;
             evlist = p;
             }
           else {     /* middle of list */
             p->next=q;
             p->prev=q->prev;
             q->prev->next=p;
             q->prev=p;
             }
         }
}

printevlist()
{
  struct event *q;
  int i;
  printf("--------------\nEvent List Follows:\n");
  for(q = evlist; q!=NULL; q=q->next) {
    printf("Event time: %f, type: %d entity: %d\n",q->evtime,q->evtype,q->eventity);
    }
  printf("--------------\n");
}



/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
stoptimer(AorB)
int AorB;  /* A or B is trying to stop timer */
{
 struct event *q,*qold;

 if (TRACE>2)
    printf("          STOP TIMER: stopping timer at %f\n",time);
/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
 for (q=evlist; q!=NULL ; q = q->next) 
    if ( (q->evtype==TIMER_INTERRUPT  && q->eventity==AorB) ) { 
       /* remove this event */
       if (q->next==NULL && q->prev==NULL)
             evlist=NULL;         /* remove first and only event on list */
          else if (q->next==NULL) /* end of list - there is one in front */
             q->prev->next = NULL;
          else if (q==evlist) { /* front of list - there must be event after */
             q->next->prev=NULL;
             evlist = q->next;
             }
           else {     /* middle of list */
             q->next->prev = q->prev;
             q->prev->next =  q->next;
             }
       free(q);
       return;
     }
  printf("Warning: unable to cancel your timer. It wasn't running.\n");
}


starttimer(AorB,increment)
int AorB;  /* A or B is trying to stop timer */
float increment;
{

 struct event *q;
 struct event *evptr;
 char *malloc();

 if (TRACE>2)
    printf("          START TIMER: starting timer at %f\n",time);
 /* be nice: check to see if timer is already started, if so, then  warn */
/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
   for (q=evlist; q!=NULL ; q = q->next)  
    if ( (q->evtype==TIMER_INTERRUPT  && q->eventity==AorB) ) { 
      printf("Warning: attempt to start a timer that is already started\n");
      return;
      }
 
/* create future event for when timer goes off */
   evptr = (struct event *)malloc(sizeof(struct event));
   evptr->evtime =  time + increment;
   evptr->evtype =  TIMER_INTERRUPT;
   evptr->eventity = AorB;
   insertevent(evptr);
} 


/************************** TOLAYER3 ***************/
tolayer3(AorB,packet)
int AorB;  /* A or B is trying to stop timer */
struct pkt packet;
{
 struct pkt *mypktptr;
 struct event *evptr,*q;
 char *malloc();
 float lastime, x, jimsrand();
 int i;


 ntolayer3++;

 /* simulate losses: */
 if (jimsrand() < lossprob)  {
      nlost++;
      if (TRACE>0)    
	printf("          TOLAYER3: packet being lost\n");
      return;
    }  

/* make a copy of the packet student just gave me since he/she may decide */
/* to do something with the packet after we return back to him/her */ 
 mypktptr = (struct pkt *)malloc(sizeof(struct pkt));
 mypktptr->seqnum = packet.seqnum;
 mypktptr->acknum = packet.acknum;
 mypktptr->checksum = packet.checksum;
 for (i=0; i<20; i++)
    mypktptr->payload[i] = packet.payload[i];
 if (TRACE>2)  {
   printf("          TOLAYER3: seq: %d, ack %d, check: %d ", mypktptr->seqnum,
	  mypktptr->acknum,  mypktptr->checksum);
    for (i=0; i<20; i++)
        printf("%c",mypktptr->payload[i]);
    printf("\n");
   }

/* create future event for arrival of packet at the other side */
  evptr = (struct event *)malloc(sizeof(struct event));
  evptr->evtype =  FROM_LAYER3;   /* packet will pop out from layer3 */
  evptr->eventity = (AorB+1) % 2; /* event occurs at other entity */
  evptr->pktptr = mypktptr;       /* save ptr to my copy of packet */
/* finally, compute the arrival time of packet at the other end.
   medium can not reorder, so make sure packet arrives between 1 and 10
   time units after the latest arrival time of packets
   currently in the medium on their way to the destination */
 lastime = time;
/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
 for (q=evlist; q!=NULL ; q = q->next) 
    if ( (q->evtype==FROM_LAYER3  && q->eventity==evptr->eventity) ) 
      lastime = q->evtime;
 evptr->evtime =  lastime + 1 + 9*jimsrand();
 


 /* simulate corruption: */
 if (jimsrand() < corruptprob)  {
    ncorrupt++;
    if ( (x = jimsrand()) < .75)
       mypktptr->payload[0]='Z';   /* corrupt payload */
      else if (x < .875)
       mypktptr->seqnum = 999999;
      else
       mypktptr->acknum = 999999;
    if (TRACE>0)    
	printf("          TOLAYER3: packet being corrupted\n");
    }  

  if (TRACE>2)  
     printf("          TOLAYER3: scheduling arrival on other side\n");
  insertevent(evptr);
} 

tolayer5(AorB,datasent)
  int AorB;
  char datasent[20];
{
  int i;  
  if (TRACE>2) {
     printf("          TOLAYER5: data received: ");
     for (i=0; i<20; i++)  
        printf("%c",datasent[i]);
     printf("\n");
   }
  
}
