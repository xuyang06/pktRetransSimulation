#ifndef timeout_update_h
#define timeout_update_h

class TimeoutUpdate{
	private:
		double currentTimeout;
		int totalNum;



	public:
		TimeoutUpdate(){}
		~TimeoutUpdate(){}
		TimeoutUpdate(double rtt){
			currentTimeout = rtt;
			totalNum = 1;
		}

		void updateTimeout(double rtt){
			double totalRTT = (currentTimeout*totalNum + rtt)*1.0;
			totalNum = totalNum + 1;
			currentTimeout = totalRTT/totalNum;
		}

		double getCurrentTimeout(){
			return currentTimeout;
		}



};



#endif