========================================================================
    CONSOLE APPLICATION : videoTransmissionSimulation Project Overview
========================================================================

AppWizard has created this videoTransmissionSimulation application for you.

This file contains a summary of what you will find in each of the files that
make up your videoTransmissionSimulation application.


videoTransmissionSimulation.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

videoTransmissionSimulation.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

videoTransmissionSimulation.cpp
    This is the main application source file. You can change a lot of settings in that file.

channelProperty.h
It is used to record channel condition, RTT, BW, loss, etc.

EventFIFOqueue.h  eventNode.h  EventQueueManagement_version4.h
They are used to record the event in the simulation. Event could be "packet ready to transmit", "packet time out", "packet received", etc.

networkFIFOqueue.h
It is used to simulate the condition of network queue. For example, packet order in network, whether queue loss would happen, etc.

rateID.h
It is used to record the rate and layer number relation

RTTUpdate.h timeoutUpdate.h
It is used to update RTT value and timeout value information

sendingWindow.h
It is used to record the sending window behavior in the sender side.

unAckedQueue.h
It is used to record the un-acked queue information

videoPreordersvc.h  videoPreordersvc.h
It is used to read "pkt.txt" information

videoDecoder.h
It is used to simulate video receiver side behavior.

videoFrame.h videopkt.h
It is used to record video frame and video packet.



/////////////////////////////////////
In "videoTransmissionSimulation.cpp" file, you can change a lot of settings in the simulation:

1. string infor_base_directory = "E:\\simulation";

This is the directory that this program would try to read and write.
In this example, the program would try to read two input files: "E:\\simulation\\data\\pkt.txt" and "E:\\simulation\\data\\rate.txt". You Need to make sure that these two input files are in the appropriate directory before running the program. It would generate 10 output files in directory "E:\\simulation\\result". The format and the meaning of each files would be talked later.

2. double round_trip_time = 20;

This is the network round-trip-time setting in the simulation. Here it means that network RTT is 20 ms.

3. double time_out_duration = 60;

This is the time-out-time. This is the time that if sender doesn't receive ACK of the packet after sending it, the sent out packet is considerred to be lost.

4. int a_queue_limit_ = 10;

This is the queue size of the simulated network, the maximum packet number that the network could hold.

5. double measuredBandwidth = 1000;

This is the bandwidth that the program that measured. This simulation has no network measurement component. Thus, this is just the number that we assume the program gets from the network measurement component. In this example, we assume that the measuredBandwidth is 1000 kbps.

6. double repeatTime = 1;

In setting 1, we assume we would get packet data from "E:\\simulation\\data\\pkt.txt". repeatTime means the time that we would repeat to tranfer those packet data. Like if the frames in "E:\\simulation\\data\\pkt.txt" are frameA, frameB, frameC and repeatTime = 2, then the program would transfer frameA, frameB, frameC, frameA, frameB, frameC.

7. int frame_num_decoder = 300;

This is the frame number that file "E:\\simulation\\data\\pkt.txt" in setting1 would have. Basically, that is just the total line number that file has.

8. double deadline = 200;

This is duration we would try to decode the received frame in the receiver side after that frame has been generated in the sender side.

9. int turnFECOn = 0;

This means the technology that you would use in the transmission. If "int turnFECOn = 0;", it means that it would use retransmission to do loss recovery. And in this setting, "float iFECRatio = 1.0; float pFECRatio = 0.5;" has no meaning.

If ""int turnFECOn = 0;", it means that it would use FEC to do packet protection. We only do FEC to protect the layer 0 packets. "float iFECRatio = 1.0" means that for I frame, if it is also layer 0 frame, the FEC ratio for this frame would be 1.0. If the original packet number is 5, then the total packet number would be 10 if FEC is added. "float pFECRatio = 0.5;" means that for P frame, if it is also layer 0 frame, the FEC ratio for this frame would be 0.5.

10. int fps = 30;

This means the origianl video frame FPS in "E:\\simulation\\data\\pkt.txt" (setting 1 example) is 30.

