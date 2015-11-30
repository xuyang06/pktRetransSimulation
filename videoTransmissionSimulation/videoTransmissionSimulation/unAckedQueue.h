#ifndef un_acked_queue_h
#define un_acked_queue_h

#include "videopkt.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;



struct unAckedNode {
	videoPkt pkt_;
	unAckedNode* next_;
};

class UnAckedQueue {
	private:
		unAckedNode *buf_; // the underlying buffer
		int size_;
		
	public:
		UnAckedQueue()
			{
				buf_ = NULL;
				size_ = 0;
			}

		~UnAckedQueue()
			{
				printf("delete variables in UnACKedQueue.\n");
				unAckedNode *p;

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
				unAckedNode *p;

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
		
		void del(videoPkt pkt)
			{
				unAckedNode *p,*q;

				p = q = buf_;
				int find = 0;

				if (size_ == 0)
				{
					return ;
		
				}else{
					if ( ( p->pkt_.frame == pkt.frame) && (p->pkt_.slice == pkt.slice) ){
						buf_ = buf_->next_;
						size_ = size_ - 1;
					}else{ 
						if ( p->next_ != NULL){
							q = p->next_;
							while( q!= NULL){
								if ( ( q->pkt_.frame == pkt.frame) && (q->pkt_.slice == pkt.slice) ){
									find = 1;
									p->next_ = q->next_;
									size_ = size_ - 1;
									break;
								}else{
									p = q;
									q = q->next_;
								} 
							}
						}
					}
				}

			}


		void push(videoPkt pkt)
			{
				unAckedNode *p, *t;

				if (buf_ == NULL)
				{
					buf_ = new unAckedNode;
					buf_->pkt_ = pkt;
					buf_->next_ = NULL;
				} else {
		
					p = buf_;
					while ( p->next_ != NULL )
					p = p->next_;

					t = new unAckedNode;
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

		unAckedNode* pop()
			{
				unAckedNode *p;

				if (size_ == 0)
				{
					return NULL;
		
				} else {

					p = buf_;
					buf_ = buf_->next_;
					size_ --;

					return p;
				}
			}

		int checkSeqInQueue(videoPkt pkt)
		{
			unAckedNode *p;
			if (size_ == 0){

				return -1;
			}else{

				int index = 1;
				p = buf_;
				while ( p->next_ != NULL ){
					if ( (p->pkt_.frame == pkt.frame) && (p->pkt_.slice == pkt.slice) ){
						break;
					}
					p = p->next_;
					index = index + 1;
				}


				if ( ( p->next_ == NULL ) && ( p->pkt_.frame == pkt.frame ) && ( p->pkt_.slice == pkt.slice ) ) {
					index = index + 1;
					return index;
				}else if ( ( p->next_ == NULL ) &&  ( ( p->pkt_.frame != pkt.frame ) || ( ( p->pkt_.frame == pkt.frame ) && ( p->pkt_.slice != pkt.slice ) )  ) ) {
					return -1;
				}else{
					return index;
				}

			}
		
		}


		vector<videoPkt> getUnAckedVideoPkts( videoPkt pkt, FILE *f_status )
		{
			vector<videoPkt> unAckedVideoPkts;
			unAckedNode *p;

			int index = checkSeqInQueue(pkt);

			if ( ( index == -1 ) || ( index == 1) ){
				fprintf(f_status,"UnAckedVideoPkts: No Data. \n");
				//f_status<<"UnAckedVideoPkts: No Data"<<endl;
				p = pop();
				return unAckedVideoPkts;
			}else{
				int i = 1;
				//p = buf_;
				while ( i < index ){
					p = pop();
					//f_status<<"UnAckedVideoPkts: Frame ID: "<<p->pkt_.frame<<", Nslices: "<<p->pkt_.nslice<<", slice: "<<p->pkt_.slice
						//<<", Enctime:"<<p->pkt_.enctime<<", Dectime:"<<p->pkt_.dectime<<endl;
					fprintf(f_status,"UnAckedVideoPkts: Frame ID: %d, nslice: %d, slice: %d, encoding time: %f, decoding time: %f. \n",
						p->pkt_.frame, p->pkt_.nslice, p->pkt_.slice, p->pkt_.enctime, p->pkt_.dectime);

					unAckedVideoPkts.push_back(p->pkt_);
					//p = p->next_;
					i = i + 1;
				}
				p = pop();
				return unAckedVideoPkts;
			}	
		}


		void PrintFIFOqueue( FILE * f_status)
		{
			fprintf(f_status,"\n");
			fprintf(f_status,"Begin to print un-acked queue status ... \n");

			//f_status<<endl;
			//f_status<<"Begin to print un-acked queue status ... "<<endl;
			unAckedNode *temp = buf_;
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
			fprintf(f_status,"End to print un-acked queue status ... \n");
			fprintf(f_status,"\n");
			//f_status<<"End to print un-acked queue status ... "<<endl;
			//f_status<<endl;
		}


};

#endif //#define ns_FIFOqueue_h
