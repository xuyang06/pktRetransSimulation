#ifndef video_Decoder_h
#define video_Decoder_h
#include <stdio.h>
#include <vector>
#include <iostream>
#include "videoFrame.h"
#include "videopkt.h"
#include <fstream>
#include <algorithm>
using namespace std;

class VideoDecoder{
	private:
		vector<double> decodedTime;
		vector<double> decodedRate;
		vector<double> decodedUpperRate;
		vector<double> upperRate;
		vector<int> decodedFrameNum;
		vector<int> decodedUpperFrameNum;
		vector<int> trans_time_list;
		//vector<>
		vector<int> highestDecodedGOPInEachPeriods;
		vector<int> smallestFrameInEachPeriods;
		vector<int> frameNumInEachPeriods;

		//int highestDecodedGOP;
		vector<VideoFrame> videoFrames;
		vector<int> frameBytes;
		vector<int> receivedAllPacketInFrame;
		vector<int> decodedFrame;
		vector<int> currentDecodedFrame;
		double FPS;
		double FPSUpper;
		//int highestDecodedGop;
		int frame_num;

	public:
		VideoDecoder()
		{
		}

		~VideoDecoder()
		{
			printf("delete variables in VideoDecoder.\n");
		}

		/*
		VideoDecoder(int frame_number)
		{
			highestDecodedGOP = -1;
			frame_num = frame_number;
			for (int i = 0; i < frame_num; i++){
				VideoFrame videoFrame;
				videoFrames.push_back(videoFrame);
				frameBytes.push_back(0);
				receivedAllPacketInFrame.push_back(0);
				decodedFrame.push_back(0);
				currentDecodedFrame.push_back(0);
			}
		}
		*/


		VideoDecoder(int frame_number)
		{
			//highestDecodedGOP = -1;
			frame_num = frame_number;
			for (int i = 0; i < frame_num; i++){
				VideoFrame videoFrame;
				videoFrames.push_back(videoFrame);
				frameBytes.push_back(0);
				receivedAllPacketInFrame.push_back(0);
				decodedFrame.push_back(0);
				currentDecodedFrame.push_back(0);
			}
		}

		void addSmallestNumInOneNewGOP(int smallestNum){
			smallestFrameInEachPeriods.push_back(smallestNum);
		}
	
		void sortSmallestNumInGOPs(FILE *f_status){
			int smallestFrames[10*60];
			for(int i = 0; i < smallestFrameInEachPeriods.size(); i ++){
				smallestFrames[i] = smallestFrameInEachPeriods[i];
			}
			int size = smallestFrameInEachPeriods.size();
			sort(smallestFrames, smallestFrames + size);
			smallestFrameInEachPeriods.clear();
			for(int i = 0; i < size; i ++ ){
				smallestFrameInEachPeriods.push_back(smallestFrames[i]);
				highestDecodedGOPInEachPeriods.push_back(-1);
			}
			int first = 0;
			int last = 0;
			for(int i = 0; i < smallestFrameInEachPeriods.size(); i ++ ){
				if ( i != (smallestFrameInEachPeriods.size() - 1) ){
					first = smallestFrameInEachPeriods[i];
					last = smallestFrameInEachPeriods[i+1];
					int frame_num_in_period = last - first;
					frameNumInEachPeriods.push_back(frame_num_in_period);
				}else{
					first = smallestFrameInEachPeriods[i];
					last = frame_num;
					int frame_num_in_period = last - first;
					frameNumInEachPeriods.push_back(frame_num_in_period);
				}
			}
			fprintf(f_status, "\n");
			fprintf(f_status, "Smallest Frame Id \n");
			//f_status<<endl;
			//f_status<<"Smallest Frame Id"<<endl;
			for(int i = 0; i < smallestFrameInEachPeriods.size(); i ++ ){
				fprintf(f_status, "%d;",smallestFrameInEachPeriods[i]);
				//f_status<<smallestFrameInEachPeriods[i]<<";";
			}
			fprintf(f_status, "\n");
			//f_status<<endl;
			

			//sort(smallestFrameInEachPeriods,(smallestFrameInEachPeriods + smallestFrameInEachPeriods.size()));
		}
		

