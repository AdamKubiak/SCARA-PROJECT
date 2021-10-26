#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include <QDebug>
#define ENC_ZERO 32768

class Robot : public QObject
{
    Q_OBJECT
public:
    explicit Robot(QObject *parent = nullptr);

    Robot(float J1,float J2,float sJ1,float sJ2);
    float setpoint_J1_angle;
    float setpoint_J2_angle;
    float J1_angle;
    float J2_angle;


signals:
    //void encoderPositionChanged(float sJ1, float sJ2);

public slots:
    void encToDegree(int sJ1,int sJ2);

};

#endif // ROBOT_H
