#!/usr/bin/env python
import rospy 
import time
from PID.msg import speed
from bot_coordinates.msg import Coordinates
from PID.msg import FloatArr
from std_msgs.msg import Bool
import signal
import sys
import numpy as np


def signal_handler(signal, frame):
    sys.exit(0)


if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    global rightpub, leftpub
    rospy.init_node("tster", anonymous=False)
    rightpub = rospy.Publisher("/N1/target", speed, queue_size=2)
    print("waiting for full init")
    time.sleep(15)
    msg = speed()
    msg.dir = True
    msg.ticks = 0
    rightpub.publish(msg)
    print("ON TEST BENCH")
    print("test for motor connectivity")
    print("when ready press enter:")
    while True:
        msg.dir = True
        msg.ticks = 30
        rightpub.publish(msg)
        time.sleep(0.2)
        print("--------------------------------------------")
        print("RIGHT FORWARD")
        time.sleep(2)
        msg.dir = False
        msg.ticks = 30
        rightpub.publish(msg)
        time.sleep(0.2)
        print("--------------------------------------------")
        print("RIGHT BACKWARD")
        time.sleep(2)