// -----------------------
// videoPreordersvc.cc
// 
// SVC traffic generator  
// use pre-ordered pkts
// 
// -----------------------

#include <math.h>
#include "stdafx.h"
#include "videoPreordersvc.h"


#define		min(x,y)	((x) < (y) ? (x) : (y))
#define		max(x,y)	((x) > (y) ? (x) : (y))


VideoSVCPreorder::VideoSVCPreorder()
{
	debug_ = 1;
	bursty_ = 0;
	fps_ = 30;	
	pktsize_ = 1400;
	FECOn_ = 0;
	
	gop_time_ = 0.0;
	fp = NULL;
	fp_debug = NULL;	
	numNALU_ = 300;
	numFrms_ = 0;

	totalNumNALU_ = 0;
	totalNumFrms_ = 0;

	svcNALU_ = NULL;
	numFragsNALU_ = NULL;

	rate_points_ = NULL;
	num_rate_points_ = 0;

	currNALU_  = 0;
	lastNALU_  = 0;
	I_FEC_ratio_ = 0.0;
	P_FEC_ratio_ = 0.0;
	VideoSVCPreorder::init();
}

VideoSVCPreorder::VideoSVCPreorder(int fps, int numNALU, int pktsize)
{
	debug_ = 1;
	bursty_ = 0;
	fps_ = fps;	
	pktsize_ = pktsize;
	FECOn_ = 0;
	
	gop_time_ = 0.0;
	fp = NULL;
	fp_debug = NULL;	
	numNALU_ = numNALU;
	numFrms_ = 0;

	totalNumNALU_ = 0;
	totalNumFrms_ = 0;

	svcNALU_ = NULL;
	numFragsNALU_ = NULL;

	rate_points_ = NULL;
	num_rate_points_ = 0;

	currNALU_  = 0;
	lastNALU_  = 0;
	I_FEC_ratio_ = 0.0;
	P_FEC_ratio_ = 0.0;
	VideoSVCPreorder::init();
}




VideoSVCPreorder::~VideoSVCPreorder()
{
	printf("delete variables in VideoSVCPreorder.\n");
	if( fp != NULL ) fclose(fp);
	if( fp_debug != NULL ) fclose(fp_debug);
	
	if( svcNALU_ != NULL )
	{
		for(int i=0; i<numNALU_; i++)
			if (svcNALU_[i] != NULL) 
				delete [] svcNALU_[i];

		delete [] svcNALU_;
	}	

	if(numFragsNALU_!= NULL) delete [] numFragsNALU_;

}

void VideoSVCPreorder::setFECOn(){
	FECOn_ = 1;
}

void VideoSVCPreorder::setIFECRatio( float _I_FEC_ratio_){
	I_FEC_ratio_ = _I_FEC_ratio_;
}

void VideoSVCPreorder::setPFECRatio( float _P_FEC_ratio_){
	P_FEC_ratio_ = _P_FEC_ratio_;
}

void VideoSVCPreorder::init()
{
	gop_time_ = (double)GOPsize_/fps_;
	gop_tosent_ = 0;
}


