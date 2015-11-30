#ifndef video_Frame_h
#define video_Frame_h
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

class VideoFrame{
	private:
		vector<int> slices;
		int nslices;
		int fecSlices;



	public:

		int fecSize(){
			return fecSlices;
		}

		int size(){
			return nslices;
		}

		int geti(int i){
			return slices[i];
		}

		VideoFrame(){
			nslices = -1;
			fecSlices = -1;
		}

		~VideoFrame(){}

		void initialize(int slice_num, int fec_slice_num){
			nslices = slice_num;
			fecSlices = fec_slice_num;
			int i = 0;
			while ( i < fec_slice_num ){
				slices.push_back(0);
				i++;
			}
		}
		
		void receiveOneSlice(int slice){
			slices[slice] = 1;
		}
		/*
		bool receivedAllSlicesInOneFrame(){
			int allreceived = 1;
			int i = 0;
			while ( i < nslices){
				if ( slices[i] == 0){
					allreceived = 0;
					break;
				}
				i++;
			}
			if (allreceived == 1){
				return true;
			}else{
				return false;
			}
		}
		*/

		bool receivedAllSlicesInOneFrame(){
			//int allreceived = 1;
			int receivedNum = 0;
			int i = 0;
			while ( i < fecSlices){
				if ( slices[i] == 1){
					//allreceived = 0;
					//break;
					receivedNum = receivedNum + 1;
				}
				i++;
			}
			if (receivedNum >= nslices){
				return true;
			}else{
				return false;
			}
		}
		
};

#endif