		void addOneNewFrame(videoPkt pkt, int frameByte){
			int i = pkt.frame;
			videoFrames[i].initialize(pkt.nslice, pkt.fecTotalSlice);
			frameBytes[i] = frameByte;
		}

		/*
		void receivePkt(videoPkt pkt, fstream &f_status){
			trans_time_list.push_back(pkt.trans_num);
			int i = pkt.frame;
			videoFrames[i].receiveOneSlice(pkt.slice);
			if (videoFrames[i].receivedAllSlicesInOneFrame()){
				receivedAllPacketInFrame[i] = 1;
				if ( i%4 == 0){
					if ( highestDecodedGOP >= (i/4 - 1) ){
						decodedFrame[i] = 1;
						highestDecodedGOP = i/4;
						int j = highestDecodedGOP + 1;
						while ( (j < frame_num/4) && (receivedAllPacketInFrame[(j*4)] == 1)){
							highestDecodedGOP = j;
							decodedFrame[(j*4)] = 1;
							j = j + 1;
						}
					}
				}else if (i%4 == 1){
					if ( highestDecodedGOP >= i/4 ){
						decodedFrame[i] = 1;
					}
				}else if (i%4 == 2){
					if ( highestDecodedGOP >= i/4 ){
						decodedFrame[i] = 1;
					}
				}else{
					if ( ( highestDecodedGOP == i/4 ) && ( decodedFrame[i-1] == 1 ) ){
						decodedFrame[i] = 1;
					}
				}
			}
			f_status<<endl;
			f_status<<"Begin to Receive One Pkt...."<<endl;
			f_status<<"Check For Frame: "<<i<<"."<<endl;
			VideoFrame videoFrameI = videoFrames[i];
			for (int i = 0; i< videoFrameI.size(); i++){
				int condition = videoFrameI.geti(i);
				f_status<<condition<<",";
			}
			f_status<<endl;
			f_status<<"End to Receive One Pkt...."<<endl;
			f_status<<"Begin Highest Decoded GOP"<<endl;
			f_status<<highestDecodedGOP<<endl;
			f_status<<"End Highest Decoded GOP"<<endl;
			f_status<<endl;


		}
		*/

		int getCorrespondingSmallestFrameInEachPeriodsFromFrame(int frame_id){
			int now = 0;
			int pre = now;
			//int i = 0;
			if (frame_id == 0){
				return 0;
			}else{
				while ( now < smallestFrameInEachPeriods.size()){
					if (smallestFrameInEachPeriods[now] > frame_id){
						return pre;
					}else{
						pre = now;
						now = now + 1;
					}
				}
				if ( now == smallestFrameInEachPeriods.size() ){
					if (smallestFrameInEachPeriods[smallestFrameInEachPeriods.size() - 1] <= frame_id){
						return smallestFrameInEachPeriods.size() - 1;
					}			
				}
				return -1;
			}
		} 


