#!/usr/bin/python3
# -*- coding: utf-8 -*-

from binserial import BinSerial

if __name__ == "__main__":
    bser = BinSerial("/dev/ttyACM0", 9600)
    bser.write(['uint16', 'uint16', 'uint8', 'uint8', 'uint8'], [1, 2, 3, 4, 5])
    bser.write(['uint16', 'uint16', 'uint8', 'uint8', 'uint8'], [1, 2, 3, 4, 5])
    print(bser.read(['uint16', 'uint16', 'uint8', 'uint8', 'uint8']))
    # bser.write(['bool'], [True])
    # bser.write(['int16'], [int(input('PWM: '))])
    # while True:
    #     print(bser.read(['float']*2))
