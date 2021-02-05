#!/usr/bin/env python
import rospy
import time
from PID.msg import FloatArr
from std_msgs.msg import Bool
from bot_coordinates.msg import command
from bot_coordinates.msg import move
from PID.msg import FloatArr
from std_msgs.msg import Int8
import signal
import sys
import numpy as np


def signal_handler(signal, frame):
    sys.exit(0)


def commandCallback(msg):
    global thestack, me
    if msg.destination == me:
        thestack.append((msg.sender, msg.order, msg.precision))


def mainloop():
    global thestack, precision, state, actionpos, me
    if state == 0:
        # turn on the mcontrol
        commsg = command()
        commsg.sender = me
        commsg.destination = "mcontrol"
        commsg.order = 1
        commsg.precision = 1  # here the precision refers to the rate buffer
        # trun on the gotogoal
        commsg = command()
        commsg.sender = me
        commsg.destination = "gotogoal"
        commsg.order = 1
        commsg.precision = 0  # here precision refers to the mod
        state += 1
    if state == 1:
        # TODO : wait for the start from the arduino
        # if len(thestack) > 0:
        #     a = thestack.pop()
        #     if a == ("start", 1, 2) or a == ("start", 1, 1):
        #         state += 1
        #         (_, __, precision) = a
        state += 1
        precision = 1
    if state == 2:
        # TODO load teh correct file according to the chosen strategie
        if precision == 1:  # laod the right file
            pass
        elif precision == 2:  # load the left file
            pass
        state += 1
    if state == 3:  # start ieration
        if actionpos >= len(waypoints):
            state += 1
        else:
            rospy.loginfo("action n "+(str)(actionpos))
            currentaction = waypoints[actionpos]
            senderid = currentaction[0]
            # ##------------------- what are we waiting for
            waiting = currentaction[1]
            # ##------------------- hwo are we waiting for
            if senderid == 1:
                sender = "mcontrol"
            elif senderid == 2:
                sender = "gotogoal"
            # ##--------------------now that we kno what we are waiting for we check if its teh case
            skip = False
            if len(thestack) > 0:  # obtain latest msg
                (sender, order, precision) = thestack.pop()
                if sender == senderid and waiting == order:
                    actionpos += 1
                    skip = True
            if not skip:  # we need to stimulate a response
                if sender == "gotogoal":
                    global waypointpub
                    msg = FloatArr()
                    msg.X = currentaction[2]
                    msg.Y = currentaction[3]
                    msg.theta = currentaction[4]
                    msg.epsilon = currentaction[5]
                    waypointpub.publish(msg)
                    cmsg = command()
                    cmsg.sender = me
                    cmsg.destination = sender
                    cmsg.order = currentaction[6]
                    cmsg.precision = currentaction[7]
                    rospy.loginfo("published a waypoint")
                elif sender == "start":
                    # TODO implement the control of the servos and shit
                    pass
    if state >= 4:  # go back home because end
        currentaction = waypoints[len(waypoints)-1]  # this line needs the last value of the waypoints to be the coord of home
        msg = FloatArr()
        msg.X = currentaction[2]
        msg.Y = currentaction[3]
        msg.theta = currentaction[4]
        msg.epsilon = currentaction[5]
        waypointpub.publish(msg)
        rospy.loginfo("go back home quick")


if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    # ##---------------------waypoints and stuff
    global waypoints
    waypoints = np.array([[1, 1, 1, 1, 1, 1, 1, 1],
                          [1, 1, 1, 1, 1, 1, 1, 1],
                          [1, 1, 1, 1, 1, 1, 1, 1],
                          [1, 1, 1, 1, 1, 1, 1, 1]])
    # ##---------------------logique
    global blocked, waiting, sender, me, precision, order, state, actionpos, thestack
    blocked = False
    thestack = []
    actionpos = 0
    waiting = 0  # what we waiting for
    sender = "null"  # from whom are we waiting a message
    me = "brain"
    precision = 0
    order = 0
    state = 0

    # ##---------------------ROS
    global emergencystop, commandpub, commandsub, waypointpub, guisub
    rospy.init_node("thebrain", anonymous=False)
    emergencystop = rospy.Publisher("/breakServo", Bool, queue_size=1)       # pub for emergency break
    commandpub = rospy.Publisher("/control", command, queue_size=1)          # pub for commanding teh nodes
    commandsub = rospy.Subscriber("/control", command, commandCallback)      # sub for teh commands
    waypointpub = rospy.Publisher("/target", FloatArr, queue_size=1)           # pub for teh waypoints from actions
    rospy.loginfo(">  the brain has been succesfully initialised")
    while True:
        mainloop()
        time.sleep(1)
