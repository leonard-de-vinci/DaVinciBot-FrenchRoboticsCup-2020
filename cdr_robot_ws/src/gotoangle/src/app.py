#!/usr/bin/env python
import rospy 
import time
from PID.msg import IntArr
from bot_coordinates.msg import Coordinates
import matplotlib.pyplot as plt
import signal
import sys
import numpy as np

def signal_handler(signal, frame):
  sys.exit(0)

def coordcallback(msg):
    global rightpub, leftpub , V , K, targetAngle, buffer
    if(buffer%10==0):
        alpha = targetAngle - msg.theta
        Vr = V*(np.cos(alpha)+K*np.sin(alpha))  #rad/s
        Vl = V*(np.cos(alpha)-K*np.sin(alpha))  #rad/s
        msg = IntArr()
        msg.cycles = 1000
        msg.ticks = Vr
        rightpub.publish(msg)
        msg.ticks = Vl
        leftpub.publish(msg)
        rospy.loginfo("|| "+str(Vr)+" | "+str(Vl)+" |")
    buffer+=1

if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    global wheelDiameter, entraxe , wheelRadius , l ,K , targetAngle , V
    wheelDiameter = 69.00#mm
    wheelRadius = wheelDiameter/2
    entraxe = 160.00#mm
    l = 160.00#mm? not sure
    K = entraxe/(2*l)#main parameter
    targetAngle = 1
    V = 10.00#mm/s
    global rightpub , leftpub , coordsub , buffer
    buffer = 0
    rospy.init_node("tickviewer", anonymous=False)
    coordsub = rospy.Subscriber("/coords",Coordinates,coordcallback) 
    rightpub = rospy.Publisher("/N1/target",IntArr,queue_size=1)
    leftpub = rospy.Publisher("/N2/target",IntArr,queue_size=1)
    rospy.loginfo("> viewer correctly initialised")
    rospy.spin()