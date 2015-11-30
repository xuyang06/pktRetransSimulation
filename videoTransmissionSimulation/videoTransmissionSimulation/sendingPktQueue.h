#ifndef sending_pkt_queue_h
#define sending_pkt_queue_h

#include "videopkt.h"
#include <stdlib.h>
#include <time.h>


struct videoPktNode {
	videoPkt pkt_;
	videoPktNode* next_;
};

class SendingPktQueue{
	private:
		videoPktNode *buf_; // the underlying buffer
		int size_;


	public:
		SendingPktQueue()
		{
			buf_ = NULL;
			size_ = 0;
		}

		~SendingPktQueue(){
			printf("delete variables in SendingPktQueue.\n");
		}

		void PrintFIFOqueue( FILE *f_status)
		{
			fprintf(f_status,"\n");
			fprintf(f_status,"Begin to print sending pkt queue status ... \n");
			//f_status<<endl;
			//f_status<<"Begin to print sending pkt queue status ... "<<endl;
			videoPktNode *temp = buf_;
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
			fprintf(f_status,"End to print sending pkt queue status ... \n");
			fprintf(f_status,"\n");
			//f_status<<"End to print sending pkt queue status ... "<<endl;
			//f_status<<endl;
		}


		void clearQ()
			{
				videoPktNode *p;

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


		void push(videoPkt pkt)
		{
				videoPktNode *p, *t;

				if (buf_ == NULL)
				{
					buf_ = new videoPktNode;
					buf_->pkt_ = pkt;
					buf_->next_ = NULL;
				} else {
					p = buf_;
					while ( p->next_ != NULL )
						p = p->next_;

					t = new videoPktNode;
					t->pkt_ = pkt;
					t->next_ = NULL;

					p->next_ = t;
				}
				size_ ++;
		}


		int compare(videoPkt pkt, videoPktNode *p){
			double pkt_enctime = pkt.enctime;
			int pkt_slice = pkt.slice; 
			
			double p_pkt_enctime = p->pkt_.enctime;
			int p_pkt_slice = p->pkt_.slice;  

			if (( pkt_enctime <= p_pkt_enctime) && (pkt_slice <= p_pkt_slice)){
				return 1;
			}else if (( pkt_enctime == p_pkt_enctime) && (pkt_slice == p_pkt_slice)){
				return 0;
			}else
				return -1;

		}

		void addToHeader(videoPkt pkt)
		{
			videoPktNode *pre, *now;

			pre = buf_;
			now = buf_;
			int change = 1;
				if (buf_ == NULL)
				{
					buf_ = new videoPktNode;
					buf_->pkt_ = pkt;
					buf_->next_ = NULL;
				} else {
					if ( compare(pkt, buf_) == 1){
						pre = new videoPktNode;
						pre->pkt_ = pkt;
						pre->next_ = buf_;
						buf_ = pre;
					}else if ( compare(pkt, buf_) == 0){
						change = 0;
					}
					else{
						while (now->next_ != NULL){
							pre = now;
							now = now->next_;
							if (compare(pkt, now) == 1){
								videoPktNode *inter = new videoPktNode;
								inter->pkt_ = pkt;
								inter->next_ = now;
								pre->next_ = inter;
								break;
							}else if ( compare(pkt, buf_) == 0){
								change = 0;
								break;
							}
						}
						if ( now->next_ == NULL){
							videoPktNode *inter = new videoPktNode;
							inter->pkt_ = pkt;
							inter->next_ = NULL;
							now->next_ = inter;
						}
					}
				}
				if (change == 1){
					size_ ++;
				}
		}

		bool isEmpty()
		{
			return (size_ == 0);
		}


		videoPktNode *pop()
		{
				videoPktNode *p;

				if (size_ == 0)
				{
					return NULL;
		
				}else{

					p = buf_;
					buf_ = buf_->next_;
					size_ --;
					return p;
					
				}

		}

};



#endif