/*
void VideoSVCPreorder::reload_FIFOqueue()
{
	//loop back if all NALU have been sent
	int nal_idx_low  = (gop_tosent_*GOPsize_*numQLayers_)%numNALU_;
	int nal_idx_high = nal_idx_low + GOPsize_*numQLayers_;
	int pickFlag[GOPsize_*numQLayers_];

	memset(pickFlag, 0, GOPsize_*numQLayers_*sizeof(int));

	//clear_FIFOqueue
	toBeSent.clearQ();

	for(int i=0; i<num_rate_points_; i++)
	{
		int QID_high = rate_points_[i].QID;
		int TID_high = rate_points_[i].TID;

		for (int j=nal_idx_low; j<nal_idx_high; j++ )
		{
			int curr_NAL_QID = svcNALU_[j][0].QID_;
			int curr_NAL_TID = svcNALU_[j][0].TID_;

			if (curr_NAL_QID>QID_high || curr_NAL_TID>TID_high)
				continue;
			else {

				if ( pickFlag[j-nal_idx_low] == 1 )
					continue;
				else {
					for (int k=0; k<numFragsNALU_[j]; k++) {

						videoPkt tmp;
						svcPkt svc_pkt = svcNALU_[j][k];
						
						memset(&tmp,0, sizeof(videoPkt));
						
						tmp.displayorder = svc_pkt.frm_+ (gop_tosent_/(numFrms_/GOPsize_))*numFrms_;
						tmp.actualorder  = svc_pkt.frm_;
						tmp.pktsize	     = svc_pkt.bits_/8; // may require 1 more bytes	
						tmp.QID		     = svc_pkt.QID_;
						tmp.TID		     = svc_pkt.TID_;
						tmp.nslice	     = svc_pkt.fragNum_;  
						tmp.slice	     = svc_pkt.fragIdx_; 
						tmp.psnr         = svc_pkt.PSNR_;
						tmp.layerID      = i;
						tmp.dectime      = (double)tmp.displayorder*(1/(double)fps_)+ startTime_;
						tmp.gop          = gop_tosent_;

						toBeSent.push(tmp);

						printf("push in %d frame\n", tmp.displayorder);

					}

					pickFlag[j-nal_idx_low] = 1;

				}
			}
		}
	}

	// for debug
	int pkts_enque = 0;
	for (int j=nal_idx_low; j<nal_idx_high; j++)
		pkts_enque += numFragsNALU_[j];

	if (pkts_enque != toBeSent.size()) {
		printf("ERROR when loading FIFO queue!\n");
		exit(0);
	}

	return;
}
*/
/*
void VideoSVCPreorder::timeout()
{  
	if (debug_)
	{
		printf("***svc sending a GOP to lower layer***\n");
		fflush(stdout);
	}
	
	if(!running_) return;

	reload_FIFOqueue();
	
	agent_->sendmsg(-1, (char*)&toBeSent);
		
	gop_tosent_++;
	
	timer_.resched(gop_time_);

	return;
}
*/
// read in per-packet trace 
// **********************
// ** with I frame excluded **
// **********************
bool VideoSVCPreorder::read_videopkt_trace(const char * pkt_file, double transmissionDeadline, 
	EventFIFOqueue *eventFIFOqueue, double timeoutDuration, VideoDecoder *videoDecoder, 
	double repeatTime, FILE *f_status)
{   
	fprintf(f_status, "Begin to svc preorder: ............................ \n");
	fprintf(f_status, "\n");
	fprintf(f_status, "\n");
	//f_status<<"Begin to svc preorder: ............................"<<endl;
	
	//f_status<<endl;
	
	//f_status<<endl;


	FILE *desc_fp=NULL;
	desc_fp =  fopen(pkt_file, "r");
	
	if(desc_fp==NULL)
	{
		printf("Error in VideoSVCPreorder::read_videopkt_trace: cannot open description %s\n", pkt_file);
		return false;
	}
	
	//fscanf(desc_fp, "%d", &numNALU_);  // number of NAL pkt
	//fscanf(desc_fp,"%*[^\n]"); // read in rest of the line

	if (fp_debug)
		fprintf(fp_debug,"NUM Q layers: %d  NUM T layers: %d\n",numQLayers_, numTLayers_);
	
	totalNumNALU_ = numNALU_*repeatTime;
	numFrms_  = numNALU_/numQLayers_;
	totalNumFrms_ = numFrms_*repeatTime;
	double repeatInterval = (numNALU_/3)*100;
	printf("NUM Q layers: %d  NUM T layers: %d\n",numQLayers_, numTLayers_);
	printf("numNALU_ %d  NUM frames: %d  GOPsize: %d\n",numNALU_, numFrms_, GOPsize_);

	if (( numNALU_%numQLayers_ != 0 ) && ( numFrms_%GOPsize_ != 0)) {
		printf("Error in videopkt_trace file\n");
		printf("-->The number of NALU are not supported!\n");
		printf("-->or, The number of Frames are not supported!\n");
		return false;
	}		
	
	svcNALU_ = new videoPkt *[totalNumNALU_];
	numFragsNALU_ = new int [totalNumNALU_];  
	
	if(debug_) printf("VideoSVCPreorder reading in %d NAL pkts ...\n", numNALU_);
	
	int tmp_num_pkt = 0;
	int tmp_fec_total_pkt = 0;

	//int tmp_QID;
	int tmp_TID;
	char tmp_frm_type[2];
	int tmp_bits;
	int tmp_frm;
	//int tmp_QP;
	//double tmp_PSNR;

	for (int i=0; i<numNALU_; i++)
	{
		// 1  \t<Frame number>
		// 2  \t<TID> 
		// 3  \t<QID> 
		// 4  \t<QP> 
		// 5  \t<PSNR> 
		// 6  \t<bits> 
		
		fscanf(desc_fp, "%d", 	&tmp_frm);
		fscanf(desc_fp, "%d", 	&tmp_TID);
		fscanf(desc_fp, "%s", 	&tmp_frm_type);
		//fscanf(desc_fp, "%d", 	&tmp_QID);
		//fscanf(desc_fp, "%d", 	&tmp_QP);
		//fscanf(desc_fp, "%lf", 	&tmp_PSNR);
		fscanf(desc_fp, "%d", 	&tmp_bits);
		fscanf(desc_fp,"%*[^\n]"); // read in rest of the line

		if (fp_debug)
			fprintf(fp_debug,"%d %d %d \n",tmp_frm,tmp_TID, tmp_bits);


		tmp_num_pkt = (tmp_bits/(8*pktsize_) + (tmp_bits % (8*pktsize_) ? 1 : 0));

		if (FECOn_ == 0){
			tmp_fec_total_pkt = tmp_num_pkt;
		}else{
			if (tmp_TID == 0){
				if ( strcmp(tmp_frm_type,"I") == 0){
					int totalSize = ceil(tmp_num_pkt*(1 + I_FEC_ratio_));
					tmp_fec_total_pkt = totalSize;
				}else{
					int totalSize = ceil(tmp_num_pkt*(1 + P_FEC_ratio_));
					tmp_fec_total_pkt = totalSize;
				}
			}else{
				tmp_fec_total_pkt = tmp_num_pkt;
			}
		}
		
		if(debug_) 
			printf("Reading in NAL Pkt %d,  frame %d,  size %d, %d pkts, %d FEC pkts...\n", 
				   i, tmp_frm, tmp_bits, tmp_num_pkt, tmp_fec_total_pkt);
		
		
		// translate to video_desc
		for (int k = 0; k< repeatTime; k++){
			//svcNALU_[i+k*numNALU_] = new videoPkt [tmp_num_pkt];
			svcNALU_[i+k*numNALU_] = new videoPkt [tmp_fec_total_pkt];
			if ( strcmp(tmp_frm_type,"I") == 0){
				videoDecoder->addSmallestNumInOneNewGOP(tmp_frm+k*numNALU_);
			}
		}


		int remain = tmp_bits % (8*pktsize_) ;
		
		//for (int j=0; j<tmp_num_pkt; j++)
		for (int j=0; j<tmp_fec_total_pkt; j++)
		{
			if (j == tmp_num_pkt -1 && remain > 0 )
			{
				for (int k = 0; k< repeatTime; k++){
					//svcNALU_[i+k*numNALU_][tmp_num_pkt-1].QID     = tmp_QID;
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].TID     = tmp_TID;
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].pktsize   = ((tmp_bits % (8*pktsize_))/8 +  ((((tmp_bits % (8*pktsize_))/8)%8) ? 1 : 0) );
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].enctime   = (tmp_frm/3)*100.00 + (tmp_frm%3)*33.33 + k*repeatInterval; 	 
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].dectime   = svcNALU_[i+k*numNALU_][tmp_num_pkt-1].enctime + transmissionDeadline;
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].sendtime   = -1;
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].trans_num = 1;
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].frame = tmp_frm + k*numNALU_;
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].slice = tmp_num_pkt-1;
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].nslice = tmp_num_pkt;
					svcNALU_[i+k*numNALU_][tmp_num_pkt-1].fecTotalSlice = tmp_fec_total_pkt;
					if ( strcmp(tmp_frm_type,"I") == 0){
						svcNALU_[i+k*numNALU_][tmp_num_pkt-1].frame_type = I;
					}else{
						svcNALU_[i+k*numNALU_][tmp_num_pkt-1].frame_type = P;
					}
					/*
					if ( i == 0 ){
						svcNALU_[i+k*numNALU_][tmp_num_pkt-1].frame_type = I;
					}else{
						svcNALU_[i+k*numNALU_][tmp_num_pkt-1].frame_type = P;					
					}*/
					eventFIFOqueue->push(READY_PKT, svcNALU_[i+k*numNALU_][tmp_num_pkt-1], svcNALU_[i+k*numNALU_][tmp_num_pkt-1].enctime);
					//eventFIFOqueue.push(SEND_TIMEOUT, svcNALU_[i][tmp_num_pkt-1], (svcNALU_[i][tmp_num_pkt-1].enctime+timeoutDuration));
					if((tmp_num_pkt-1)==0){
						eventFIFOqueue->push(FRAME_PLAY, svcNALU_[i+k*numNALU_][0], svcNALU_[i+k*numNALU_][0].dectime);
						videoDecoder->addOneNewFrame(svcNALU_[i+k*numNALU_][0], tmp_bits);
					}
				
				//svcNALU_[i][tmp_num_pkt-1].PSNR_    = tmp_PSNR;
				}
				//eventFIFOqueue.PrintFIFOqueue();
			} else {
				for (int k = 0; k< repeatTime; k++){
					//svcNALU_[i+k*numNALU_][j].QID = tmp_QID;
					svcNALU_[i+k*numNALU_][j].TID = tmp_TID;
					svcNALU_[i+k*numNALU_][j].pktsize = pktsize_;
					svcNALU_[i+k*numNALU_][j].enctime = (tmp_frm/3)*100.00 + (tmp_frm%3)*33.33 + k*repeatInterval;
					svcNALU_[i+k*numNALU_][j].dectime = svcNALU_[i+k*numNALU_][j].enctime + transmissionDeadline;
					svcNALU_[i+k*numNALU_][j].sendtime   = -1;
					svcNALU_[i+k*numNALU_][j].trans_num = 1;
					svcNALU_[i+k*numNALU_][j].frame = tmp_frm + k*numNALU_;
					svcNALU_[i+k*numNALU_][j].slice = j;
					svcNALU_[i+k*numNALU_][j].nslice= tmp_num_pkt;
					svcNALU_[i+k*numNALU_][j].fecTotalSlice = tmp_fec_total_pkt;
					if ( strcmp(tmp_frm_type,"I") == 0){
						svcNALU_[i+k*numNALU_][j].frame_type = I;
					}else{
						svcNALU_[i+k*numNALU_][j].frame_type = P;
					}
					/*
					if ( i == 0 ){
						svcNALU_[i+k*numNALU_][j].frame_type = I;
					}else{
						svcNALU_[i+k*numNALU_][j].frame_type = P;					
					}
					*/
					eventFIFOqueue->push(READY_PKT, svcNALU_[i+k*numNALU_][j], svcNALU_[i+k*numNALU_][j].enctime);
					if(j==0){
						eventFIFOqueue->push(FRAME_PLAY, svcNALU_[i+k*numNALU_][j], svcNALU_[i+k*numNALU_][j].dectime);
						videoDecoder->addOneNewFrame(svcNALU_[i+k*numNALU_][j], tmp_bits);
					}
				}
				//eventFIFOqueue.push(SEND_TIMEOUT, svcNALU_[i][j], (svcNALU_[i][j].enctime+timeoutDuration));
				//svcNALU_[i][j].PSNR_ = tmp_PSNR;
				//eventFIFOqueue.PrintFIFOqueue();
			}
			//printf("DEBUGGING: saving frame %d, segno %d/%d, size %d....\n", tmp_frm, j, tmp_num_pkt, svcNALU_[i][j].bits_);
		}
		for (int k = 0; k< repeatTime; k++){
			//numFragsNALU_[i+k*numNALU_] = tmp_num_pkt;
			numFragsNALU_[i+k*numNALU_] = tmp_fec_total_pkt;
		}
	}
	eventFIFOqueue->PrintFIFOqueue(f_status);
	if (fp_debug)
		fflush(fp_debug);
	
	fclose(desc_fp);

	videoDecoder->sortSmallestNumInGOPs(f_status);
	
	fprintf(f_status, "End svc preorder: ............................ \n");
	fprintf(f_status, "\n");
	fprintf(f_status, "\n");
	//f_status<<"End svc preorder: ............................"<<endl;
	
	//f_status<<endl;
	
	//f_status<<endl;


	return true;
}


