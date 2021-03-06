% Communications MatLab <--> Arduino
% Matlab file 1 for use with Arduino file 1
clc;
clear all;
numSec=1;
t=[];
v=[];
B=3988;
rinf=10e3*exp(-B/298.15);

s1 = serial('COM3');    % define serial port
s1.BaudRate=9600;               % define baud rate
set(s1, 'terminator', 'LF');    % define the terminator for println
fopen(s1);

try                             % use try catch to ensure fclose
    for i=1:100                               % signal the arduino to start collection
	w=fscanf(s1,'%d');
    vdiff=(w*5/1023);
    resistance=100e3/(2*vdiff+5) -10e3;
    %display(resistance);
    T=B/log(resistance/rinf)-273.15;
    display(T);
    end                                                                                                                                                                                                                                                           
fclose(s1);
catch exception
    fclose(s1);                 % always, always want to close s1
    throw (exception);
end     
