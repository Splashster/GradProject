#!/usr/bin/ env python

'''
The puropse of this program is to read and return data from a serial port on 
the device that's running this program.
The readSerial function takes a port name and a baudrate as parameters
'''


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

            
    
