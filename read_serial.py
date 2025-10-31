#!/usr/bin/env python

import serial
import sys
from pprint import pprint

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=74880,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

while 1:
    response = ser.readline()
    if response:
        if sys.argv[1] == '0':
            print(response.decode(), end='')
        elif sys.argv[1] == '1':
            pprint(response)
        else:
            print('incorrect arguments')
