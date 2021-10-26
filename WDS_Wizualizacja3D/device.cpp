#include "device.h"
#include <QDebug>
#include "string.h"
#define M_PI 3.14159265358979323846
#include <math.h>

Device::Device(QObject *parent)
    : QObject(parent)
{
    dataContainer = new DataContainer(0,0,0,0);
    Check = 0;

}

Device::~Device()
{

}


QString &Device::getSerialPortName()
{
    return serialName;
}

QSerialPort *Device::get_SerialPortObject()
{
    return &serialPort;
}

void Device::resetDeviceValues()
{
    orientationVector = QVector3D(0,0,0);
    orientationQauternion = QQuaternion(0,orientationVector);
    positionVelocityVector = QVector3D(0,0,0);
    positionVector = QVector3D(0,0,0);
}



void Device::setSerialName(const QString name)
{
    qDebug()<< name;
    serialName = name;

}

void Device::getData()
{

    while(serialPort.canReadLine()) {
        QString line = serialPort.readLine();
        auto strSplited = line.split(' ');

        if(strSplited.at(0) == 'E')
        {
         if(strSplited.at(4).toDouble() == 55)
         {
        char id = 'E';
        int x = strSplited.at(1).toDouble();
        int y = strSplited.at(2).toDouble();
        int z = strSplited.at(3).toDouble();
        qDebug() << line;
        qDebug() << x << y << z;
        dataContainer->setID(id);
        dataContainer->setX(x);
        dataContainer->setY(y);
        dataContainer->setZ(z);
        qDebug()<<dataContainer->getID()<<dataContainer->getX()<<dataContainer->getY()<<dataContainer->getZ();
         }
        }
        QString terminator = "\r";
        int pos = line.lastIndexOf(terminator);
        }

        emit newDeviceValues(dataContainer->getX(),dataContainer->getY());

        data.clear();
}



void Device::startCommunication()
{

    if(serialName == 0) {
      return;
    }
    serialPort.setPortName(serialName);

    // OTWÓRZ I SKONFIGURUJ PORT:
    if(!serialPort.isOpen())
    {
    if(serialPort.open(QSerialPort::ReadWrite)) {
      serialPort.setBaudRate(QSerialPort::Baud115200);
      serialPort.setDataBits(QSerialPort::Data8);
      serialPort.setParity(QSerialPort::NoParity);
      serialPort.setStopBits(QSerialPort::OneStop);
      serialPort.setFlowControl(QSerialPort::NoFlowControl);
      qDebug()<<"Otwarcie portu szeregowego się powiodło!";
      connect(&serialPort,&QSerialPort::readyRead, this, &Device::getData);

    } else {
      qDebug()<<"Otwarcie portu szeregowego się nie powiodło!";
    }
    }

    else
    {
        qDebug()<<"Port już otwarty";
        return;
    }
}

void Device::sendMessageToDevice(QString message)
{
    if(serialPort.isOpen() && serialPort.isWritable()) {
        //this->addToLogs("Wysyłam informacje do urządzenia " + message);
        serialPort.write(message.toStdString().c_str());
      } else {
        qDebug() <<"Nie mogę wysłać wiadomości. Port nie jest otwarty!";
      }
}

