#!/usr/bin/ env python

import time
import serial


def readSerial(portName, br):
    ser = serial.Serial(
            port=portName,
            baudrate = br,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=5
            )
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
    
