// ---------------------------
// videopkt.h
//
// Video pkt header (Video Metadata Portion)
// In videohdr.h, structure 'hdr_video' 
// defines the Video Pkt Header
//
// Hao Hu
// 06/2010
// ---------------------------

#ifndef videopkt_h
#define videopkt_h


#define VIDEO_FRAME_TYPE_I 0
#define VIDEO_FRAME_TYPE_P 1
#define VIDEO_FRAME_TYPE_B 2

#define VIDEO_PKT_SVC_BL  0
#define VIDEO_PKT_SVC_EL  1

#define MAX_NUM_DESC 50
#define MAX_NUM_GOP  50

typedef enum
{
	I,
	P,
	//CHANNEL_CHANGE,
}FrameType;


struct videoPkt {
	//int eof;            // flag for end of video stream
	//int ack;            // flag for acknowledgment pkt
	//int seq;            // sequence number of each video pkt
	double enctime;     // time at which the pkt is ready at encoder
	double dectime;     // time at which the pkt is due at decoder
	double sendtime;
	FrameType frame_type;
	int trans_num;
	//double interval;    // time interval between current and next pkt
	//int gop;            // gop index
	int frame;          // frame index, in encoding order
	//int nalu;           // nalu index, in encoding order
	int slice;          // slice index within a NAL
	int nslice;         // number of slice in each NAL (same as number of pkts in each FEC block)
	int fecTotalSlice;

	int pktsize;        // encoded pkt size in bytes
	//int ftype;          // I, P or B  // for SVC: quality level (BL/EL)
	//int temporal;       // index of temporal level, for SVC;  (0 for AVC)
	//int quality;        // index of quality level: 0 for CBR; 
	//double dist;        // MSE distortion of current frame
	//double psnr;        // encoded PSNR of current frame

	//int displayorder;  // frame display index

	//int actualorder; // for loop use
	
	//for Q-R model extension
	//int QID;     
	int TID; 

	//double rate;

	//int layerID;
	
	// for frame Q-R model
	//double frate;
	//double qstep;
};



#endif				//ns_videopkt_h
