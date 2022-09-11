#include "datacontainer.h"

DataContainer::DataContainer()
{

}

DataContainer::DataContainer(char _ID,int _X,int _Y,int _Z)
{
    this->ID = _ID;
    this->X = _X;
    this->Y = _Y;
    this->Z = _Z;
}

void DataContainer::setX(int &X_)
{
    X = X_;
}

void DataContainer::setY(int &Y_)
{
    Y = Y_;
}

void DataContainer::setZ(int &Z_)
{
    Z = Z_;
}

void DataContainer::setID(char &ID_)
{
    ID = ID_;
}

int DataContainer::getX()
{
    return X;
}

int DataContainer::getY()
{
    return Y;
}

int DataContainer::getZ()
{
    return Z;
}

char DataContainer::getID()
{
    return ID;
}

