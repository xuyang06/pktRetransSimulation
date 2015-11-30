#ifndef Event_Queue_Management_Version4_h
#define Event_Queue_Management_Version4_h

#include "networkFIFOqueue.h"
#include "videoPreordersvc.h"
#include "EventFIFOqueue.h"
#include "sendingWindow.h"
#include "sendingPktQueue.h"
#include "videoDecoder.h"
#include "RTTUpdate.h"
//#include <fstream>
//using std::fstream;

#include "rateID.h"
#include "unAckedQueue.h"
#include <string>
#include "io.h"
#include "direct.h"

class EventQueueManagementVersion4{
	private:
		EventFIFOqueue *eventFIFOqueue;
		NetworkFIFOqueue *networkFIFOqueue;
		VideoSVCPreorder *videosvcpreorder;
		SendingWindow *sendingWindow;
		SendingPktQueue *sendingPktQueue;
		VideoDecoder *videoDecoder;
		UnAckedQueue *unAckedQueue;
		FILE *f_status;
		FILE *f_frame_decode;
		FILE *f_status_time;
		FILE *f_status_rate;
		FILE *f_status_upper_rate;
		FILE *f_status_decoded_frame_num;
		FILE *f_status_final_result;
		FILE *f_trans_time;
		FILE *f_status_rate_decoded;
		FILE *f_status_frame_num_decoded;
		RTTUpdate rttUpdate;
		int RTT;
		double timeout;
		RateID rateID;
		int fecOn;
		int highestIID;

