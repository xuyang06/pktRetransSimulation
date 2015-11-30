#ifndef rate_id_h
#define rate_id_h

#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

class RateID {
	private:
		vector<double> rates;
		vector<int> IDs;
		int selectedID;

		int getIDAccordingToRate(double rate){
			if ( getIDIndexAccordingToRate(rate) == 0 ){
				return -1;
			}else{
				return IDs[getIDIndexAccordingToRate(rate)-1];
			}
		}



		int getIDIndexAccordingToRate(double rate){
			int i = IDs.size();
			double rateHigh = rates[i-1];
			if (rate >= rateHigh){
				return i;
			}else{
				while(i>=1){
					if (rate >= rates[i-1]){
						return i;
						break;
					}else{
						i = i - 1;
					}
				}
				return i;

			}
		}

	public:
		RateID(){
			selectedID = -1;
		}

		~RateID(){
		}

		void initialize_add_rateid_pair(int id, double rate){
			IDs.push_back(id);
			rates.push_back(rate);
		}

		void updateID(double rate){
			selectedID = getIDAccordingToRate(rate);
		}

		void setID(int id){
			selectedID = id;
		}

		int getSelectedID(){
			return selectedID;
		}

		



};


#endif