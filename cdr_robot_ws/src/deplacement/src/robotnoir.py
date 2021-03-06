#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
from std_msgs.msg import Float64
from std_msgs.msg import Int16
from sensor_msgs.msg import Range
from std_msgs.msg import Int32MultiArray

class Robot():
   def __init__(self):
      rospy.init_node("test")
      rospy.loginfo("Press CTRL + C to terminate")
      self.speedLeft = rospy.Publisher('/speedLeft', Int16, queue_size=1)
      self.speedRight = rospy.Publisher('/speedRight', Int16, queue_size=1)

      self.go = 1
      self.distance = 30
      self.distance1 = 30
      self.distance2 = 30      
      self.tirette = rospy.Subscriber ("/PinGo", Int16, self.tirette_callback)
      self.ultrasons = rospy.Subscriber("/ultrasound",Int32MultiArray , self.ultrasound_cb)

   
      self.speedLeft.publish(0)       
      self.speedRight.publish(0)

      self.rate = rospy.Rate(10)
      try:
         self.run()
      except rospy.ROSInterruptException:
         rospy.loginfo("exception")
      finally:
         rospy.loginfo("Action finie")

   def stop (self):
      stop = 0
      self.speedLeft.publish(stop)
      self.speedRight.publish(stop)
      
   def tirette_callback(self, msg):
	   self.go = msg.data
   
   def ultrasound_cb(self, msg):
      self.distance1 = msg.data[0]
      self.distance2 = msg.data[1]
      if (self.distance1 < self.distance2):
           self.distance = self.distance1
      else:
	   self.distance = self.distance2
      
      rospy.loginfo("La distance est %i",self.distance)
   
   def straight(self, time, speed):
      begin = rospy.get_time()
      now = rospy.get_time()
      temps_ecoule = 0
      while (now - begin < time):
         if (self.distance < 20):
            self.stop()
            temps_ecoule = rospy.get_time() - now
            rospy.loginfo("Le temps d'attente est de %i",temps_ecoule)
            continue
         self.speedLeft.publish(speed)
         self.speedRight.publish(speed)
         begin += temps_ecoule
         now = rospy.get_time()
         temps_ecoule = 0
      rospy.loginfo("Fin de la ligne droite")
      self.stop()
   
   
   def virage(self, cote,speed):
      begin = rospy.get_rostime()
      now = rospy.get_rostime()
      temps_ecoule = 0
      while (now.secs - begin.secs < 5):
         if (self.distance < 20):
            self.stop()
            temps_ecoule = rospy.get_time() - now.secs
            rospy.loginfo("Le temps d'attente est de %i",temps_ecoule)
            continue
         if (cote == 1):
            self.speedLeft.publish(-speed)
            self.speedRight.publish(-speed/2)
         
         elif (cote == 0):
            self.speedLeft.publish(-speed/2)
            self.speedRight.publish(-speed)

         rospy.loginfo("Il est censé tourner")
         begin.secs += temps_ecoule
         now = rospy.get_rostime()
         temps_ecoule = 0
      rospy.loginfo("Fin du virage")
      self.stop()
   
   def autotour(self, cote, time):
      begin = rospy.get_rostime()
      now = rospy.get_rostime()
      temps_ecoule = 0
      while (now.secs - begin.secs < time):
         if (self.distance < 20):
            self.stop()
            temps_ecoule = rospy.get_time() - now.secs
            rospy.loginfo("Le temps d'attente est de %i",temps_ecoule)
            continue
         if (cote == 1):
            self.speedLeft.publish(-40)
            self.speedRight.publish(0)
         
         elif (cote == 0):
            self.speedLeft.publish(40)
            self.speedRight.publish(0)

         rospy.loginfo("Il est censé tourner")
         begin.secs += temps_ecoule
         now = rospy.get_rostime()
         temps_ecoule = 0
      rospy.loginfo("Fin du virage")
      self.stop()

   def run(self):
      while(self.go == 1):
         rospy.loginfo("J'att")
         self.stop()      
      self.straight(5, 50)
      self.stop()
      self.stop()
      #rospy.spin()
     
if __name__ == '__main__':
    try:
        whatever = Robot()
    except rospy.ROSInterruptException:
        rospy.loginfo("Action termine.")
