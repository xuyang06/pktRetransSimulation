#ifndef network_FIFOqueue_h
#define network_FIFOqueue_h

#include "videopkt.h"
#include "channelProperty.h"
#include <stdlib.h>
#include <time.h>
#include "EventFIFOqueue.h"
#include <fstream>

struct networkQueueNode {
	videoPkt pkt_;
	networkQueueNode* next_;
};


class NetworkFIFOqueue {
	private:
		int queue_limit_;
		ChannelProperty *channel;
		networkQueueNode *buf_; // the underlying buffer
		int burstyTotal;
		int burstyOn;
		int burstyNow;
		int GilbertLossOn;
		double p_prob;
		double q_prob;
		int size_;
		
	public:
		NetworkFIFOqueue()
		{
			buf_ = NULL;
			size_ = 0;
			//srand(time(NULL));
			srand((unsigned)time(0));
		}


		void PrintFIFOqueue( FILE *f_status)
		{
			fprintf(f_status,"\n");
			fprintf(f_status,"Begin to print network queue status ... \n");
			//f_status<<endl;
			//f_status<<"Begin to print network queue status ... "<<endl;
			networkQueueNode *temp = buf_;
			while ( temp != NULL){
				//printf("Enter\n");
				videoPkt pktTemp = temp->pkt_;
				//double eventTimeTemp = temp->eventTime;
				/*printf("Queue Info: %d, encoding time %f, decoding time %f, nslice %d, slice %d, pktsize %d, type %d, eventTime %f...\n", 
					pktTemp.frame, pktTemp.enctime, pktTemp.dectime, pktTemp.nslice, pktTemp.slice, 
					pktTemp.pktsize, typeTemp, eventTimeTemp);*/
				fprintf(f_status,"Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f, Pktsize: %d. \n",
						pktTemp.frame, pktTemp.nslice, pktTemp.slice, pktTemp.enctime, pktTemp.dectime, pktTemp.pktsize);
				//f_status<<"Frame ID: "<<pktTemp.frame <<",Encoding time: "<<pktTemp.enctime<<",Decoding time: "<<pktTemp.dectime <<
					//",Nslice: " <<pktTemp.nslice<<",Slice: "<<pktTemp.slice<<",Pktsize: "<<pktTemp.pktsize<<",Type: "<<pktTemp.frame_type<<
					//"." <<endl;
				temp = temp->next_;
			}
			//f_status<<"End to print network queue status ... "<<endl;
			//f_status<<endl;
			fprintf(f_status,"End to print network queue status ... \n");
			fprintf(f_status,"\n");
			

		}


		NetworkFIFOqueue(double round_trip_time, int a_queue_limit_)
		{
				channel = new ChannelProperty(round_trip_time);
				buf_ = NULL;
				size_ = 0;
				queue_limit_ = a_queue_limit_;
		}

		void setBandwidth(double a_bandwidth, double a_bandwidth_time)
		{
			channel->setBandwidth(a_bandwidth, a_bandwidth_time);
		}

		void setRandomLossProperty(double a_loss_probability)
		{
			channel->setRandomLossProperty(a_loss_probability);
		}

		void setBurstyLossProperty(double a_loss_probability, int a_bursty_loss_pkt)
		{
			burstyOn = 0;
			channel->setBurstyLossProperty(a_loss_probability, a_bursty_loss_pkt);
			burstyTotal = a_bursty_loss_pkt;
		}

		void setGilbertLossProperty(double a_p_unconditional, double a_p_conditional)
		{
			GilbertLossOn = 0;
			channel->setGilbertBurstyLossProperty(a_p_unconditional, a_p_conditional);
			p_prob = channel->get_p_prob_Gilbert();
			q_prob = channel->get_q_prob_Gilbert();
		}

		void PrintChannelInfo()
		{
			channel->PrintChannelInfo();
		} 

		~NetworkFIFOqueue()
			{
				printf("delete variables in NetworkFIFOqueue.\n");
				networkQueueNode *p;

				if ( buf_ == NULL )
					return;

				while ( buf_ != NULL ) {
					p = buf_->next_;
					delete buf_;
					buf_ = p;
				}
			}

		void clearQ()
			{
				networkQueueNode *p;

				if ( buf_ == NULL )
					return;

				while ( buf_ != NULL ) {
					p = buf_->next_;
					delete buf_;
					buf_ = p;
				}
				size_ = 0;
			}

		int size()
		{
			return size_;
		}

