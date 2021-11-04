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
    //static float previous = 0;
    this->setpoint_J1_angle = (slider1_Value - this->J1_angle)*J1degree;
    this->setpoint_J2_angle = ((slider2_Value - this->J2_angle)*J2degree + (setpoint_J1_angle/J1degree)*AXIS2_AXIS1_correction*J2degree);


   /* if(previous == slider1_Value)
    {
        this->setpoint_J2_angle = (slider2_Value - this->J2_angle + (setpoint_J1_angle/J1degree)*AXIS2_AXIS1_correction)*J2degree;
    }
    else
    {
        this->setpoint_J2_angle = (slider2_Value - this->J2_angle)*J2degree;
    }*/

    this->setpoint_J3_angle = slider3_Value;
    //previous = slider1_Value;
   // return QVector3D((int)setpoint_J1_angle,(int)setpoint_J2_angle,(int)setpoint_J3_angle);
}


