#!/usr/bin/ env python

import time
import serial

ser = serial.Serial(
	port='/dev/ttyACM3',
        baudrate = 2000000,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=5
        )

def readSerial():
    counter=0

    try:
        x = ser.readline()
        return x
    except serial.serialutil.SerialException:
        pass

            
'''
if __name__ == "__main__":
    while 1:
        val = readSerial()
        print val
'''
    
