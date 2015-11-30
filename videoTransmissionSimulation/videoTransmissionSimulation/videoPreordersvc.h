// ---------------------------
// videoPreordersvc.h
//
// Video traffic trace of SVC streams 
// Pre-ordered version
//
// ---------------------------

#ifndef videoPreordersvc_h
#define videoPreordersvc_h

#include "videopkt.h"
#include "FIFOqueue.h"
#include "videosvcrq.h"
#include <stdio.h>
#include <string>
#include "EventFIFOqueue.h"
#include "videoDecoder.h"
#include <fstream>
#include <math.h>

class VideoSVCPreorder  {
	
	public:
		VideoSVCPreorder();
		VideoSVCPreorder(int fps, int numNALU, int pktsize);
		~VideoSVCPreorder();
		//void reload_FIFOqueue();
		bool read_videopkt_trace(const char * pkt_file, double transmissionDeadline, EventFIFOqueue *eventFIFOqueue, double timeoutDuration, VideoDecoder *videoDecoder, double repeatTime, FILE *f_status);
		bool read_preorder_table(const char * tab_file);
		void setFECOn();
		void setIFECRatio(float);
		void setPFECRatio(float);
		
	protected:
		
		void VideoSVCPreorder::init();
		// to be binded
		int     FECOn_;
		float	I_FEC_ratio_;
		float	P_FEC_ratio_;
		int     debug_;
		int     bursty_;  // whether traffic is bursty or in CBR pattern
		int     fps_;          
		int     pktsize_ ; // payload size (in Bytes)
		static const int GOPsize_ = 4;
		double  gop_time_;

		// tracefile recording transmitted video packets
		FILE *fp;           
		FILE *fp_debug;

		int numNALU_ ;
		int totalNumNALU_;
		int numFrms_; // = numNALU_/numQLayers_
		int totalNumFrms_;
		
		// holds svc pkt info for every nal unit
		// fragment needed for non bursty traffic
		// because VTP will not space up sending
		// time for fragment. Application layer should
		// determine the sending time for each frag
		//svcPkt** svcNALU_;
		videoPkt** svcNALU_;
		int* numFragsNALU_;

		// pre-order table
		ratePoints* rate_points_;
		int num_rate_points_;
		
		// read from tracefile
		static const int numTLayers_ = 3;
		static const int numQLayers_ = 1;
		
		// starts from 0
		int currNALU_;
		int lastNALU_;

		int gop_tosent_;

		double clock_; // local clock starts from the first pkt
		double startTime_;

	public:
		FIFOqueue toBeSent; //FIFO queue of pre-ordered pkt with in a GOP

		
		
};

#endif // videoPreordersvc_h