	public:
		~EventQueueManagementVersion4(){
			/*
			if (f_status != NULL) fclose(f_status);
			if (f_frame_decode != NULL) fclose(f_frame_decode);
			if (f_status_time != NULL) fclose(f_status_time);
			if (f_status_rate != NULL) fclose(f_status_rate);
			if (f_status_upper_rate != NULL) fclose(f_status_upper_rate);
			if (f_status_decoded_frame_num != NULL) fclose(f_status_decoded_frame_num);
			if (f_trans_time != NULL) fclose(f_trans_time);
			if (f_status_rate_decoded != NULL) fclose(f_status_rate_decoded);
			if (f_status_frame_num_decoded != NULL) fclose(f_status_frame_num_decoded);*/
		}
		/*
		fstream f_status;
		fstream f_frame_decode;
		fstream f_status_time;
		fstream f_status_rate;
		fstream f_status_upper_rate;
		fstream f_status_decoded_frame_num;
		fstream f_trans_time;
		fstream f_status_rate_decoded;
		fstream f_status_frame_num_decoded;
		*/
		EventQueueManagementVersion4(int round_trip_time, double time_out_duration, int a_queue_limit_, 
			int measuredBandwidth, double repeatTime, int frame_num_decoder, double deadline, int turnFECOn, float iFECRatio,
			float pFECRatio, string infor_base_directory, int fps, int  pktsize)
		{
			fecOn = 0;
			string f_status_string = infor_base_directory;
			f_status_string.append("\\result\\status.txt");
			const char *f_status_string_char = f_status_string.c_str();
			char * tag = new char[f_status_string.size() + 1];
			std::strcpy(tag, f_status_string_char);
			for(;*tag;tag++){
				if (*tag=='\\'){
					char buf[1000],path[1000];
					strcpy(buf,f_status_string_char);
					buf[strlen(f_status_string_char)-strlen(tag)+1]=NULL;
					strcpy(path,buf);
					if (access(path,6)==-1){
						mkdir(path);
					}
				}
			}
			f_status = fopen(f_status_string_char,"w");
			//f_status.open(f_status_string_char,ios::out);
			string f_frame_decode_string = infor_base_directory;
			f_frame_decode_string.append("\\result\\frame_decode.txt");
			string f_status_time_string = infor_base_directory;
			f_status_time_string.append("\\result\\status_time.txt");
			string f_status_rate_string = infor_base_directory;
			f_status_rate_string.append("\\result\\status_rate.txt");
			string f_status_rate_decoded_string = infor_base_directory;
			f_status_rate_decoded_string.append("\\result\\status_rate_decoded.txt");
			string f_status_upper_rate_string = infor_base_directory;
			f_status_upper_rate_string.append("\\result\\status_upper_rate.txt");
			string f_trans_time_string = infor_base_directory;
			f_trans_time_string.append("\\result\\trans_time.txt");
			string f_status_decoded_frame_num_string = infor_base_directory;
			f_status_decoded_frame_num_string.append("\\result\\status_decoded_frame_num.txt");
			string f_status_frame_num_decoded_string = infor_base_directory;
			f_status_frame_num_decoded_string.append("\\result\\status_frame_num_decoded.txt");
			string f_status_final_result_string = infor_base_directory;
			f_status_final_result_string.append("\\result\\status_final_result.txt");

			f_frame_decode = fopen(f_frame_decode_string.c_str(),"w");
			f_status_time = fopen(f_status_time_string.c_str(),"w");
			f_status_rate = fopen(f_status_rate_string.c_str(),"w");
			f_status_rate_decoded = fopen(f_status_rate_decoded_string.c_str(),"w");
			f_status_upper_rate = fopen(f_status_upper_rate_string.c_str(),"w");
			f_trans_time = fopen(f_trans_time_string.c_str(),"w");
			f_status_decoded_frame_num = fopen(f_status_decoded_frame_num_string.c_str(), "w");
			f_status_frame_num_decoded = fopen(f_status_frame_num_decoded_string.c_str(), "w");
			f_status_final_result = fopen(f_status_final_result_string.c_str(), "w");

			/*f_frame_decode.open(f_frame_decode_string.c_str(),ios::out);
			f_status_time.open(f_status_time_string.c_str(),ios::out);
			f_status_rate.open(f_status_rate_string.c_str(),ios::out);
			f_status_rate_decoded.open(f_status_rate_decoded_string.c_str(),ios::out);
			f_status_upper_rate.open(f_status_upper_rate_string.c_str(),ios::out);
			f_trans_time.open(f_trans_time_string.c_str(),ios::out);
			f_status_decoded_frame_num.open(f_status_decoded_frame_num_string.c_str(),ios::out);
			f_status_frame_num_decoded.open(f_status_frame_num_decoded_string.c_str(),ios::out);*/

			string rate_file_string = infor_base_directory;
			rate_file_string.append("\\data\\rate.txt");
			const char * rate_file = rate_file_string.c_str();
			FILE *desc_frate=NULL;
			desc_frate =  fopen(rate_file, "r");

			int temp_layer;
			double temp_rate;
			for(int i=0; i<3; i++){
				fscanf(desc_frate, "%d", 	&temp_layer);
				fscanf(desc_frate, "%f", 	&temp_rate);
				fscanf(desc_frate,"%*[^\n]");
				rateID.initialize_add_rateid_pair(temp_layer,temp_rate);
			}
			fclose(desc_frate);

			videoDecoder = new VideoDecoder(frame_num_decoder*repeatTime);
			eventFIFOqueue = new EventFIFOqueue();

			videosvcpreorder = new VideoSVCPreorder(fps, frame_num_decoder, pktsize);
			sendingWindow = new SendingWindow();
			sendingPktQueue = new SendingPktQueue();
			unAckedQueue = new UnAckedQueue();
			networkFIFOqueue = new NetworkFIFOqueue(round_trip_time, a_queue_limit_);
			//videosvcpreorder.init();
			RTT = round_trip_time + (int) ((1400*8)/measuredBandwidth);
			timeout = time_out_duration;
			//double transmissionDeadline = 200;
			double transmissionDeadline = deadline;
			//const char * pkt_file = "E:\\simulation\\foreman_VGA_GOP4_QP33_enc_pktData_CBR.dat";
			string pkt_file_string = infor_base_directory;
			const char * pkt_file = pkt_file_string.append("\\data\\pkt.txt").c_str();
			fecOn = turnFECOn;
			highestIID = 0; 
			if (fecOn == 1){
				videosvcpreorder->setFECOn();
				videosvcpreorder->setIFECRatio(iFECRatio);
				videosvcpreorder->setPFECRatio(pFECRatio);
				fprintf(f_status, "iFEC Ratio: %f\n", iFECRatio);
				fprintf(f_status, "pFEC Ratio: %f\n", pFECRatio);
				//f_status<<"iFEC Ratio: "<<iFECRatio<<endl;
				//f_status<<"pFEC Ratio: "<<pFECRatio<<endl;
			}
			videosvcpreorder->read_videopkt_trace(pkt_file, transmissionDeadline, eventFIFOqueue,timeout, videoDecoder,repeatTime, f_status);
			int a_totalWindowSize = ((measuredBandwidth*(RTT))/(1400*8)) + ((measuredBandwidth*(RTT))%(1400*8) ? 1 : 0);
			sendingWindow->initilize(a_totalWindowSize);
			rttUpdate = RTTUpdate(RTT*1.0);
			rateID.updateID(measuredBandwidth);
			fprintf(f_status, "\n");
			fprintf(f_status, "Experiment Setting: ............................begin\n");
			//f_status<<"Experiment Setting: ............................begin"<<endl;
			fprintf(f_status,"RTT: %d, Timeout: %f, transmissionDeadline: %f, a_totalWindowSize: %d. \n",RTT, timeout, transmissionDeadline, a_totalWindowSize);
			//f_status<<"RTT: "<<RTT<<", Timeout: "<<timeout<<", transmissionDeadline: "<<transmissionDeadline<<", a_totalWindowSize:"<<a_totalWindowSize<<"."<<endl;
			//f_status<<"Experiment Setting: ............................end"<<endl;
			fprintf(f_status, "Experiment Setting: ............................end\n");
			fprintf(f_status, "\n");
			//printf("Just out experiment setting. \n");
			//eventFIFOqueue->PrintBuf(f_status);	
			//printf("Just finish experiment setting. \n");
		}
/*
	TRANS_PKT,
	RECV_ACK,
	SEND_TIMEOUT,
	READY_PKT,
	RECV_PKT,
*/
		/*
		void setFECOn(){
			videosvcpreorder.setFECOn();
			videosvcpreorder.setIFECRatio(0.0);
			videosvcpreorder.setPFECRatio(0.0);
			fecOn = 1;
		}

		void setIFECRatio(float iFECRatio){
			videosvcpreorder.setIFECRatio(iFECRatio);
			f_status<<"iFEC Ratio: "<<iFECRatio<<endl;
		}

		void setPFECRatio(float pFECRatio){
			videosvcpreorder.setPFECRatio(pFECRatio);
			f_status<<"pFEC Ratio: "<<pFECRatio<<endl;
		}
		*/

