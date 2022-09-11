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
    this->J3_angle = 0.0;

    this->setpoint_J1_angle = sJ1;
    this->setpoint_J2_angle = sJ2;
    this->setpoint_J3_angle = 0.0;

}

void Robot::forwardKinematics(float J1, float J2, float J3)
{
    this->position.setX(A2*cos((J1/180)*M_PI) + A4*cos((J1 + J2)/180*M_PI));
    this->position.setY(A2*sin((J1/180)*M_PI) + A4*sin((J1 + J2)/180*M_PI));
    this->position.setZ(fromAngleToPositionServo(J3));

    emit robotPositionChanged(this->position);
}

void Robot::inverseKinematics(double X, double Y, double Z,bool mode)
{
    float r,phi1,phi2,phi3, T1,T2,T3;
    std::pair<float,float> constraints;
      r = sqrt((X*X)+(Y*Y));
      if(!mode)
      {
      phi1 = acos(((A4*A4)-(A2*A2)-(r*r))/(-2.0*A2*r));
      phi2 = atan(Y/X);
      phi3 = acos(((r*r)-(A2*A2)-(A4*A4))/(-2.0*A2*A4));
      T1 = phi2-phi1;
      T2 = (M_PI-phi3);
      T3 = Z;
      }
      else
      {
          phi1 = -acos(((A4*A4)-(A2*A2)-(r*r))/(-2.0*A2*r));
          phi2 = atan(Y/X);
          phi3 = acos(((r*r)-(A2*A2)-(A4*A4))/(-2.0*A2*A4));
          T1 = phi2-phi1;
          T2 = -(M_PI-phi3);
          T3 = Z;
      }


      if(!isnan(T1) && !isnan(T1))
      {
      T1 = T1/M_PI*180;// %jesli X na - to wtedy dodajemy 180 stopni
      if(X<0) T1 = T1+180;
      T2 = T2/M_PI*180;
      //T3 = T3/M_PI*180;
      T1 = constrain(T1, J1_MIN,J1_MAX);
      constraints = constraintJ2(T1);
      T2 = constrain(T2, constraints.first,constraints.second);

      this->setpoint_J1_angle = (T1 - this->J1_angle)*J1degree;
      this->setpoint_J2_angle = ((T2 - this->J2_angle)*J2degree + (setpoint_J1_angle/J1degree)*AXIS2_AXIS1_correction*J2degree);
      this->setpoint_J3_angle = ((7600.0-1600.0)/(4.0))*(T3)+1600.0;
      }
      else{
          QMessageBox::information(NULL,"Bład", "Zadana pozycja efektora jest nieosiągalna");
      }
}

void Robot::encToDegree(int sJ1, int sJ2, int sJ3)
{
    sJ1 = (float)sJ1;
    sJ2 = (float)sJ2;
    sJ3 = (float)sJ3;
    this->J1_angle = (sJ1-ENC_ZERO)/J1degree;//roundf(((sJ1-ENC_ZERO)/J1degree) * 1000) / 1000;
    this->J2_angle = (sJ2-ENC_ZERO)/J2degree - J1_angle*AXIS2_AXIS1_correction;//roundf(((sJ2-ENC_ZERO)/J2degree - J1_angle*AXIS2_AXIS1_correction) * 1000) / 1000;
    this->J3_angle =  (180.0/6000.0)*(sJ3-1600.0);

    //qDebug() << x << y;

    emit encoderPositionChanged(this->J1_angle,this->J2_angle, this->J3_angle);
    forwardKinematics(this->J1_angle, this->J2_angle,this->J3_angle);
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

void Robot::fromSpinboxToPosition(double spinbox1_Value, double spinbox2_Value, double spinbox3_Value,bool mode)
{
    inverseKinematics(spinbox1_Value,spinbox2_Value,spinbox3_Value,mode);
}

int Robot::fromAngleToPWMForward(float angle)
{
        angle = ((7600.0-1600.0)/(180.0))*(angle)+1600.0;
        return angle;
}

int Robot::fromAngleToPWMInverse(float position)
{
          position = ((180.0)/(4.0))*(position);

          return position;
}

float Robot::fromAngleToPositionServo(float angle)
{

          angle = ((4.0)/(180.0))*(angle);

          return angle;
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


