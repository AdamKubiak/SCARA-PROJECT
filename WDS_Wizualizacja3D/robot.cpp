#include "robot.h"
#include <math.h>
#define J1degree 18.425 // 19
#define J2degree 29.807// 31
#define AXIS2_AXIS1_correction 33.0/62.0
#define A2 9.4
#define A4 11.0

Robot::Robot(QObject *parent) : QObject(parent)
{

}

Robot::Robot(float J1, float J2,float sJ1,float sJ2)
{
    this->J1_angle = J1;
    this->J2_angle = J2;
    this->J3_angle = 0;

    this->setpoint_J1_angle = sJ1;
    this->setpoint_J2_angle = sJ2;

}

void Robot::forwardKinematics(float J1, float J2)
{
    this->position.setX(A2*cos((J1/180)*M_PI) + A4*cos((J1 + J2)/180*M_PI));
    this->position.setY(A2*sin((J1/180)*M_PI) + A4*sin((J1 + J2)/180*M_PI));
   this->position.setZ(0);

    emit robotPositionChanged(this->position);
}

void Robot::inverseKinematics(double X, double Y, double Z)
{
    float r,phi1,phi2,phi3, T1,T2,T3;
    std::pair<float,float> constraints;
      r = sqrt((X*X)+(Y*Y));
      phi1 = acos(((A4*A4)-(A2*A2)-(r*r))/(-2.0*A2*r));
      phi2 = atan(Y/X);
      phi3 = acos(((r*r)-(A2*A2)-(A4*A4))/(-2.0*A2*A4));
      T1 = phi2-phi1;
      T2 = (M_PI-phi3);
      T3 = Z;


      T1 = T1/M_PI*180;// %jesli X na - to wtedy dodajemy 180 stopni
      if(X<0) T1 = T1+180;
      T2 = T2/M_PI*180;
      T3 = T3/M_PI*180;

      T1 = constrain(T1, J1_MIN,J1_MAX);
      constraints = constraintJ2(T1);
      T2 = constrain(T2, constraints.first,constraints.second);
      this->setpoint_J1_angle = (T1 - this->J1_angle)*J1degree;
      this->setpoint_J2_angle = ((T2 - this->J2_angle)*J2degree + (setpoint_J1_angle/J1degree)*AXIS2_AXIS1_correction*J2degree);
      this->setpoint_J3_angle = T3;
}

void Robot::encToDegree(int sJ1, int sJ2)
{
    sJ1 = (float)sJ1;
    sJ2 = (float)sJ2;
    this->J1_angle = (sJ1-ENC_ZERO)/J1degree;//roundf(((sJ1-ENC_ZERO)/J1degree) * 1000) / 1000;
    this->J2_angle = (sJ2-ENC_ZERO)/J2degree - J1_angle*AXIS2_AXIS1_correction;//roundf(((sJ2-ENC_ZERO)/J2degree - J1_angle*AXIS2_AXIS1_correction) * 1000) / 1000;
    //float x = this->J1_angle;
    //float y = this->J2_angle;

    //qDebug() << x << y;

    emit encoderPositionChanged(this->J1_angle,this->J2_angle);
    forwardKinematics(this->J1_angle, this->J2_angle);
}

void Robot::fromSlidersToPosition(float slider1_Value, float slider2_Value, float slider3_Value)
{
    std::pair<float,float> constraints;
    slider1_Value = constrain(slider1_Value, J1_MIN,J1_MAX);
    constraints = constraintJ2(slider1_Value);
    slider2_Value = constrain(slider2_Value, constraints.first,constraints.second);

    this->setpoint_J1_angle = (slider1_Value - this->J1_angle)*J1degree;
    this->setpoint_J2_angle = ((slider2_Value - this->J2_angle)*J2degree + (setpoint_J1_angle/J1degree)*AXIS2_AXIS1_correction*J2degree);
    this->setpoint_J3_angle = fromAngleToPWMForward(slider3_Value);
}

void Robot::fromSpinboxToPosition(double spinbox1_Value, double spinbox2_Value, double spinbox3_Value)
{
    inverseKinematics(spinbox1_Value,spinbox2_Value,spinbox3_Value);
}

int Robot::fromAngleToPWMForward(float angle)
{

        float  value;
        int minC=1600;
        int maxC=7600;
        int minA=0;
        int maxA=180;
        value = ((maxC-minC)/(maxA-minA))*(angle-minA)+minC;
        return value;
}

std::pair<float,float> Robot::constraintJ2(float J1)
{
   std::pair<float,float> constrains;
   if(J1<50)
   {
   float minA=-10;
   float maxA=50;
   float minC=-85;
   float maxC=-130;
   constrains.first= ((maxC-minC)/(maxA-minA))*(J1-minA)+minC;
   constrains.second = 130;

   return constrains;
   }

   if(J1>130)
   {
       float minA=190;
       float maxA=130;
       float minC=85;
       float maxC=130;
       constrains.first= -130;
       constrains.second = ((maxC-minC)/(maxA-minA))*(J1-minA)+minC;
       return constrains;
   }

   if(J1>=50 && J1<=130)
   {
       constrains.first= -130;
       constrains.second = 130;
       return constrains;

   }
}


