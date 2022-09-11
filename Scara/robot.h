#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include <QDebug>
#include <QVector3D>
#include <QMessageBox>
#define ENC_ZERO 32768
#define M_PI 3.14159265358979323846
#define J1_MIN -10.0
#define J1_MAX 190.0
#define J2_MIN -130.0
#define J2_MAX 130.0
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))




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

    void forwardKinematics(float J1, float J2,float J3);
    void inverseKinematics(double X, double Y, double Z,bool mode);
    void fromSlidersToPosition(float slider1_Value, float slider2_Value, float slider3_Value);
    void fromSpinboxToPosition(double spinbox1_Value, double spinbox2_Value, double spinbox3_Value,bool mode);
    int fromAngleToPWMForward(float angle);
    int fromAngleToPWMInverse(float position);
    float fromAngleToPositionServo(float angle);
    std::pair<float,float> constraintJ2(float J1);


signals:
    void encoderPositionChanged(float sJ1, float sJ2,float sJ3);
    void robotPositionChanged(QVector3D position);

public slots:
    void encToDegree(int sJ1,int sJ2,int sJ3);

    //void degreeToPosition(float X,float Y,float Z);

};

#endif // ROBOT_H