		void receivePkt(videoPkt pkt, FILE* f_status){
			trans_time_list.push_back(pkt.trans_num);
			int i = pkt.frame;
			videoFrames[i].receiveOneSlice(pkt.slice);
			fprintf(f_status,"\n");
			fprintf(f_status,"Begin to Receive One Pkt.... \n");
			//f_status<<endl;
			//f_status<<"Begin to Receive One Pkt...."<<endl;
			if (videoFrames[i].receivedAllSlicesInOneFrame()){
				receivedAllPacketInFrame[i] = 1;
				int SmallestFrameId = getCorrespondingSmallestFrameInEachPeriodsFromFrame(i);
				fprintf(f_status,"SmallestFrameId: %d \n", SmallestFrameId);
				//f_status<<"SmallestFrameId: "<<SmallestFrameId<<endl;
				if ( SmallestFrameId == -1 ){
					fprintf(f_status,"No GOP for this frame.... \n");
					//f_status<<"No GOP for this frame...."<<endl;
				}else{
					int highestDecodedGOP = highestDecodedGOPInEachPeriods[SmallestFrameId];
					int smallestFrame = smallestFrameInEachPeriods[SmallestFrameId];
					fprintf(f_status,"SmallestFrame: %d \n", smallestFrameInEachPeriods[SmallestFrameId] );
					//f_status<<"SmallestFrame: "<<smallestFrameInEachPeriods[SmallestFrameId]<<endl;
					if (highestDecodedGOP == -1){
						if( i == smallestFrame){
							highestDecodedGOPInEachPeriods[SmallestFrameId] = 0;
							fprintf(f_status,"No.%d GOP is changed to be 0.\n", SmallestFrameId );
							//f_status<<"No. "<<SmallestFrameId<<" GOP is changed to be 0."<<endl;
							decodedFrame[i] = 1;
						}
					}else{
						if ( (i - smallestFrame)%4 == 0 ){
							if ( highestDecodedGOP >= ( (i - smallestFrame)/4 - 1 ) ){
								decodedFrame[i] = 1;
								highestDecodedGOPInEachPeriods[SmallestFrameId] = (i - smallestFrame)/4;
								fprintf(f_status,"No. %d GOP is changed to be %d.\n", SmallestFrameId, (i - smallestFrame)/4 );
								//f_status<<"No. "<<SmallestFrameId<<" GOP is changed to be"<<(i - smallestFrame)/4<<endl;
								int j = highestDecodedGOPInEachPeriods[SmallestFrameId] + 1;
								int frame_num_in_period = frameNumInEachPeriods[SmallestFrameId];
								while ( ( j < (frame_num_in_period/4) ) && ( receivedAllPacketInFrame[(j*4)+ smallestFrame] == 1 )){
									highestDecodedGOPInEachPeriods[SmallestFrameId] = j;
									fprintf(f_status,"No. %d GOP is changed to be %d.\n", SmallestFrameId, j );
									//f_status<<"No. "<<SmallestFrameId<<" GOP is changed to be"<<j<<endl;
									decodedFrame[(j*4)] = 1;
									j = j + 1;
								}
							}
						}else if ( (i - smallestFrame)%4 == 1 ){
							if ( highestDecodedGOP >= ( (i - smallestFrame)/4 ) ){
								decodedFrame[i] = 1;
							}

						}else if ( (i - smallestFrame)%4 == 2){
							if ( highestDecodedGOP >= ( (i - smallestFrame)/4 ) ){
								decodedFrame[i] = 1;
							}
						}else{
							if ( ( highestDecodedGOP == ( (i - smallestFrame)/4 ) ) && ( decodedFrame[i-1] == 1 ) ){
								decodedFrame[i] = 1;
							}						
						}
					}
				}
			}
			fprintf(f_status,"Begin to Receive One Pkt.... \n");
			fprintf(f_status,"Check For Frame: %d \n", i);
			//f_status<<"Begin to Receive One Pkt...."<<endl;
			//f_status<<"Check For Frame: "<<i<<"."<<endl;
			VideoFrame videoFrameI = videoFrames[i];
			for (int i = 0; i< videoFrameI.fecSize(); i++){
				int condition = videoFrameI.geti(i);
				fprintf(f_status,"%d, ", condition);
				//f_status<<condition<<",";
			}
			fprintf(f_status,"\n");
			//f_status<<endl;
			fprintf(f_status,"End to Receive One Pkt....\n");
			fprintf(f_status,"Begin Highest Decoded GOP \n");
			//f_status<<"End to Receive One Pkt...."<<endl;
			//f_status<<"Begin Highest Decoded GOP"<<endl;
			//f_status<<highestDecodedGOP<<endl;
			for ( int i = 0; i < highestDecodedGOPInEachPeriods.size(); i++ ){
				int decodedGOP = highestDecodedGOPInEachPeriods[i];
				fprintf(f_status,"%d, ", decodedGOP);
				//f_status<<decodedGOP<<",";
			}
			fprintf(f_status,"\n");
			fprintf(f_status,"End Highest Decoded GOP \n");
			fprintf(f_status,"\n");
			//f_status<<endl;
			//f_status<<"End Highest Decoded GOP"<<endl;
			//f_status<<endl;


		}

