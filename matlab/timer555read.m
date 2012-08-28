% Communications MatLab <--> Arduino
% Matlab file 1 for use with Arduino file 1
clc;
clear all;
numSec=1;
t=[];
v=[];
B=3988;
rinf=10e3*exp(-B/298.15);

C=1e-6;
r_1=5e3;
s1 = serial('COM5');    % define serial port
s1.BaudRate=9600;               % define baud rate
set(s1, 'terminator', 'LF');    % define the terminator for println
fopen(s1);

try                             % use try catch to ensure fclose
    for i=1:10                               % signal the arduino to start collection
	period=fscanf(s1,'%d');
    r_T=(period*1e-6)/(2*log(2)*C)-r_1/2;
    
    
    
    T=B/log(r_T/rinf)-273.15;
    display(period);
    display(T);
    end                                                                                                                                                                                                                                                           
fclose(s1);
catch exception
    fclose(s1);                 % always, always want to close s1
    throw (exception);
end     