bool VideoSVCPreorder::read_preorder_table(const char * tab_file)
{   
	FILE *desc_fp=NULL;
	desc_fp =  fopen(tab_file, "r");
	
	if(desc_fp==NULL)
	{
		printf("Error in VideoSVCPreorder::read_preorder_table: cannot open description %s\n", tab_file);
		return false;
	}
	   
	// get number of preorder points first
    int num_rate_points = 0;
	double temp_rate;
	while(fscanf(desc_fp,"%lf", 	&temp_rate) > 0)
	{
	   	num_rate_points++;
		fscanf(desc_fp,"%*[^\n]"); // read in rest of the line
	}
	
	rate_points_ = new ratePoints [num_rate_points];

	num_rate_points_ = num_rate_points;
	
	printf("VideoSVCPreorder reading in %d preorder points ...\n", num_rate_points);
	
	fclose(desc_fp);
	desc_fp =  fopen(tab_file, "r");
	
	double tmp_rate;
	int tmp_TID;
	int tmp_QID;
	double tmp_quality;

	for (int i=0; i<num_rate_points; i++)
	{		
		fscanf(desc_fp, "%lf", 	&tmp_rate);
		fscanf(desc_fp, "%d", 	&tmp_TID);
		fscanf(desc_fp, "%d", 	&tmp_QID);
		fscanf(desc_fp, "%lf", 	&tmp_quality);
		fscanf(desc_fp,"%*[^\n]"); // read in rest of the line

		printf("Reading Preorder Points %f %d %d %f  ",tmp_rate,tmp_TID, tmp_QID, tmp_quality);

		rate_points_[i].rate = tmp_rate*1e3;   // in bps
		rate_points_[i].quality = tmp_quality;
		rate_points_[i].TID = tmp_TID;
		rate_points_[i].QID = tmp_QID;
		
		printf(" -- %d %d\n",rate_points_[i].TID, rate_points_[i].QID);

	}
	
	fclose(desc_fp);
	
	return true;
}