		/*
		bool updateHighestDecodedGop(){
			
		}
		*/
		void checkDecodedFrame(int i, FILE * f_status){
			int SmallestFrameId = getCorrespondingSmallestFrameInEachPeriodsFromFrame(i);
			if ( decodedFrame[i] == 1){
				currentDecodedFrame[i] = 1;
			}else{
				if ( SmallestFrameId == -1 ){
					fprintf(f_status, "No GOP for this frame.... \n");
					//f_status<<"No GOP for this frame...."<<endl;
				}else{
					int highestDecodedGOP = highestDecodedGOPInEachPeriods[SmallestFrameId];
					int smallestFrame = smallestFrameInEachPeriods[SmallestFrameId];
					if ( ( (i - smallestFrame)%4 == 1 ) || ( (i - smallestFrame)%4 == 2 ) ){
						if ( ( highestDecodedGOP >= ((i - smallestFrame)/4) ) && (receivedAllPacketInFrame[i] == 1) ){
							decodedFrame[i] = 1;
							currentDecodedFrame[i] = 1;
						} 	
					}else if ( (i - smallestFrame)%4 == 3){
						if (( highestDecodedGOP >= (i - smallestFrame)/4 ) && (receivedAllPacketInFrame[i-1] == 1) && (receivedAllPacketInFrame[i] == 1) ){
							decodedFrame[i-1] = 1;
							decodedFrame[i] = 1;
							currentDecodedFrame[i] = 1;	
						}
					}
				}
			}
			fprintf(f_status, "\n");
			//f_status<<endl;
			fprintf(f_status, "Begin to check decoded Frame.... \n");
			fprintf(f_status, "Check For Frame: %d .\n", i);
			//f_status<<"Begin to check decoded Frame...."<<endl;
			//f_status<<"Check For Frame: "<<i<<"."<<endl;
			VideoFrame videoFrameI = videoFrames[i];
			for (int i = 0; i< videoFrameI.fecSize(); i++){
				int condition = videoFrameI.geti(i);
				fprintf(f_status, "%d,", condition);
				//f_status<<condition<<",";
			}
			fprintf(f_status, "\n");
			fprintf(f_status, "End to check decoded Frame....\n");
			fprintf(f_status, "Begin Highest Decoded GOP\n");
			//f_status<<endl;
			//f_status<<"End to check decoded Frame...."<<endl;
			//f_status<<"Begin Highest Decoded GOP"<<endl;
			for ( int i = 0; i < highestDecodedGOPInEachPeriods.size(); i++ ){
				int decodedGOP = highestDecodedGOPInEachPeriods[i];
				fprintf(f_status, "%d,", decodedGOP);
				//f_status<<decodedGOP<<",";
			}
			fprintf(f_status, "\n");
			fprintf(f_status, "End Highest Decoded GOP\n");
			fprintf(f_status, "\n");
			fprintf(f_status, "\n");
			//f_status<<endl;
			//f_status<<"End Highest Decoded GOP"<<endl;
			//f_status<<endl;
			//f_status<<endl;
		}

