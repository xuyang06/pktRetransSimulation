#ifndef sending_window_h
#define sending_window_h

#include <fstream>
class SendingWindow{
	private:
		int totalWindowSize;
		int availableWindowSize;


	public:
		SendingWindow()
		{
			availableWindowSize = 0;
			totalWindowSize = 0;
		}

		~SendingWindow()
		{
				printf("delete variables in SendingWindow.\n");
		}

		void initilize(int a_totalWindowSize)
		{
			totalWindowSize = a_totalWindowSize;
			availableWindowSize = totalWindowSize;
		}


		void setTotalWindowSize(int a_totalWindowSize)
		{
			totalWindowSize = a_totalWindowSize;
		}

		
		int getAvaiableWindowSize(){
			return availableWindowSize;
		}


		bool sendOnePkt()
		{
			if (availableWindowSize == 0)
			{
				return false;			
			}else{
				availableWindowSize = availableWindowSize - 1;
				return true;
			}
		}

		bool receiveOneAck()
		{
			if (availableWindowSize == totalWindowSize){
				return false;
			}else{
				availableWindowSize = availableWindowSize + 1;
				return true;
			}
		}

};


#endif