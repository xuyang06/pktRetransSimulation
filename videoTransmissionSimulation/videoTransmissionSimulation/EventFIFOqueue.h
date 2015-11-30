#ifndef EventFIFOqueue_h
#define EventFIFOqueue_h

#include "channelProperty.h"
#include "eventNode.h"
#include <fstream>
class EventFIFOqueue {
	private:
		EventNode *buf_; // the underlying buffer
		int size_;
		
	public:
		void PrintBuf(FILE *f_status){
			fprintf(f_status, "\n");
			fprintf(f_status, "For buf_:  EventTime: %f, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f, Pktsize: %d. Size: %d\n",
				buf_->eventTime, buf_->pkt.frame, buf_->pkt.nslice, buf_->pkt.slice, buf_->pkt.enctime, buf_->pkt.dectime, buf_->pkt.pktsize, size_);
			fprintf(f_status, "\n");
		}

		void PrintFIFOqueue( FILE *f_status)
		{
			fprintf(f_status, "print event queue situation .... begin\n");
			//f_status<<"print event queue situation .... begin"<<endl;
			EventNode *temp = buf_;
			fprintf(f_status, "For buf_:  EventTime: %f, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f, Pktsize: %d. \n",
				buf_->eventTime, buf_->pkt.frame, buf_->pkt.nslice, buf_->pkt.slice, buf_->pkt.enctime, buf_->pkt.dectime, buf_->pkt.pktsize);

			while ( temp != NULL){
				//printf("Enter\n");
				eventType typeTemp = temp->nodeType;
				videoPkt pktTemp = temp->pkt;
				double eventTimeTemp = temp->eventTime;
				/*printf("Queue Info: %d, encoding time %f, decoding time %f, nslice %d, slice %d, pktsize %d, type %d, eventTime %f...\n", 
					pktTemp.frame, pktTemp.enctime, pktTemp.dectime, pktTemp.nslice, pktTemp.slice, 
					pktTemp.pktsize, typeTemp, eventTimeTemp);*/
				fprintf(f_status,"EventTime: %f, Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f, Pktsize: %d. \n",
						eventTimeTemp, pktTemp.frame, pktTemp.nslice, pktTemp.slice, pktTemp.enctime, pktTemp.dectime, pktTemp.pktsize);

				//f_status<<"Frame ID: "<<pktTemp.frame <<",Encoding time: "<<pktTemp.enctime<<",Decoding time: "<<pktTemp.dectime <<
					//",Nslice: " <<pktTemp.nslice<<",Slice: "<<pktTemp.slice<<",Pktsize: "<<pktTemp.pktsize<<",Type: "<<typeTemp<<
					//",EventTime: "<<eventTimeTemp<<"." <<endl;
				temp = temp->next_;
			}
			fprintf(f_status, "print event queue situation .... end \n");
			//f_status<<"print event queue situation .... end"<<endl;
			
		}


		EventFIFOqueue()
			{
				buf_ = NULL;
				size_ = 0;
			}

		
		~EventFIFOqueue()
			{
				
				printf("delete variables in EventFIFOqueue.\n");

				EventNode *p;

				if ( buf_ == NULL )
					return;

				while ( buf_ != NULL ) {
					p = buf_->next_;
					delete buf_;
					buf_ = p;
				}
				
			}

		/*void clearQ()
			{
				EventNode *p;

				if ( buf_ == NULL )
					return;

				while ( buf_ != NULL ) {
					p = buf_->next_;
					delete buf_;
					buf_ = p;
				}
				size_ = 0;
			}
			*/
		int size()
			{
				return size_;
			}

		int typeCompare(eventType nodeType1, eventType nodeType2){
			if (nodeType1 < nodeType2)
				return 1;
			else if (nodeType1 == nodeType2)
				return 0;
			else
				return -1;
		
		}


		int compare(eventType nodeType, double eventTime, EventNode *p){
			if (p->eventTime < eventTime){
				return 1;
			}else if (p->eventTime == eventTime){
				if ( typeCompare(p->nodeType, nodeType) == 1)
					return 1;
				else if ( typeCompare(p->nodeType, nodeType) == 0)
					return 0;
				else
					return -1;
			}else{
				return -1;
			}
		}



