#ifndef channel_property_h
#define channel_property_h

#include <iostream>
#include <vector>
using namespace std;

typedef enum
{
	Rand_loss,
	Bursty_loss,
	Bursty_Gilbert_Model_loss,
	No_loss,
}LossType;


class ChannelProperty{
	private:
		double round_trip_time;
		//double bandwidth;
		LossType type;
		double loss_probability;
		int bursty_loss_pkt;

		int bandwidthVarLen;
		vector<double> bandwidthValueList;
		vector<double> bandwidthTimeList;


		double p_unconditional_loss;
		double p_conditional_loss;
		double p_prob;
		double q_prob;
	public:
		ChannelProperty(double a_time)
		{
			round_trip_time = a_time;
			type = No_loss;
		}

		~ChannelProperty()
		{
				
		}

		double getRoundTripTime()
		{
			return round_trip_time;
		}

		void setBandwidth(double a_bandwidth, double a_bandwidth_time)
		{
			bandwidthValueList.push_back(a_bandwidth);
			bandwidthTimeList.push_back(a_bandwidth_time);
		}

		int getBandwidthListSize()
		{
			return bandwidthValueList.size();
		}

		double getBandwidthValue(int i)
		{
			return bandwidthValueList[i];
		}
		                                                                            
		double getBandwidthTime(int i)
		{
			return bandwidthTimeList[i];
		}

		void setRandomLossProperty(double a_loss_probability)
		{
			type = Rand_loss;
			loss_probability = a_loss_probability;
		} 

		void setBurstyLossProperty(double a_loss_probability, int a_bursty_loss_pkt)
		{
			type = Bursty_loss;
			loss_probability = a_loss_probability;
			bursty_loss_pkt = a_bursty_loss_pkt;
		}

		void setGilbertBurstyLossProperty(double a_p_unconditional, double a_p_conditional)
		{
			type = Bursty_Gilbert_Model_loss;
			p_unconditional_loss = a_p_unconditional;
			p_conditional_loss = a_p_conditional;
			q_prob = 1 - p_conditional_loss;
			p_prob = (p_unconditional_loss*(1 - p_conditional_loss))/(1 - p_unconditional_loss);
		}

		double get_q_prob_Gilbert()
		{
			return q_prob;
		}

		double get_p_prob_Gilbert()
		{
			return p_prob;
		}

		void PrintChannelInfo()
		{
			printf("Round trip time %f...\n", round_trip_time);
			if (type == Rand_loss){
				printf("Random Loss Probability %f...\n", loss_probability);
			}else if (type == Bursty_loss){
				printf("Bursty Loss Probability %f, bursty loss pkt %d...\n", loss_probability, bursty_loss_pkt);
			}else{
				printf("No loss...\n");
			}
			for (int i = 0; i <bandwidthValueList.size(); i++){
				printf("bandwidth time %f, value %f...\n", bandwidthTimeList[i], bandwidthValueList[i]);
			}
		}

		LossType getLossType()
		{
			return type;
		}

		double getLossProbability()
		{
			return loss_probability;
		}

		int getBurstyLossPkt()
		{
			return bursty_loss_pkt;
		}

		
};




#endif