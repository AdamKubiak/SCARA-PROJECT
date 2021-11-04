#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include <QDebug>
#include <QVector3D>
#define ENC_ZERO 32768
#define M_PI 3.14159265358979323846


class Robot : public QObject
{
    Q_OBJECT
public:
    explicit Robot(QObject *parent = nullptr);

    Robot(float J1,float J2,float sJ1,float sJ2);
    float setpoint_J1_angle;
    float setpoint_J2_angle;
    float setpoint_J3_angle;
    float J1_angle;
    float J2_angle;
    float J3_angle;
    QVector3D position;
    float setpoint_X_position;
    float setpoint_Y_position;
    float setpoint_Z_position;

    void forwardKinematics(float J1, float J2);
    void fromSlidersToPosition(float slider1_Value, float slider2_Value, float slider3_Value);


signals:
    void encoderPositionChanged(float sJ1, float sJ2);
    void robotPositionChanged(QVector3D position);

public slots:
    void encToDegree(int sJ1,int sJ2);

    //void degreeToPosition(float X,float Y,float Z);

};

#endif // ROBOT_H