		void calculate(FILE * f_trans_time, FILE * f_status_final_result){
			int FPSTotal = 0;
			int i = 0;
			int iGop = -1;
			double rate = 0;
			double upper_rate = 0;
			double frame = 0;
			double totalRate = 0;
			double totalRateUpper = 0;
			double decodableFrame = 0;
			double decodableRate = 0;
			double totalDecodableRate = 0;
			int decodableFPSTotal = 0;
			while ( i < frame_num){
				if (currentDecodedFrame[i] == 1){
					FPSTotal = FPSTotal + 1;
					rate = rate + frameBytes[i];
					frame = frame + 1;
					totalRate = totalRate + frameBytes[i];
				}
				if (decodedFrame[i] == 1){
					decodableFPSTotal = decodableFPSTotal + 1;
					decodableRate = decodableRate + frameBytes[i];
					decodableFrame = decodableFrame + 1;
					totalDecodableRate = totalDecodableRate + frameBytes[i];

				}
				upper_rate = upper_rate + frameBytes[i];
				iGop = iGop + 1;
				if ( iGop == 3){
					decodedUpperRate.push_back(decodableRate/0.133);
					decodedRate.push_back(rate/0.133);
					upperRate.push_back(upper_rate/0.133);

					decodedUpperFrameNum.push_back(decodableFrame);
					decodedFrameNum.push_back(frame);
					decodedTime.push_back(i/4*133.33 +33.33*2);
					iGop = -1;
					rate = 0;
					frame = 0;
					upper_rate = 0;
					decodableRate = 0;
					decodableFrame = 0;
				}
				i ++;
			}
			FPS = (FPSTotal*30.0)/frame_num;
			FPSUpper = (decodableFPSTotal*30.0)/frame_num;
			printf("Display FPS %f... , Upper Display FPS %f... \n", 
				   FPS, FPSUpper);
			fprintf(f_status_final_result, "Display FPS %f... , Upper Display FPS %f... \n", 
				   FPS, FPSUpper);
			totalRate = (totalRate*1.0)/(frame_num/30);
			totalRateUpper = (totalDecodableRate*1.0)/(frame_num/30);
			printf("Display rate %f... , Upper Display rate %f...  \n", 
				   totalRate, totalRateUpper);
			fprintf(f_status_final_result, "Display rate %f... , Upper Display rate %f...  \n", 
				   totalRate, totalRateUpper);
			int index = 0;
			while( index < trans_time_list.size()){
				fprintf(f_trans_time,"%d\n", trans_time_list[index]);
				//f_trans_time<<trans_time_list[index]<<endl;
				index = index + 1;
			}

		}

		void PrintTimeRateFrameInfo( FILE* f_status_time, FILE* f_status_rate, FILE* f_status_rate_decoded, FILE* f_status_decoded_frame_num, 
			FILE* f_status_frame_num_decoded, FILE* f_status_upper_rate ){
			for(int i = 0; i < decodedTime.size(); i ++){
				fprintf(f_status_time, "%f\n", decodedTime[i]);
				fprintf(f_status_rate, "%f\n", decodedRate[i]);
				fprintf(f_status_rate_decoded, "%f\n", decodedUpperRate[i]);
				fprintf(f_status_upper_rate, "%f\n", upperRate[i]);
				fprintf(f_status_decoded_frame_num, "%d\n", decodedFrameNum[i]);
				fprintf(f_status_frame_num_decoded, "%d\n", decodedUpperFrameNum[i]);

				//f_status_time<<decodedTime[i]<<endl;
				//f_status_rate<<decodedRate[i]<<endl;
				//f_status_rate_decoded<<decodedUpperRate[i]<<endl;
				//f_status_upper_rate<<upperRate[i]<<endl;
				//f_status_decoded_frame_num<<decodedFrameNum[i]<<endl;
				//f_status_frame_num_decoded<<decodedUpperFrameNum[i]<<endl;
			}

		}


		void PrintFrameDecodeInfo( FILE * f_status)
		{
			fprintf(f_status, "\n");
			fprintf(f_status, "Begin to print decoded frame status ...\n");
			//f_status<<endl;
			//f_status<<"Begin to print decoded frame status ... "<<endl;
			for(int i = 0; i< currentDecodedFrame.size(); i++){
				fprintf(f_status, "Frame ID: %d; currentDecodedFrame: %d; DecodedFrame: %d\n", i, currentDecodedFrame[i], decodedFrame[i]);
				//f_status<<"Frame ID: "<<i<<"; currentDecodedFrame:" <<currentDecodedFrame[i]<<"; DecodedFrame:" <<decodedFrame[i]<<endl;
			}
			fprintf(f_status, "End to print decoded frame status ...\n");
			fprintf(f_status, "\n");
			//f_status<<"End to print decoded frame status ... "<<endl;
			//f_status<<endl;
		
		}

};







#endif