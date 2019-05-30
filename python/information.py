#!/usr/bin/python3
# -*- coding: utf-8 -*-

from binserial import BinSerial

if __name__ == "__main__":
    bser = BinSerial("/dev/ttyACM1", 9600)
    bser.write(['float']*3+['bool'], [0, 0, 0, True])
    while True:
        # print(bser.read(['float']*3+['bool']))
        bser.write(['float']*3+['bool'], [float(input('x: ')), 0, 0, False])
