clear;
stringTimeDir1 = 'E:\simulation\result\status_time.txt';
[infoMatrix1, infoMatrixTotalLen1] = getInfoMatrix(stringTimeDir1);
stringRateDir2 = 'E:\simulation\result\status_rate.txt';
[infoMatrix2, infoMatrixTotalLen2] = getInfoMatrix(stringRateDir2);
stringFrameDir3 = 'E:\simulation\result\status_decoded_frame_num.txt';
[infoMatrix3, infoMatrixTotalLen3] = getInfoMatrix(stringFrameDir3);
stringRateDir4 = 'E:\simulation\result\status_upper_rate.txt';
[infoMatrix4, infoMatrixTotalLen4] = getInfoMatrix(stringRateDir4);


stringRateDir6 = 'E:\simulation\result\status_rate_decoded.txt';
[infoMatrix6, infoMatrixTotalLen6] = getInfoMatrix(stringRateDir6);
stringRateDir7 = 'E:\simulation\result\status_frame_num_decoded.txt';
[infoMatrix7, infoMatrixTotalLen7] = getInfoMatrix(stringRateDir7);




figure(1)
plot(infoMatrix1,infoMatrix2,'b-.','LineWidth',2);
hold on
plot(infoMatrix1,infoMatrix6,'r-.','LineWidth',2);
plot(infoMatrix1,infoMatrix4,'y-.','LineWidth',2);
legend('Display Rate','Decodable Rate','Rate Upper Bound');
xlabel('Duration Time (ms)')
ylabel('Rate (bps)')
xlim([0 10000])
figure(2)
plot(infoMatrix1,infoMatrix3,'b-.','LineWidth',2);
hold on 
plot(infoMatrix1,infoMatrix7,'r-.','LineWidth',2);
legend('Display Frame','Decodable Frame');
xlabel('Duration Time (ms)')
ylabel('Frame Number')
xlim([0 10000])
ylim([0 5])

% stringRateDir5 = 'E:\simulation\result\trans_time.txt';
% [infoMatrix5, infoMatrixTotalLen5] = getInfoTimeMatrix(stringRateDir5);
% [time_prob, time] = ecdf(infoMatrix5);
% figure(3)
% plot(time,time_prob);
% xlabel('Retransmission Num')
% ylabel('CDF')