		double getTransmitTime(videoPkt pkt, double initialTime)
		{
			int index = 0;
			int bandwidthSize = channel->getBandwidthListSize();
			for(int i = bandwidthSize-1; i >=0; i--)
			{
				if((channel->getBandwidthTime(i))<= initialTime)
				{
					index = i;
					break;
				}
			}
			double nowTime = initialTime;
			double residual = pkt.pktsize;
			int nowIndex = index;
			while ( nowIndex <= (bandwidthSize-1) ){
				double needTime = (residual*8.0)/(channel->getBandwidthValue(nowIndex));
				if ( nowIndex < (bandwidthSize-1) ){
					if ( ((channel->getBandwidthTime(nowIndex+1)) - nowTime) >= needTime ){
						double transmitTime = nowTime + needTime;
						return transmitTime;
					}else{
						residual = residual - ((channel->getBandwidthTime(nowIndex+1)) - nowTime)*(channel->getBandwidthValue(nowIndex));
						nowIndex = nowIndex+1;
						nowTime = channel->getBandwidthTime(nowIndex);
					}
				}else{
					double transmitTime = nowTime + needTime;
					return transmitTime;
				}	
			}
		}

		void push(videoPkt pkt, EventFIFOqueue *eventFIFOqueue, double eventTime)
		{
				if (size_ == queue_limit_){
					return;
				}
				networkQueueNode *p, *t;

				if (buf_ == NULL)
				{
					buf_ = new networkQueueNode;
					buf_->pkt_ = pkt;
					buf_->next_ = NULL;
					double transmitTime = getTransmitTime(pkt, eventTime);
					eventFIFOqueue->push(TRANS_PKT, pkt, transmitTime);

				} else {
					p = buf_;
					while ( p->next_ != NULL )
						p = p->next_;

					t = new networkQueueNode;
					t->pkt_ = pkt;
					t->next_ = NULL;

					p->next_ = t;
				}
				size_ ++;
		}

		bool isEmpty()
			{
				return (size_ == 0);
			}