		void push(eventType nodeType, videoPkt pkt, double eventTime)
			{
				EventNode *p, *t, *pre;
				//printf("Enter\n");
				if (buf_ == NULL)
				{
					//printf("Enter\n");
					buf_ = new EventNode;
					buf_->nodeType = nodeType;
					buf_->pkt = pkt;
					buf_->eventTime = eventTime;
					buf_->next_ = NULL;
				} else {
					//printf("Enter1\n");
					p = buf_;
					//while ( (p->next_ != NULL) && (p->eventTime <= eventTime) ){
					while ( (p->next_ != NULL) && ( compare(nodeType, eventTime, p) >= 0) ){
					//printf("Enter1\n");
						pre = p;
						p = p->next_;
					}

					if ( p->next_ == NULL)
					{
						//if (p->eventTime <= eventTime){
						if ( compare(nodeType, eventTime, p) >= 0){
							t = new EventNode;
							t->nodeType = nodeType;
							t->pkt = pkt;
							t->eventTime = eventTime;
							t->next_ = NULL;
							p->next_ = t;
						}else{
							if ( p != buf_){
								t = new EventNode;
								t->nodeType = nodeType;
								t->pkt = pkt;
								t->eventTime = eventTime;
								t->next_ = p;
								pre->next_ = t;
							}else{
								t = new EventNode;
								t->nodeType = nodeType;
								t->pkt = pkt;
								t->eventTime = eventTime;
								t->next_ = p;
								buf_ = t;		
							}
						}
					}else if ( p == buf_){
						t = new EventNode;
						t->nodeType = nodeType;
						t->pkt = pkt;
						t->eventTime = eventTime;
						t->next_ = buf_;
						buf_ = t;
					}else{
						t = new EventNode;
						t->nodeType = nodeType;
						t->pkt = pkt;
						t->eventTime = eventTime;
						t->next_ = p;
						pre->next_ = t;
					}
				}
				size_ ++;
				//printf("Queue Size %d\n",size_);
			}

		bool isEmpty()
			{
				return (size_ == 0);
			}

		void delTimeoutElement(videoPkt pkt){
			double enctime = pkt.enctime;
			double frame = pkt.frame;
			int slice = pkt.slice;
			EventNode *pre, *now;
			pre = now = buf_;
			if (size_==0){
				return;
			}else{
				if (size_ == 1){
					// (( now->nodeType == SEND_TIMEOUT ) && (now->pkt.enctime == enctime) && (now->pkt.frame == frame) 
						// (now->pkt.slice == slice)){
					if (( now->nodeType == SEND_TIMEOUT ) && (now->pkt.frame == frame) && (now->pkt.slice == slice)){
						buf_ = NULL;
						size_=0;
						return;
					}else{
						return;
					}	
				}else{
					while ( now->next_!=NULL){
				//if (( now->nodeType == SEND_TIMEOUT ) && (now->pkt.enctime == enctime) && (now->pkt.frame == frame) 
					//&& (now->pkt.slice == slice)){
						if (( now->nodeType == SEND_TIMEOUT ) && (now->pkt.frame == frame)	&& (now->pkt.slice == slice)){
								if ( now == buf_){
									buf_ = now->next_;
									size_ = size_ - 1;
									return;
								}else{
									pre->next_ = now->next_;
									size_ = size_ - 1;
									return;
								}
						}
						pre = now;
						now = now->next_;
					}
					if ( now->next_ == NULL ){
						//if (( now->nodeType == SEND_TIMEOUT ) && (now->pkt.enctime == enctime) && (now->pkt.frame == frame) 
							//&& (now->pkt.slice == slice)){
						if (( now->nodeType == SEND_TIMEOUT ) && (now->pkt.frame == frame) && (now->pkt.slice == slice)){		
								pre->next_ = NULL;
								size_ = size_ - 1;
								return;
						}else{
							return;
						}
					}
				}
			}

		
		}


		EventNode* pop()
			{
				EventNode *p;

				if ((size_ == 0) || (buf_ == NULL))
				{
					return NULL;
		
				} else {

					//printf(" size: %d \n", size_);
					p = buf_;

					buf_ = buf_->next_;
					size_ --;

					return p;
				}
			}
};




#endif