		void setTransID(int id){
			rateID.setID(id);
		} 


		void setBandwidth(double a_bandwidth, double a_bandwidth_time)
		{
			networkFIFOqueue->setBandwidth(a_bandwidth, a_bandwidth_time);
			eventFIFOqueue->PrintBuf(f_status);
			//std::cout << "In setBandwidth ..."<<std::endl;
		}

		void setRandomLossProperty(double a_loss_probability)
		{
			networkFIFOqueue->setRandomLossProperty(a_loss_probability);
		}

		void setBurstyLossProperty(double a_loss_probability, int a_bursty_loss_pkt)
		{
			networkFIFOqueue->setBurstyLossProperty(a_loss_probability, a_bursty_loss_pkt);	
		}

		void setGilbertLossProperty(double a_p_unconditional, double a_p_conditional)
		{
			networkFIFOqueue->setGilbertLossProperty(a_p_unconditional, a_p_conditional);
			eventFIFOqueue->PrintBuf(f_status);
			//std::cout << "In Gilbert ..."<<std::endl;
		}

		void running()
		{
			//std::cout << "Begin to run ..."<<std::endl;
			//eventFIFOqueue->PrintBuf(f_status);
			//std::cout << "Come here ..."<<std::endl;
			while(eventFIFOqueue->isEmpty() != 1){
				//eventFIFOqueue->PrintBuf(f_status);
				eventFIFOqueue->PrintFIFOqueue(f_status);
				EventNode* now = eventFIFOqueue->pop();
				if (now->nodeType == TRANS_PKT){
					fprintf(f_status,"EventTime: %f, transmit successfully. Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						now->eventTime, now->pkt.frame, now->pkt.nslice, now->pkt.slice, now->pkt.enctime, now->pkt.dectime);
					//f_status<<"EventTime: "<<now->eventTime<<", transmit successfully. Frame ID: "<<now->pkt.frame<<",nslice: "<<now->pkt.nslice<<
					//	",slice: "<<now->pkt.slice<<",encoding time: "<<now->pkt.enctime<<",decoding time: "<<now->pkt.dectime<<endl;
					networkFIFOqueue->pop(eventFIFOqueue, now->eventTime, f_status);
					//eventFIFOqueue.PrintFIFOqueue(f_status);
					networkFIFOqueue->PrintFIFOqueue(f_status);
					sendingPktQueue->PrintFIFOqueue(f_status);
				}
				else if (now->nodeType == RECV_ACK){
					eventFIFOqueue->delTimeoutElement(now->pkt);
					if ( now->pkt.sendtime >= 0){
						rttUpdate.updateRTT( (now->eventTime - now->pkt.sendtime) );
						fprintf(f_status, "received RTT = %f ms, current RTT = %f ms. \n", (now->eventTime - now->pkt.sendtime), rttUpdate.getCurrentRTT());
						//f_status<<"received RTT = "<<(now->eventTime - now->pkt.sendtime)<<", current RTT = "<<rttUpdate.getCurrentRTT()<<"ms."<<endl;
					}
					sendingWindow->receiveOneAck();
					fprintf(f_status, "EventTime: %f, recv ack. Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
						now->eventTime, now->pkt.frame, now->pkt.nslice, now->pkt.slice, now->pkt.enctime, now->pkt.dectime);
					//f_status<<"EventTime: "<<now->eventTime<<", recv ack. Frame ID: "<<now->pkt.frame<<",nslice: "<<now->pkt.nslice<<
						//",slice: "<<now->pkt.slice<<",encoding time: "<<now->pkt.enctime<<",decoding time: "<<now->pkt.dectime<<endl;
					unAckedQueue->PrintFIFOqueue(f_status);
					vector<videoPkt> unackedPkts = unAckedQueue->getUnAckedVideoPkts( now->pkt, f_status);
					int unackedPktLen = unackedPkts.size();
					int unackedPktIndex = 0;
					int unackedPktIndex2 = 0;
					while ( unackedPktIndex2 < unackedPktLen){
						sendingWindow->receiveOneAck();	
						unackedPktIndex2 = unackedPktIndex2 + 1;
					}

					if ( fecOn == 0 ){
						if ( unackedPktLen != 0 ){
							while( ( sendingWindow->getAvaiableWindowSize() > 0 ) && ( unackedPktIndex < unackedPktLen ) ){
								videoPkt video_pkt = unackedPkts[unackedPktIndex];
								if ( ( rateID.getSelectedID() < video_pkt.TID ) ||  ( (( video_pkt.dectime - now->eventTime ) <= 
									( rttUpdate.getCurrentRTT()/2 )) && ((video_pkt.TID != 0) || 
									( (video_pkt.TID == 0) && ( video_pkt.frame < highestIID))) ) ) {
										eventFIFOqueue->delTimeoutElement(video_pkt);
										fprintf(f_status, "Del Timeout Element for unAcked Video Pkts, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											video_pkt.frame, video_pkt.nslice, video_pkt.slice, video_pkt.enctime, video_pkt.dectime);
										//f_status<<"Del Timeout Element for unAcked Video Pkts, Frame ID: "<<video_pkt.frame<<",nslice: "<<video_pkt.nslice<<
											//",slice: "<<video_pkt.slice<<",encoding time: "<<video_pkt.enctime<<",decoding time: "<<video_pkt.dectime<<endl;
										unackedPktIndex = unackedPktIndex + 1;
								}else{
									eventFIFOqueue->delTimeoutElement(video_pkt);
									video_pkt.sendtime = now->eventTime;
									fprintf(f_status, "Add Pkt to Network FIFO queue, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											video_pkt.frame, video_pkt.nslice, video_pkt.slice, video_pkt.enctime, video_pkt.dectime);
									//f_status<<"Add Pkt to Network FIFO queue, Frame ID: "<<video_pkt.frame<<",nslice: "<<video_pkt.nslice<<
										//	",slice: "<<video_pkt.slice<<",encoding time: "<<video_pkt.enctime<<",decoding time: "<<video_pkt.dectime<<endl;
									unAckedQueue->push(video_pkt);
									networkFIFOqueue->push(video_pkt,eventFIFOqueue,now->eventTime);
									eventFIFOqueue->push(SEND_TIMEOUT, video_pkt, now->eventTime + timeout);
									unackedPktIndex = unackedPktIndex + 1;
									sendingWindow->sendOnePkt();
								}
							}
							if (  ( sendingWindow->getAvaiableWindowSize() == 0 ) && ( unackedPktIndex < unackedPktLen ) ){
								while ( unackedPktIndex < unackedPktLen ){
									videoPkt video_pkt = unackedPkts[unackedPktIndex];
									if ( ( rateID.getSelectedID() < video_pkt.TID ) ||  ( (( video_pkt.dectime - now->eventTime ) <= 
									( rttUpdate.getCurrentRTT()/2 )) && ((video_pkt.TID != 0) || 
									( (video_pkt.TID == 0) && ( video_pkt.frame < highestIID))) ) ){
											eventFIFOqueue->delTimeoutElement(video_pkt);
											fprintf(f_status, "Del Timeout Element for unAcked Video Pkts, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											video_pkt.frame, video_pkt.nslice, video_pkt.slice, video_pkt.enctime, video_pkt.dectime);

											//f_status<<"Del Timeout Element for unAcked Video Pkts, Frame ID: "<<video_pkt.frame<<",nslice: "<<video_pkt.nslice<<
											//",slice: "<<video_pkt.slice<<",encoding time: "<<video_pkt.enctime<<",decoding time: "<<video_pkt.dectime<<endl;
											unackedPktIndex = unackedPktIndex + 1;
									}else{
										sendingPktQueue->addToHeader(video_pkt);
										fprintf(f_status, "Add pkt to the header of Sending Pkt Queue, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											video_pkt.frame, video_pkt.nslice, video_pkt.slice, video_pkt.enctime, video_pkt.dectime);

										//f_status<<"Add pkt to the header of Sending Pkt Queue, Frame ID: "<<video_pkt.frame<<",nslice: "<<video_pkt.nslice<<
											//",slice: "<<video_pkt.slice<<",encoding time: "<<video_pkt.enctime<<",decoding time: "<<video_pkt.dectime<<endl;
										unackedPktIndex = unackedPktIndex + 1;
									}		
								}
							}
						}

					}
					
					if ( (sendingPktQueue->isEmpty() == 0) && (sendingWindow->getAvaiableWindowSize() > 0) ){
						videoPktNode* pktNode =  sendingPktQueue->pop();
						videoPkt pkt = pktNode->pkt_;
						int TID = pkt.TID;
						double decTime = pkt.dectime;
						double nowTime = now->eventTime;

						while ( sendingWindow->getAvaiableWindowSize() > 0 ){
							if ( ( rateID.getSelectedID() < pkt.TID ) ||  ( (( pkt.dectime - now->eventTime ) <= 
									( rttUpdate.getCurrentRTT()/2 )) && ((pkt.TID != 0) || 
									( (pkt.TID == 0) && ( pkt.frame < highestIID))) ) ){
								fprintf(f_status, "Del Pkt, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											pkt.frame, pkt.nslice, pkt.slice, pkt.enctime, pkt.dectime);
								//f_status<<"Del Pkt, Frame ID:"<<pkt.frame<<", Encoding Time:"<<pkt.enctime<<", Decoding Time:"<<
								//pkt.dectime<<", nslice:"<<pkt.nslice<<", slice:"<<pkt.slice<<endl;
								if( (sendingPktQueue->isEmpty() == 0) && ( sendingWindow->getAvaiableWindowSize() > 0 ) ){
									pktNode = sendingPktQueue->pop();
									pkt = pktNode->pkt_;
									TID = pkt.TID;
									decTime = pkt.dectime;
								}else{
									break;
								}	
							}else if ( ( sendingWindow->getAvaiableWindowSize() > 0 ) && ( rateID.getSelectedID() >= pkt.TID ) ){
								pkt.sendtime = now->eventTime;
								fprintf(f_status, "Add Pkt to Network FIFO queue, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											pkt.frame, pkt.nslice, pkt.slice, pkt.enctime, pkt.dectime);
								//f_status<<"Add Pkt to Network FIFO queue, Frame ID: "<<pkt.frame<<",nslice: "<<pkt.nslice<<
									//		",slice: "<<pkt.slice<<",encoding time: "<<pkt.enctime<<",decoding time: "<<pkt.dectime<<endl;
								unAckedQueue->push(pkt);
								networkFIFOqueue->push(pkt, eventFIFOqueue, now->eventTime);
								eventFIFOqueue->push(SEND_TIMEOUT, pkt, now->eventTime + timeout);
								sendingWindow->sendOnePkt();
								if( ( sendingPktQueue->isEmpty() == 0) && ( sendingWindow->getAvaiableWindowSize() > 0 ) ){
									pktNode = sendingPktQueue->pop();
									pkt = pktNode->pkt_;
									TID = pkt.TID;
									decTime = pkt.dectime;
								}else{
									break;
								}
	
							}else{
								break;
							}
						}
					}
					//eventFIFOqueue.PrintFIFOqueue(f_status);
					unAckedQueue->PrintFIFOqueue(f_status);
					networkFIFOqueue->PrintFIFOqueue(f_status);
					sendingPktQueue->PrintFIFOqueue(f_status);
				}else if (now->nodeType == SEND_TIMEOUT){
					fprintf(f_status, "EventTime: %f, send time out. Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											now->eventTime, now->pkt.frame, now->pkt.nslice, now->pkt.slice, now->pkt.enctime, now->pkt.dectime);

					//f_status<<"EventTime: "<<now->eventTime<<", send time out. Frame ID: "<<now->pkt.frame<<",nslice: "<<now->pkt.nslice<<
						//",slice: "<<now->pkt.slice<<",encoding time: "<<now->pkt.enctime<<",decoding time: "<<now->pkt.dectime<<endl;
					sendingWindow->receiveOneAck();
					unAckedQueue->del(now->pkt);
					unAckedQueue->PrintFIFOqueue(f_status);
					now->pkt.trans_num = now->pkt.trans_num + 1;

					if ( fecOn == 0 ){
						if (now->pkt.TID == 0){
							if ( sendingWindow->getAvaiableWindowSize() == 0){
								sendingPktQueue->addToHeader(now->pkt);
							}else{
								videoPkt pkt = now->pkt;
								sendingWindow->sendOnePkt();
								unAckedQueue->push(pkt);
								networkFIFOqueue->push(pkt,eventFIFOqueue,now->eventTime);
								eventFIFOqueue->push(SEND_TIMEOUT, pkt, now->eventTime + timeout);
							}	
						}
					}

					while ( (sendingWindow->getAvaiableWindowSize() != 0) && ( sendingPktQueue->isEmpty() == 0) ){
						videoPkt pkt = sendingPktQueue->pop()->pkt_;
						sendingWindow->sendOnePkt();
						unAckedQueue->push(pkt);
						networkFIFOqueue->push(pkt,eventFIFOqueue,now->eventTime);
						eventFIFOqueue->push(SEND_TIMEOUT, pkt, now->eventTime + timeout);
						fprintf(f_status, "Add Pkt to Network FIFO queue, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											pkt.frame, pkt.nslice, pkt.slice, pkt.enctime, pkt.dectime);
						//f_status<<"Add Pkt to Network FIFO queue, Frame ID: "<<pkt.frame<<",nslice: "<<pkt.nslice<<
						//",slice: "<<pkt.slice<<",encoding time: "<<pkt.enctime<<",decoding time: "<<pkt.dectime<<endl;
								
					}
					

					unAckedQueue->PrintFIFOqueue(f_status);
					//eventFIFOqueue.PrintFIFOqueue(f_status);
					networkFIFOqueue->PrintFIFOqueue(f_status);
					sendingPktQueue->PrintFIFOqueue(f_status);
				}else if (now->nodeType == READY_PKT){
					fprintf(f_status, "EventTime: %f, ready pkt. Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											now->eventTime, now->pkt.frame, now->pkt.nslice, now->pkt.slice, now->pkt.enctime, now->pkt.dectime);

					//f_status<<"EventTime: "<<now->eventTime<<", ready pkt. Frame ID: "<<now->pkt.frame<<",nslice: "<<now->pkt.nslice<<
						//",slice: "<<now->pkt.slice<<",encoding time: "<<now->pkt.enctime<<",decoding time: "<<now->pkt.dectime<<endl;
					FrameType frame_type = now->pkt.frame_type;
					if ( (frame_type == I) && (now->pkt.frame > highestIID) ){
						highestIID = now->pkt.frame;
					}
					int TID = now->pkt.TID;
					double decTime = now->pkt.dectime;
					double nowTime = now->eventTime;
					videoPkt pkt = now->pkt;
					if ( (( now->pkt.frame < highestIID ) && ( ( decTime - nowTime ) <= (rttUpdate.getCurrentRTT()/2))) || ( rateID.getSelectedID() < pkt.TID ) ){
						//f_status<<"Decide not to send that pkt"<<endl;
						fprintf(f_status, "Decide not to send that pkt\n");
					}else{
						if(sendingWindow->getAvaiableWindowSize() != 0){
							now->pkt.sendtime = now->eventTime;
							sendingWindow->sendOnePkt();

							unAckedQueue->PrintFIFOqueue(f_status);
							unAckedQueue->push(now->pkt);


							networkFIFOqueue->push(pkt,eventFIFOqueue,now->eventTime);
							eventFIFOqueue->push(SEND_TIMEOUT, pkt, now->eventTime + timeout);
						}else{
							sendingPktQueue->push(pkt);
						}
					}
					//eventFIFOqueue.PrintFIFOqueue(f_status);
					unAckedQueue->PrintFIFOqueue(f_status);
					networkFIFOqueue->PrintFIFOqueue(f_status);
					sendingPktQueue->PrintFIFOqueue(f_status);
				}else if (now->nodeType == RECV_PKT){
					videoPkt pkt = now->pkt;
					fprintf(f_status, "EventTime: %f, recv pkt. Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											now->eventTime, now->pkt.frame, now->pkt.nslice, now->pkt.slice, now->pkt.enctime, now->pkt.dectime);

					//f_status<<"EventTime: "<<now->eventTime<<", recv pkt. Frame ID: "<<now->pkt.frame<<",nslice: "<<now->pkt.nslice<<
						//",slice: "<<now->pkt.slice<<",encoding time: "<<now->pkt.enctime<<",decoding time: "<<now->pkt.dectime<<endl;
					videoDecoder->receivePkt(pkt,f_status);
					//eventFIFOqueue.PrintFIFOqueue(f_status);
					networkFIFOqueue->PrintFIFOqueue(f_status);
					sendingPktQueue->PrintFIFOqueue(f_status);
				}else if (now->nodeType == FRAME_PLAY){
					fprintf(f_status, "EventTime: %f, frame play. Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f \n",
											now->eventTime, now->pkt.frame, now->pkt.nslice, now->pkt.slice, now->pkt.enctime, now->pkt.dectime);
					//f_status<<"EventTime: "<<now->eventTime<<", frame play. Frame ID: "<<now->pkt.frame<<",nslice: "<<now->pkt.nslice<<
						//",slice: "<<now->pkt.slice<<",encoding time: "<<now->pkt.enctime<<",decoding time: "<<now->pkt.dectime<<endl;
					int i = now->pkt.frame;
					videoDecoder->checkDecodedFrame(i,f_status);
					//eventFIFOqueue.PrintFIFOqueue(f_status);
					networkFIFOqueue->PrintFIFOqueue(f_status);
					sendingPktQueue->PrintFIFOqueue(f_status);
				}
			}
			videoDecoder->calculate(f_trans_time, f_status_final_result);
			videoDecoder->PrintFrameDecodeInfo(f_frame_decode);
			videoDecoder->PrintTimeRateFrameInfo( f_status_time, f_status_rate, f_status_rate_decoded, f_status_decoded_frame_num, f_status_frame_num_decoded, f_status_upper_rate);
			fclose(f_frame_decode);
			fclose(f_status_time);
			fclose(f_status_rate);
			fclose(f_status_decoded_frame_num);
			fclose(f_status);
			fclose(f_trans_time);
			fclose(f_status_final_result);
			//f_frame_decode.close();
			//f_status_time.close();
			//f_status_rate.close();
			//f_status_decoded_frame_num.close();
			//f_status.close();
			//f_trans_time.close();

		}

		

};


#endif