		void pop(EventFIFOqueue *eventFIFOqueue, double eventTime,  FILE *f_status)
		{
				
			double roundtrip_time = channel->getRoundTripTime();
			networkQueueNode *p;

				if (size_ == 0)
				{
					return;		
				}else{
					p = buf_;
					buf_ = buf_->next_;
					size_ --;
					
				}
				
				if (buf_ != NULL){
					double transmitTime = getTransmitTime(buf_->pkt_, eventTime);
					eventFIFOqueue->push(TRANS_PKT, buf_->pkt_, transmitTime);
				}
				//double transmitTime = getTransmitTime(p->pkt_, eventTime);
				//eventFIFOqueue.push(TRANS_PKT, pkt, transmitTime);

				if (channel->getLossType() == Rand_loss){
					fprintf(f_status, "Check Random Loss .................\n");
					//f_status<<"Check Random Loss ................."<<endl;
					//srand(time(NULL));
					double lossProb = channel->getLossProbability();
					int rnd = rand()%100;
					fprintf(f_status, "rnd: %d, lossProb: %f\n", rnd, lossProb);
					//f_status<<"rnd: "<<rnd<<", lossProb: "<<lossProb<<endl;
					if (rnd < (lossProb*100)){
						fprintf(f_status,"Random Loss: Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						 p->pkt_.frame, p->pkt_.nslice, p->pkt_.slice, p->pkt_.enctime, p->pkt_.dectime);
						//f_status<<"Random Loss: Frame ID:"<<p->pkt_.frame<<", Nslices:"<<p->pkt_.nslice<<", slice:"<<p->pkt_.slice
							//<<", encodingTime:"<<p->pkt_.enctime<<", decodingTime:"<<p->pkt_.dectime<<endl;
						return;
					}
				}

				if (channel->getLossType() == Bursty_loss){
					if (burstyOn == 1){
						if (burstyNow < burstyTotal){
							burstyNow = burstyNow + 1;
							fprintf(f_status,"Bursty Loss: Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						 p->pkt_.frame, p->pkt_.nslice, p->pkt_.slice, p->pkt_.enctime, p->pkt_.dectime);
							//f_status<<"Bursty Loss: Frame ID:"<<p->pkt_.frame<<", Nslices:"<<p->pkt_.nslice<<", slice:"<<p->pkt_.slice
							//<<", encodingTime:"<<p->pkt_.enctime<<", decodingTime:"<<p->pkt_.dectime<<endl;
							return;
						}else{
							burstyOn = 0;
						}
					}
					
					if (burstyOn == 0){
						//srand((int)time(0));
						//srand(time(NULL));
						double lossProb = channel->getLossProbability();
						int rnd = rand()%100;
						fprintf(f_status, "rnd: %d, lossProb: %f\n", rnd, lossProb);
						//f_status<<"rnd: "<<rnd<<", lossProb: "<<lossProb<<endl;
						if (rnd < (lossProb*100)){
							burstyOn = 1;
							burstyNow = 1;
							fprintf(f_status,"Bursty Loss: Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						 p->pkt_.frame, p->pkt_.nslice, p->pkt_.slice, p->pkt_.enctime, p->pkt_.dectime);
							//f_status<<"Bursty Loss: Frame ID:"<<p->pkt_.frame<<", Nslices:"<<p->pkt_.nslice<<", slice:"<<p->pkt_.slice
							//<<", encodingTime:"<<p->pkt_.enctime<<", decodingTime:"<<p->pkt_.dectime<<endl;
							return;
						}
					}
				}

				if(channel->getLossType() == Bursty_Gilbert_Model_loss){
					fprintf(f_status, "GilbertLossProperty: p_prob -  %f, q_prob - %f\n", p_prob, q_prob);
					//f_status<<"GilbertLossProperty: p_prob - "<<p_prob<<", q_prob - "<<q_prob<<endl;
					if(GilbertLossOn ==0){
						int rnd = rand()%100;
						//f_status<<"rnd: "<<rnd<<endl;
						fprintf(f_status, "rnd: %d\n", rnd);
						if ( rnd < (p_prob*100) ){
							GilbertLossOn = 1;
							fprintf(f_status,"Gilbert Bursty Loss: 0 -> 1 : Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						 p->pkt_.frame, p->pkt_.nslice, p->pkt_.slice, p->pkt_.enctime, p->pkt_.dectime);
							//f_status<<"Gilbert Bursty Loss: 0 -> 1 : Frame ID:"<<p->pkt_.frame<<", Nslices:"<<p->pkt_.nslice<<", slice:"<<p->pkt_.slice
							//<<", encodingTime:"<<p->pkt_.enctime<<", decodingTime:"<<p->pkt_.dectime<<endl;
							return;
						}else{
							fprintf(f_status,"Gilbert Bursty Loss: 0 -> 0 : Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						 p->pkt_.frame, p->pkt_.nslice, p->pkt_.slice, p->pkt_.enctime, p->pkt_.dectime);
							//f_status<<"Gilbert Bursty Loss: 0 -> 0 : Frame ID:"<<p->pkt_.frame<<", Nslices:"<<p->pkt_.nslice<<", slice:"<<p->pkt_.slice
							//<<", encodingTime:"<<p->pkt_.enctime<<", decodingTime:"<<p->pkt_.dectime<<endl;
						}
					}else{
						int rnd = rand()%100;
						//f_status<<"rnd: "<<rnd<<endl;
						fprintf(f_status, "rnd: %d\n", rnd);
						if ( rnd < (q_prob*100) ){
							GilbertLossOn = 0;
							fprintf(f_status,"Gilbert Bursty Loss: 1 -> 0 : Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						 p->pkt_.frame, p->pkt_.nslice, p->pkt_.slice, p->pkt_.enctime, p->pkt_.dectime);
							//f_status<<"Gilbert Bursty Loss: 1 -> 0 : Frame ID:"<<p->pkt_.frame<<", Nslices:"<<p->pkt_.nslice<<", slice:"<<p->pkt_.slice
							//<<", encodingTime:"<<p->pkt_.enctime<<", decodingTime:"<<p->pkt_.dectime<<endl;
						}else{
							GilbertLossOn = 1;
							fprintf(f_status,"Gilbert Bursty Loss: 1 -> 1 : Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						 p->pkt_.frame, p->pkt_.nslice, p->pkt_.slice, p->pkt_.enctime, p->pkt_.dectime);
							//f_status<<"Gilbert Bursty Loss: 1 -> 1 : Frame ID:"<<p->pkt_.frame<<", Nslices:"<<p->pkt_.nslice<<", slice:"<<p->pkt_.slice
							//<<", encodingTime:"<<p->pkt_.enctime<<", decodingTime:"<<p->pkt_.dectime<<endl;
							return;
						}
					}
				
				}

				fprintf(f_status,"No Loss: 1 -> 1 : Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						 p->pkt_.frame, p->pkt_.nslice, p->pkt_.slice, p->pkt_.enctime, p->pkt_.dectime);
				//f_status<<"No Loss: Frame ID:"<<p->pkt_.frame<<", Nslices:"<<p->pkt_.nslice<<", slice:"<<p->pkt_.slice
					//		<<", encodingTime:"<<p->pkt_.enctime<<", decodingTime:"<<p->pkt_.dectime<<endl;
				eventFIFOqueue->push(RECV_PKT, p->pkt_, eventTime+roundtrip_time*0.5);
				eventFIFOqueue->push(RECV_ACK, p->pkt_, eventTime+roundtrip_time);


		}
};


#endif