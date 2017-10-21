# Reliable-Transport-Protocol

This project implements a simple reliable data transfer protocol.

## Version 1: The Alternating-Bit-Protocol Version

### Note
The double variable **'mmm'** (in line 550) is changed to 32767 (from 2147483647) in the submitted code. You might need to change this variable before running the program.

### Description
1. The alternating-bit-protocol version only uses 0 and 1 as sequnence number (as indicated by the meaning of alternating-bit).

2. The time-out-interval for the timer is set to 200.0 in this version. It can be changed through the **TIMER_INTERVAL** macro.

3. NACK packet is represented by normal ACK packet, with **acknum** bit set to -1.

4. The checksum calculation & evaluation algorithm is designed based on [THIS BLOG](http://www.roman10.net/2011/11/27/how-to-calculate-iptcpudp-checksumpart-1-theory/).

5. The checksum caculation algorithm contains the following 3 steps:
- Break the entire pkt (except for the checksum field) into 14 blocks, each containing 2 bytes of data.
- Sum up the 14 blocks, then fold the result into 16 bits by adding the carry to the sum.
- Obtain the final checksum by computing the complement of the sum.
6. The checksum validation algorithm is similar to the calculation process:
- Break the received pkt into (except for the checksum field) 14 blocks, each containing 2 bytes of data.
- Sum up the 14 blocks plus the checksum field, then fold the result into 16 bits by adding the carry to the sum.
- If the complement of the sum is 0, then the packet is untouched. Otherwise, it is corrupted.

### Running example
This is a running output when 10 messages have been correctly ACK'ed, with a loss possibility of 0.1, a corruption probability of 0.3, a mean-time-between-arrivals of 1000, and a trace level of 2. 

```
-----  Stop and Wait Network Simulator Version 1.1 --------

Enter the number of messages to simulate: 15
Enter packet loss probability [enter 0.0 for no loss]:0.1
Enter packet corruption probability [0.0 for no corruption]:0.3
Enter average time between messages from sender's layer5 [ > 0.0]:1000
Enter TRACE:2

EVENT time: 93.569748,  type: 1, fromlayer5  entity: 0
Info: Sending packet 0 to B ...

EVENT time: 99.062195,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 0
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 0

EVENT time: 101.561325,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 0
Info: Acknowledge received, transmission complete!

EVENT time: 1607.715088,  type: 1, fromlayer5  entity: 0
Info: Sending packet 1 to B ...

EVENT time: 1609.116333,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 1

EVENT time: 1614.552979,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 1
Info: Acknowledge received, transmission complete!

EVENT time: 2322.031250,  type: 1, fromlayer5  entity: 0
Info: Sending packet 0 to B ...

EVENT time: 2325.527344,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 0
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 0

EVENT time: 2328.412354,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 0
Info: Acknowledge received, transmission complete!

EVENT time: 3332.804199,  type: 1, fromlayer5  entity: 0
Info: Sending packet 1 to B ...
          TOLAYER3: packet being corrupted

EVENT time: 3337.380615,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 3341.256836,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit previous packet ...
Info: Retransmit packet 1 to B ...

EVENT time: 3346.818115,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Message sent to layer 5 (from B)
          TOLAYER3: packet being lost
Info: Acknowledge A with Ack num 1

EVENT time: 3541.256836,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 1 to B

EVENT time: 3547.204346,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Packet received has incorrect seq num, acknowledge with previous ACK packet
Info: Re-acked with ack num 1

EVENT time: 3549.402100,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 1
Info: Acknowledge received, transmission complete!

EVENT time: 5057.893555,  type: 1, fromlayer5  entity: 0
Info: Sending packet 0 to B ...
          TOLAYER3: packet being corrupted

EVENT time: 5058.931152,  type: 1, fromlayer5  entity: 0
Info: Packet in transit, cannot send message now!

EVENT time: 5062.472656,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 5064.000000,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit previous packet ...
Info: Retransmit packet 0 to B ...

EVENT time: 5065.919922,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 0
Info: Message sent to layer 5 (from B)
          TOLAYER3: packet being corrupted
Info: Acknowledge A with Ack num 0

EVENT time: 5075.554688,  type: 2, fromlayer3  entity: 0
Info: Packet received by A is corrupted, abandon

EVENT time: 5264.000000,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 0 to B

EVENT time: 5270.081543,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 0
Info: Packet received has incorrect seq num, acknowledge with previous ACK packet
Info: Re-acked with ack num 0

EVENT time: 5274.633301,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 0
Info: Acknowledge received, transmission complete!

EVENT time: 6557.268066,  type: 1, fromlayer5  entity: 0
Info: Sending packet 1 to B ...
          TOLAYER3: packet being corrupted

EVENT time: 6564.979980,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 6570.675781,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit previous packet ...
          TOLAYER3: packet being corrupted
Info: Retransmit packet 1 to B ...

EVENT time: 6578.529297,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 6586.390137,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit previous packet ...
          TOLAYER3: packet being corrupted
Info: Retransmit packet 1 to B ...

EVENT time: 6588.391113,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 6589.551758,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit previous packet ...
          TOLAYER3: packet being corrupted
Info: Retransmit packet 1 to B ...

EVENT time: 6593.748047,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being lost
Info: Sending NACK packet to A ...

EVENT time: 6789.551758,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 1 to B

EVENT time: 6791.095215,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 1

EVENT time: 6794.884277,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 1
Info: Acknowledge received, transmission complete!

EVENT time: 6854.091309,  type: 1, fromlayer5  entity: 0
Info: Sending packet 0 to B ...
          TOLAYER3: packet being corrupted

EVENT time: 6861.486328,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being lost
Info: Sending NACK packet to A ...

EVENT time: 7054.091309,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 0 to B

EVENT time: 7058.773926,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 0
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 0

EVENT time: 7061.742676,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 0
Info: Acknowledge received, transmission complete!

EVENT time: 8567.888672,  type: 1, fromlayer5  entity: 0
Info: Sending packet 1 to B ...
          TOLAYER3: packet being corrupted

EVENT time: 8575.871094,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being lost
Info: Sending NACK packet to A ...

EVENT time: 8767.888672,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 1 to B

EVENT time: 8773.627930,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Message sent to layer 5 (from B)
          TOLAYER3: packet being corrupted
Info: Acknowledge A with Ack num 1

EVENT time: 8779.997070,  type: 2, fromlayer3  entity: 0
Info: Packet received by A is corrupted, abandon

EVENT time: 8967.888672,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 1 to B

EVENT time: 8973.325195,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Packet received has incorrect seq num, acknowledge with previous ACK packet
          TOLAYER3: packet being corrupted
Info: Re-acked with ack num 1

EVENT time: 8974.351563,  type: 2, fromlayer3  entity: 0
Info: Packet received by A is corrupted, abandon

EVENT time: 9167.888672,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
          TOLAYER3: packet being corrupted
Info: Retransmit packet 1 to B

EVENT time: 9176.395508,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 9177.738281,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit previous packet ...
Info: Retransmit packet 1 to B ...

EVENT time: 9178.788086,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Packet received has incorrect seq num, acknowledge with previous ACK packet
          TOLAYER3: packet being lost
Info: Re-acked with ack num 1

EVENT time: 9351.054688,  type: 1, fromlayer5  entity: 0
Info: Packet in transit, cannot send message now!

EVENT time: 9377.738281,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 1 to B

EVENT time: 9385.912109,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Packet received has incorrect seq num, acknowledge with previous ACK packet
          TOLAYER3: packet being lost
Info: Re-acked with ack num 1

EVENT time: 9577.738281,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
          TOLAYER3: packet being corrupted
Info: Retransmit packet 1 to B

EVENT time: 9582.415039,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being corrupted
Info: Sending NACK packet to A ...

EVENT time: 9586.603516,  type: 2, fromlayer3  entity: 0
Info: Packet received by A is corrupted, abandon

EVENT time: 9777.738281,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 1 to B

EVENT time: 9787.673828,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Packet received has incorrect seq num, acknowledge with previous ACK packet
          TOLAYER3: packet being lost
Info: Re-acked with ack num 1

EVENT time: 9977.738281,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 1 to B

EVENT time: 9986.768555,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Packet received has incorrect seq num, acknowledge with previous ACK packet
          TOLAYER3: packet being lost
Info: Re-acked with ack num 1

EVENT time: 10177.738281,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
          TOLAYER3: packet being corrupted
Info: Retransmit packet 1 to B

EVENT time: 10181.818359,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being corrupted
Info: Sending NACK packet to A ...

EVENT time: 10186.354492,  type: 2, fromlayer3  entity: 0
Info: Packet received by A is corrupted, abandon

EVENT time: 10377.738281,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
          TOLAYER3: packet being corrupted
Info: Retransmit packet 1 to B

EVENT time: 10382.782227,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 10392.371094,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit previous packet ...
          TOLAYER3: packet being corrupted
Info: Retransmit packet 1 to B ...

EVENT time: 10396.857422,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 10400.151367,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit previous packet ...
          TOLAYER3: packet being lost
Info: Retransmit packet 1 to B ...

EVENT time: 10600.151367,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 1 to B

EVENT time: 10603.973633,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Packet received has incorrect seq num, acknowledge with previous ACK packet
Info: Re-acked with ack num 1

EVENT time: 10610.795898,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 1
Info: Acknowledge received, transmission complete!

EVENT time: 11012.116211,  type: 1, fromlayer5  entity: 0
Info: Sending packet 0 to B ...
          TOLAYER3: packet being lost

EVENT time: 11212.116211,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
Info: Retransmit packet 0 to B

EVENT time: 11214.657227,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 0
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 0

EVENT time: 11215.809570,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 0
Info: Acknowledge received, transmission complete!

EVENT time: 12017.212891,  type: 1, fromlayer5  entity: 0
Info: Sending packet 1 to B ...

EVENT time: 12024.374023,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 1

EVENT time: 12029.194336,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 1
Info: Acknowledge received, transmission complete!
```

##Version 2: The Go-Back-N Version

### Note
The double variable **'mmm'** (in line 671) is changed to 32767 (from 2147483647) in the submitted code. You might need to change this variable before running the program.

### Description
1. The time-out-interval for the timer is set to 50.0 in this version. It can be changed through the **TIMER_INTERVAL** macro.

2. NACK packet is represented by normal ACK packet, with **acknum** bit set to -1. Upon receving NACK packet, the sender(A) will retransmit all packets in the current window.

3. The checksum calculation & evaluation algorithm is the same as the ones used in Version 1.

4. The maximum window size is set to 8. It can be changed through the **WINDOW_SIZE** macro. To avoid sequence# collision, the maximum sequence number (**MAX_SEQ_NUM**) is set to 16.

5. The Go-Back-N version provides buffering for incoming message, which will be stored in a linkedlist with maximum size of 50. Buffered message will be automatically sent if there is available space in the window.

6. The current window is also buffered in linkedlist, with a window size of 8.

### Running example
This is a running output when 20 messages have been correctly ACK'ed, with a loss possibility of 0.2, a corruption probability of 0.2, a mean-time-between-arrivals of 10, and a trace level of 2. 

```
-----  Stop and Wait Network Simulator Version 1.1 --------

Enter the number of messages to simulate: 30
Enter packet loss probability [enter 0.0 for no loss]:0.2
Enter packet corruption probability [0.0 for no corruption]:0.2
Enter average time between messages from sender's layer5 [ > 0.0]:10
Enter TRACE:2

EVENT time: 0.935697,  type: 1, fromlayer5  entity: 0
Info: Sending packet 0 to B ...
Info: Window size: 8. Current num of unacked pkts: 1

EVENT time: 6.428144,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 0
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 0

EVENT time: 8.927275,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 0
Info: ACK received, packets with seq num up to 0 have been received
Info: All packets sent, timer A stopped!

EVENT time: 16.077150,  type: 1, fromlayer5  entity: 0
Info: Sending packet 1 to B ...
Info: Window size: 8. Current num of unacked pkts: 1

EVENT time: 17.478437,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 1

EVENT time: 22.915127,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 1
Info: ACK received, packets with seq num up to 1 have been received
Info: All packets sent, timer A stopped!

EVENT time: 23.220314,  type: 1, fromlayer5  entity: 0
Info: Sending packet 2 to B ...
Info: Window size: 8. Current num of unacked pkts: 1

EVENT time: 26.716484,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 2
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 2

EVENT time: 29.601521,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 2
Info: ACK received, packets with seq num up to 2 have been received
Info: All packets sent, timer A stopped!

EVENT time: 33.328045,  type: 1, fromlayer5  entity: 0
Info: Sending packet 3 to B ...
Info: Window size: 8. Current num of unacked pkts: 1

EVENT time: 37.904480,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 3
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 3

EVENT time: 45.933746,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 3
Info: ACK received, packets with seq num up to 3 have been received
Info: All packets sent, timer A stopped!

EVENT time: 50.578938,  type: 1, fromlayer5  entity: 0
Info: Sending packet 4 to B ...
Info: Window size: 8. Current num of unacked pkts: 1

EVENT time: 56.140324,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 4
Info: Message sent to layer 5 (from B)
          TOLAYER3: packet being lost
Info: Acknowledge A with Ack num 4

EVENT time: 57.463303,  type: 1, fromlayer5  entity: 0
Info: Sending packet 5 to B ...
Info: Window size: 8. Current num of unacked pkts: 2

EVENT time: 66.798332,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 5
Info: Message sent to layer 5 (from B)
          TOLAYER3: packet being lost
Info: Acknowledge A with Ack num 5

EVENT time: 68.762474,  type: 1, fromlayer5  entity: 0
Info: Sending packet 6 to B ...
Info: Window size: 8. Current num of unacked pkts: 3
          TOLAYER3: packet being lost

EVENT time: 81.533249,  type: 1, fromlayer5  entity: 0
Info: Sending packet 7 to B ...
Info: Window size: 8. Current num of unacked pkts: 4

EVENT time: 84.911591,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 7
Info: Packet received has incorrect seq num, expecting #6, acknowledge with previous ACK packet
          TOLAYER3: packet being corrupted
Info: Re-acked with ack num 5

EVENT time: 93.890381,  type: 2, fromlayer3  entity: 0
Info: Packet received by A is corrupted, abandon!

EVENT time: 100.578934,  type: 0, timerinterrupt   entity: 0
Info: Timer A times out!
          TOLAYER3: packet being corrupted
Info: Retransmit all packets in the window to B

EVENT time: 100.750145,  type: 1, fromlayer5  entity: 0
Info: Sending packet 8 to B ...
Info: Window size: 8. Current num of unacked pkts: 5
          TOLAYER3: packet being corrupted

EVENT time: 102.498795,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 4
Info: Packet received has incorrect seq num, expecting #6, acknowledge with previous ACK packet
Info: Re-acked with ack num 5

EVENT time: 103.718376,  type: 1, fromlayer5  entity: 0
Info: Sending packet 9 to B ...
Info: Window size: 8. Current num of unacked pkts: 6

EVENT time: 108.194496,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 5
Info: ACK received, packets with seq num up to 5 have been received
Info: Timer A restarted for packet with seq num 6

EVENT time: 112.133759,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being lost
Info: Sending NACK packet to A ...

EVENT time: 118.215088,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 6
Info: Message sent to layer 5 (from B)
          TOLAYER3: packet being corrupted
Info: Acknowledge A with Ack num 6

EVENT time: 120.704979,  type: 1, fromlayer5  entity: 0
Info: Sending packet 10 to B ...
Info: Window size: 8. Current num of unacked pkts: 5
          TOLAYER3: packet being lost

EVENT time: 122.766800,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 7
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 7

EVENT time: 123.093971,  type: 1, fromlayer5  entity: 0
Info: Sending packet 11 to B ...
Info: Window size: 8. Current num of unacked pkts: 6
          TOLAYER3: packet being corrupted

EVENT time: 127.738815,  type: 2, fromlayer3  entity: 0
Info: Packet received by A is corrupted, abandon!

EVENT time: 128.899490,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 7
Info: ACK received, packets with seq num up to 7 have been received
Info: Timer A restarted for packet with seq num 8

EVENT time: 130.478546,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 132.022110,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being lost
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being lost

EVENT time: 133.344635,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 9
Info: Packet received has incorrect seq num, expecting #8, acknowledge with previous ACK packet
Info: Re-acked with ack num 7

EVENT time: 136.648270,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 138.027359,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 7
Info: Duplicate ACK #7 received, abandon ACK packet!

EVENT time: 138.522293,  type: 1, fromlayer5  entity: 0
Info: Sending packet 12 to B ...
Info: Window size: 8. Current num of unacked pkts: 5

EVENT time: 140.995895,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being lost

EVENT time: 142.566177,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 9
Info: Packet received has incorrect seq num, expecting #8, acknowledge with previous ACK packet
Info: Re-acked with ack num 7

EVENT time: 146.353958,  type: 1, fromlayer5  entity: 0
Info: Sending packet 13 to B ...
Info: Window size: 8. Current num of unacked pkts: 6
          TOLAYER3: packet being lost

EVENT time: 146.521042,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 7
Info: Duplicate ACK #7 received, abandon ACK packet!

EVENT time: 149.960968,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being corrupted
Info: Sending NACK packet to A ...

EVENT time: 157.942993,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 12
Info: Packet received has incorrect seq num, expecting #8, acknowledge with previous ACK packet
Info: Re-acked with ack num 7

EVENT time: 158.619217,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being lost

EVENT time: 159.605209,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 8
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 8

EVENT time: 159.668930,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 7
Info: Duplicate ACK #7 received, abandon ACK packet!

EVENT time: 161.276489,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 8
Info: ACK received, packets with seq num up to 8 have been received
Info: Timer A restarted for packet with seq num 9

EVENT time: 163.035980,  type: 1, fromlayer5  entity: 0
Info: Sending packet 14 to B ...
Info: Window size: 8. Current num of unacked pkts: 6

EVENT time: 166.624527,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 9
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 9

EVENT time: 167.988739,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 9
Info: ACK received, packets with seq num up to 9 have been received
Info: Timer A restarted for packet with seq num 10

EVENT time: 168.410904,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 172.947510,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being lost
          TOLAYER3: packet being lost
          TOLAYER3: packet being corrupted

EVENT time: 174.646591,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 11
Info: Packet received has incorrect seq num, expecting #10, acknowledge with previous ACK packet
Info: Re-acked with ack num 9

EVENT time: 177.940613,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 9
Info: Duplicate ACK #9 received, abandon ACK packet!

EVENT time: 178.731644,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 9
Info: Packet received has incorrect seq num, expecting #10, acknowledge with previous ACK packet
          TOLAYER3: packet being lost
Info: Re-acked with ack num 9

EVENT time: 179.742905,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 10
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 10

EVENT time: 180.880768,  type: 1, fromlayer5  entity: 0
Info: Sending packet 15 to B ...
Info: Window size: 8. Current num of unacked pkts: 6

EVENT time: 182.774063,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 11
Info: Message sent to layer 5 (from B)
          TOLAYER3: packet being lost
Info: Acknowledge A with Ack num 11

EVENT time: 183.565109,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 10
Info: ACK received, packets with seq num up to 10 have been received
Info: Timer A restarted for packet with seq num 11

EVENT time: 186.962112,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 12
Info: Message sent to layer 5 (from B)
          TOLAYER3: packet being lost
Info: Acknowledge A with Ack num 12

EVENT time: 188.850372,  type: 1, fromlayer5  entity: 0
Info: Sending packet 0 to B ...
Info: Window size: 8. Current num of unacked pkts: 6
          TOLAYER3: packet being corrupted

EVENT time: 192.275772,  type: 1, fromlayer5  entity: 0
Info: Sending packet 1 to B ...
Info: Window size: 8. Current num of unacked pkts: 7

EVENT time: 195.244629,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 13
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 13

EVENT time: 196.686020,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 14
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 14

EVENT time: 200.064697,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 13
Info: ACK received, packets with seq num up to 13 have been received
Info: Timer A restarted for packet with seq num 14

EVENT time: 201.378098,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 14
Info: ACK received, packets with seq num up to 14 have been received
Info: Timer A restarted for packet with seq num 15

EVENT time: 205.843338,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 10
Info: Packet received has incorrect seq num, expecting #15, acknowledge with previous ACK packet
          TOLAYER3: packet being lost
Info: Re-acked with ack num 14

EVENT time: 209.515686,  type: 1, fromlayer5  entity: 0
Info: Sending packet 2 to B ...
Info: Window size: 8. Current num of unacked pkts: 4
          TOLAYER3: packet being corrupted

EVENT time: 215.431885,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 13
Info: Packet received has incorrect seq num, expecting #15, acknowledge with previous ACK packet
Info: Re-acked with ack num 14

EVENT time: 219.918228,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being lost
Info: Sending NACK packet to A ...

EVENT time: 221.291016,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 14
Info: Duplicate ACK #14 received, abandon ACK packet!

EVENT time: 226.490936,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 15
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 15

EVENT time: 228.006851,  type: 1, fromlayer5  entity: 0
Info: Sending packet 3 to B ...
Info: Window size: 8. Current num of unacked pkts: 5
          TOLAYER3: packet being lost

EVENT time: 231.094559,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 15
Info: ACK received, packets with seq num up to 15 have been received
Info: Timer A restarted for packet with seq num 0

EVENT time: 234.819321,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being corrupted
Info: Sending NACK packet to A ...

EVENT time: 236.340912,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being corrupted

EVENT time: 241.980362,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Packet received has incorrect seq num, expecting #0, acknowledge with previous ACK packet
Info: Re-acked with ack num 15

EVENT time: 244.109634,  type: 1, fromlayer5  entity: 0
Info: Sending packet 4 to B ...
Info: Window size: 8. Current num of unacked pkts: 5
          TOLAYER3: packet being lost

EVENT time: 244.458618,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 15
Info: Duplicate ACK #15 received, abandon ACK packet!

EVENT time: 246.743301,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
          TOLAYER3: packet being corrupted
Info: Sending NACK packet to A ...

EVENT time: 250.450684,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 0
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 0

EVENT time: 251.931137,  type: 2, fromlayer3  entity: 0
Info: Packet received by A is corrupted, abandon!

EVENT time: 251.997543,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 1
Info: Message sent to layer 5 (from B)
Info: Acknowledge A with Ack num 1

EVENT time: 254.350433,  type: 1, fromlayer5  entity: 0
Info: Sending packet 5 to B ...
Info: Window size: 8. Current num of unacked pkts: 6

EVENT time: 255.431366,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 259.486603,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 0
Info: ACK received, packets with seq num up to 0 have been received
Info: Timer A restarted for packet with seq num 1

EVENT time: 262.601196,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 267.840790,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 1
Info: ACK received, packets with seq num up to 1 have been received
Info: Timer A restarted for packet with seq num 2

EVENT time: 272.594055,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 5
Info: Packet received has incorrect seq num, expecting #2, acknowledge with previous ACK packet
Info: Re-acked with ack num 1

EVENT time: 274.095306,  type: 1, fromlayer5  entity: 0
Info: Sending packet 6 to B ...
Info: Window size: 8. Current num of unacked pkts: 5
          TOLAYER3: packet being corrupted

EVENT time: 276.479614,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 277.569153,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being lost
          TOLAYER3: packet being corrupted

EVENT time: 283.318695,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 2
Info: Message sent to layer 5 (from B)
          TOLAYER3: packet being corrupted
Info: Acknowledge A with Ack num 2

EVENT time: 283.887512,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being lost
          TOLAYER3: packet being lost

EVENT time: 284.579010,  type: 1, fromlayer5  entity: 0
Info: Sending packet 7 to B ...
Info: Window size: 8. Current num of unacked pkts: 6

EVENT time: 286.475098,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 1
Info: Duplicate ACK #1 received, abandon ACK packet!

EVENT time: 289.870911,  type: 1, fromlayer5  entity: 0
Info: Sending packet 8 to B ...
Info: Window size: 8. Current num of unacked pkts: 7

EVENT time: 290.238037,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 294.011902,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 294.396423,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being lost
          TOLAYER3: packet being corrupted

EVENT time: 296.266571,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 298.880798,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 2
Info: Packet received has incorrect seq num, expecting #3, acknowledge with previous ACK packet
Info: Re-acked with ack num 2

EVENT time: 302.226013,  type: 1, fromlayer5  entity: 0
Info: Sending packet 9 to B ...
Info: Window size: 8. Current num of unacked pkts: 8

EVENT time: 302.707764,  type: 2, fromlayer3  entity: 0
Info: Packet received by A is corrupted, abandon!

EVENT time: 304.968719,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 4
Info: Packet received has incorrect seq num, expecting #3, acknowledge with previous ACK packet
Info: Re-acked with ack num 2

EVENT time: 311.080627,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being lost
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being lost

EVENT time: 314.215851,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 5
Info: Packet received has incorrect seq num, expecting #3, acknowledge with previous ACK packet
Info: Re-acked with ack num 2

EVENT time: 317.436890,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being lost
          TOLAYER3: packet being lost
          TOLAYER3: packet being lost

EVENT time: 319.612976,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 7
Info: Packet received has incorrect seq num, expecting #3, acknowledge with previous ACK packet
          TOLAYER3: packet being corrupted
Info: Re-acked with ack num 2

EVENT time: 319.625366,  type: 2, fromlayer3  entity: 0
Info: Receive NACK packet from B, retransmit all packets in the window ...
          TOLAYER3: packet being corrupted
          TOLAYER3: packet being lost
          TOLAYER3: packet being lost
          TOLAYER3: packet being lost

EVENT time: 320.915558,  type: 1, fromlayer5  entity: 0
Info: Incoming packets fall outside of window, buffer packets!
Info: Current buffer size: 1

EVENT time: 324.631897,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 8
Info: Packet received has incorrect seq num, expecting #3, acknowledge with previous ACK packet
          TOLAYER3: packet being lost
Info: Re-acked with ack num 2

EVENT time: 326.613098,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 2
Info: ACK received, packets with seq num up to 2 have been received
Info: Timer A restarted for packet with seq num 3
Info: Send buffered packet ...
Info: Sending packet 10 to B ...
Info: Window size: 8. Current num of unacked pkts: 8

EVENT time: 328.039185,  type: 1, fromlayer5  entity: 0
Info: Incoming packets fall outside of window, buffer packets!
Info: Current buffer size: 1

EVENT time: 332.939240,  type: 1, fromlayer5  entity: 0
Info: Incoming packets fall outside of window, buffer packets!
Info: Current buffer size: 2

EVENT time: 333.289886,  type: 2, fromlayer3  entity: 1
Info: Receive packet from A with seq num 2
Info: Packet received has incorrect seq num, expecting #3, acknowledge with previous ACK packet
          TOLAYER3: packet being corrupted
Info: Re-acked with ack num 2

EVENT time: 334.104004,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 2
Info: Duplicate ACK #2 received, abandon ACK packet!

EVENT time: 339.676117,  type: 2, fromlayer3  entity: 0
Info: Receive ACK packet from B with ack num 2
Info: Duplicate ACK #2 received, abandon ACK packet!

EVENT time: 342.186829,  type: 2, fromlayer3  entity: 1
Info: Packet received by B is corrupted, respond with NACK
Info: Sending NACK packet to A ...

EVENT time: 344.350098,  type: 1, fromlayer5  entity: 0
Info: Incoming packets fall outside of window, buffer packets!
Info: Current buffer size: 3

EVENT time: 346.076843,  type: 1, fromlayer5  entity: 0
 Simulator terminated at time 346.076843
 after sending 30 msgs from layer5
```