#!/usr/bin/ env python

import time
import serial

ser = serial.Serial(
	port='/dev/ttyACM0',
        baudrate = 115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
        )
counter=0

def readSerial():
    x = ser.readline()
    print x
    return x

if __name__ == "__main__":
    while 1:
        val = readSerial()
        #print val

    
