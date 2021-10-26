#include "robot.h"
#include <math.h>
#define J1degree 19
#define J2degree 31


Robot::Robot(QObject *parent) : QObject(parent)
{

}

Robot::Robot(float J1, float J2,float sJ1,float sJ2)
{
    this->J1_angle = J1;
    this->J2_angle = J2;

    this->setpoint_J1_angle = sJ1;
    this->setpoint_J2_angle = sJ2;

}

void Robot::encToDegree(int sJ1, int sJ2)
{
    this->J1_angle = roundf(((sJ1-ENC_ZERO)/J1degree) * 1000) / 1000;
    this->J2_angle = roundf(((sJ2-ENC_ZERO)/J2degree) * 1000) / 1000;
    float x = this->J1_angle;
    float y = this->J2_angle;

    qDebug() << x << y;

    //emit encoderPositionChanged(this->J1_angle,this->J2_angle);
}
