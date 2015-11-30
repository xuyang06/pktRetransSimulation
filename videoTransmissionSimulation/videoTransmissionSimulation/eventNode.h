#ifndef event_node_h
#define event_node_h

#include "videopkt.h"
#include <stdio.h>
#include <string>

typedef enum
{
	TRANS_PKT,
	RECV_ACK,
	SEND_TIMEOUT,
	READY_PKT,
	RECV_PKT,
	FRAME_PLAY,
	SEND_WINDOW_UPDATE,
	//CHANNEL_CHANGE,
}eventType;

struct EventNode{
	eventType nodeType;
	videoPkt pkt;
	double eventTime;
	EventNode* next_;
};




#endif