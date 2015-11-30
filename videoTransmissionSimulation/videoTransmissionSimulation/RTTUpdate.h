#ifndef rtt_update_h
#define rtt_update_h

class RTTUpdate{
	private:
		double currentRTT;
		int totalNum;



	public:
		RTTUpdate(){}
		RTTUpdate(double rtt){
			currentRTT = rtt;
			totalNum = 1;
		}
		~RTTUpdate(){}
		void updateRTT(double rtt){
			double totalRTT = (currentRTT*totalNum + rtt)*1.0;
			totalNum = totalNum + 1;
			currentRTT = totalRTT/totalNum;
		}

		double getCurrentRTT(){
			return currentRTT;
		}



};



#endif