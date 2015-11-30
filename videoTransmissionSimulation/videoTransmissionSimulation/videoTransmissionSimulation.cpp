// videoTransmissionSimulation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "videoPreordersvc.h"
#include "EventFIFOqueue.h"
#include "networkFIFOqueue.h"
#include "videoDecoder.h"
//#include "EventQueueManagement.h"
//#include "EventQueueManagement_version3.h"
#include "EventQueueManagement_version4.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Video Conference Transmission Simulation ..."<<std::endl;
	//int v1;
	/*
	VideoDecoder videoDecoder = VideoDecoder(300*120);
	double timeoutDuration = 50;
	VideoSVCPreorder preorder;
	EventFIFOqueue eventFIFOqueue;
	double transmissionDeadline = 200;
	//const char * pkt_file = "E:\\simulation\\foreman_VGA_GOP4_QP33_enc_pktData_CBR.dat";
	const char * pkt_file = "E:\\simulation\\FrmnumTidSize.txt";

	double repeatTime = 10;
	preorder.read_videopkt_trace(pkt_file, transmissionDeadline, eventFIFOqueue, timeoutDuration, videoDecoder, repeatTime);
	eventFIFOqueue.PrintFIFOqueue();
	if (eventFIFOqueue.isEmpty()){
		printf("queue empty\n");
	}
	eventFIFOqueue.PrintFIFOqueue();
	*/
	
	string infor_base_directory = "E:\\simulation";
	double round_trip_time = 20;
	double time_out_duration = 60;
	int a_queue_limit_ = 10;
	double measuredBandwidth = 1000;
	//double repeatTime = 60;
	double repeatTime = 1;
	//int frame_num_decoder = 300*120;
	
	int frame_num_decoder = 300;	
	double deadline = 200; 
	
	int turnFECOn = 0;
	float iFECRatio = 1.0;
	float pFECRatio = 0.5;
	int fps = 30;
	int pktsize = 1400;
	//EventQueueManagement eventQueueManagement = EventQueueManagement(round_trip_time, time_out_duration, a_queue_limit_, measuredBandwidth, repeatTime, frame_num_decoder, deadline );
	//EventQueueManagementVersion3 eventQueueManagement = EventQueueManagementVersion3(round_trip_time, time_out_duration, a_queue_limit_, measuredBandwidth, repeatTime, frame_num_decoder, deadline );
	EventQueueManagementVersion4 *eventQueueManagement = new EventQueueManagementVersion4(round_trip_time, time_out_duration, a_queue_limit_, measuredBandwidth, repeatTime, frame_num_decoder, deadline, turnFECOn, iFECRatio, pFECRatio, infor_base_directory, fps, pktsize);
	


	double a_bandwidth = 1000;
	double a_bandwidth_time = 0;	
	eventQueueManagement->setBandwidth(a_bandwidth, a_bandwidth_time);
	eventQueueManagement->setTransID(2);

	double a_p_unconditional = 0.1;
	double a_p_conditional = 0.6;
	/*eventQueueManagement.setFECOn();
	eventQueueManagement.setIFECRatio(1.0);
	eventQueueManagement.setPFECRatio(0.5);
	*/
	//eventQueueManagement.setRandomLossProperty(0.30);
	//eventQueueManagement.setBurstyLossProperty(0.04, 5);
	eventQueueManagement->setGilbertLossProperty(a_p_unconditional, a_p_conditional);

	eventQueueManagement->running();
	/*
	int a_queue_limit = 10;
	double round_trip_time = 33;
	NetworkFIFOqueue networkfifoqueue(round_trip_time, a_queue_limit);
	double bandwidth1 = 320;
	double time1 = 0;
	double bandwidth2 = 600;
	double time2 = 100;
	networkfifoqueue.setBandwidth(bandwidth1, time1);
	networkfifoqueue.setBandwidth(bandwidth2, time2);
	double rand_loss_prob = 0.04;
	networkfifoqueue.setRandomLossProperty(rand_loss_prob);
	networkfifoqueue.PrintChannelInfo();
	*/
	/*int randnumber1, randnumber2;
	randnumber1 = rand() %100;
	randnumber2 = rand() %100;
	std::cout << randnumber1<<std::endl;
	std::cout << randnumber2<<std::endl;*/
	/*
	srand((unsigned int)time(0));
	int rnd = rand()%100;
	std::cout << rnd<<std::endl;
	*/
	std::cout << "....End"<<std::endl;
	//std::cin>>v1;

	return 0;



}

