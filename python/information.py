#!/usr/bin/python3
# -*- coding: utf-8 -*-

from binserial import BinSerial
import threading


def print_odometry():
    while True:
        print(bser.read(['float']*3+['bool']))


if __name__ == "__main__":
    # Initialize serial
    bser = BinSerial("/dev/ttyACM1", 9600)

    # Prepare odometry thread
    odometry_thread = threading.Thread(target=print_odometry, daemon=True)

    # Sent initial position to the robot (True)
    bser.write(['float']*3+['bool'], [0, 0, 0, True])

    # Start odometry thread
    odometry_thread.start()

    while True:
        # Sent setpoint position to the robot (False)
        bser.write(['float']*3+['bool'], [float(input('x: ')), 0, 0, False])
