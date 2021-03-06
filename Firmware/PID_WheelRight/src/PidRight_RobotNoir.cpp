#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <PID_v1.h>
#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float64.h>
//l'Encodeur droit c'est celui qui fonctionne 

int EnA = 2;
int In1 = 3;
int In2 = 4;

int count = 0;
double sens = -1;

Encoder knobLeft(5, 6);
double kp = 13 , ki = 0 , kd = 0;            

ros::NodeHandle nh;

double input2, output2, setpoint2; 
PID myPID2(&input2, &output2, &setpoint2, kp, ki, kd, DIRECT); 


void messageCb( const std_msgs::Int16& toggle_msg){
  setpoint2=toggle_msg.data;
  if (setpoint2 < 0){
    digitalWrite(In1, HIGH);   
    digitalWrite(In2, LOW);
    sens = 1;
    setpoint2 = -setpoint2;

  }
  else if (setpoint2 > 0){
    digitalWrite(In1, LOW);   
    digitalWrite(In2, HIGH);
    sens = -1;
  }
}

std_msgs::Float64 enco_right;
ros::Publisher encoder_right("encoRight", &enco_right);

ros::Subscriber<std_msgs::Int16> sub("/speedRight", &messageCb );

void setup() {

  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(encoder_right);
  pinMode(EnA, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);

  digitalWrite(In1, LOW);   
  digitalWrite(In2, HIGH);   //Avance dans le bon sens

  input2 = 0;
  setpoint2 = 0;

  myPID2.SetMode(AUTOMATIC);
  myPID2.SetSampleTime(1); //Fréquence du PID dans le loop
  myPID2.SetOutputLimits(0, 200); //Va fixer le PWM entre -400 et 400 comme sur nos moteurs

  //Serial.begin (115200);                              //DEBUG
}

void asservissement(double cible, bool arret)
{
  setpoint2=cible;
  input2 = knobLeft.read() * sens;
  
  
  
  if (myPID2.Compute()) {
    /*
    Serial.print(input2);
    Serial.print(" , ");
    Serial.print(" , ");
    Serial.print(setpoint2);
    Serial.println();
    */
    if (output2 >= 0 && setpoint2!=0) {
      analogWrite(EnA, output2);
    }
    /*
    else if (output2 < 0 && setpoint2!=0) {
      analogWrite(EnA,output2);
    }
    */
    else if (setpoint2==0) {
      analogWrite(EnA,0);
    }

    enco_right.data= input2;
    knobLeft.write(0);
  }
}

void loop()
{
  nh.spinOnce();
  delay(1);
  asservissement(setpoint2, false);
  count++;
  if (count == 10){
    encoder_right.publish(&enco_right);
    count = 0;
  }
}