11. int pktsize = 1400;

This means the packet size we would use in the simulation. In this example, it means that we would try to formulate packet with size 1400 bytes. The last packet size of the frame may be smaller than 1400 bytes. But other packets size should be always 1400 bytes.

12. 	double a_bandwidth = 1000;
	double a_bandwidth_time = 0;	
	eventQueueManagement->setBandwidth(a_bandwidth, a_bandwidth_time);

This means the true bandwidth that network would have. In this example, we assume the network bandwidth would be constant forever and the actual bandwidth is 1000kbps. You can also simulate the varying network bandwidth. For example, if the setting is network bandwidth being 1000 kbps from start to 400 ms and then becoming 500 kbps, the setting could be set as below:

double a_bandwidth = 1000;
double a_bandwidth_time = 0;	
eventQueueManagement->setBandwidth(a_bandwidth, a_bandwidth_time); 
a_bandwidth = 500;
a_bandwidth_time = 400;	
eventQueueManagement->setBandwidth(a_bandwidth, a_bandwidth_time);

 
13. eventQueueManagement->setTransID(2);

This means that the highest layer that the program would try to transmit. In this example, it means that the program would try to tranmit layer 0, layer 1 and layer 2 packets.

14. 	double a_p_unconditional = 0.1;
	double a_p_conditional = 0.6; 
	eventQueueManagement->setGilbertLossProperty(a_p_unconditional, a_p_conditional);

This means how we set the Gilbert Loss Model. a_p_unconditional means p_u and a_p_conditional means p_c in the model.

15.     eventQueueManagement.setRandomLossProperty(0.30);

If you don't want to use Gilber Loss Model, you could also use the random loss model. In this example, the random loss probability is set to be 30%.
     
/////////////////////////////////////
Input File && Output File

I listed the sample input files and output files in the directory "file sample".
Input files are in directory "//file sample//data" 

"//file sample//data//pkt.txt" record the information of the frames sent out. For example, the first two lines of the sample file are:
"0 0 I 121712
4 0 P 17032"
The first row means the order of the frames generated. The second row means the layer number that frame belongs to. The third row means whether this frame is "I" frame or "P" frame. The fourth row means the size of that frame.
So, the first line means that it is the first frame and it belongs to layer 0. Its frame type is "I" and its size is 121712 bytes. The second line means that it is the 5-th frame and it belongs to layer 0. Its frame type is "P" and its size is 17032 bytes.

"//file sample//data//rate.txt" record the rate information of the layer. In the sample file, we have three lines:
"0 190.00
1 276.00
2 400.50"
It means that the rate of layer 0 is 190.00 kbps. The rate of layer 0 and layer 1 is 276.00 kbps. The rate of layer 0, layer 1 and layer 2 is 400.50 kbps.


In directory "//file sample//result", I list most of the resulting files.
   
"frame_decode.txt" records the frame decoding information, whether the specific frame could be displayed ("currentDecodedFrame" item in the file) or decodable ("DecodedFrame" item in the file).

"status_decoded_frame_num.txt" records the frame number that could be displayed in one GOP. While "status_frame_num_decoded.txt" records the frame number that could be decodable in one GOP. Thus, the later one couldn't be smaller than the former one.

"status_rate.txt" records the displayed packets rate in one GOP. "status_rate_decoded.txt" records the decodable packets rate in one GOP. The later one couldn't be smaller than the former one. "status_upper_rate.txt" records the packet rate upper bound in one GOP. That is the rate for the frame generated.


"status_time.txt" records the GOP generating time. Bascially that is the third packet generating time in one GOP. It is used for plotting "time-xxxx(rate, num, etc)" relation graph.

"trans_time.txt" records for each frame, how many times the program would try to transmit it.

In addition, the program will generate one file called "status.txt", which I didn't list in the directory. That file records the running details of the simulation.


In directory "//file sample//sampleMatlab", I list the sample MATLAB file using those files to do graph plot. 
  


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named videoTransmissionSimulation.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
