#!/usr/bin/python3
# -*- coding: utf-8 -*-

from binserial import BinSerial

if __name__ == "__main__":
    bser = BinSerial("/dev/ttyACM0", 9600)
    bser.write(['bool'], [True])
    # bser.write(['int16'], [int(input('PWM: '))])
    while True:
        # print(bser.read(['float']*3))
        input()
        bser.write(['bool'], [False])
        input()
        bser.write(['bool'], [True])
