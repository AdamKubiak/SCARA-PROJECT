#include "manipulator.h"

#include <QConeMesh>
#include <QCuboidMesh>
#include <QMesh>
#include <QMesh>
#include <QPhongMaterial>
#include <QTransform>
#include <QNode>

Manipulator::Manipulator( Qt3DCore::QEntity* scene, QObject *parent)
    : QObject(parent), sceneRoot(scene)
{

}

Manipulator::~Manipulator()
{
    if(Base)
    {
        delete Base;
    }
    if(Link1)
    {
        delete Link1;
    }
    if(Link2)
    {
        delete Link2;
    }
}

void Manipulator::createManipulator()
{
    Base = new Render_Object(sceneRoot);
    Link1 = new Render_Object(sceneRoot);
    Link2 = new Render_Object(sceneRoot);
    renderFile = QUrl(QString("file:C:/Users/john/OneDrive/Pulpit/modele/Podstawa_inz.obj"));
    Base->loader()->setSource(renderFile);
    Base->material()->setDiffuse(QColor(QRgb(0xB23149)));
    renderFile = QUrl(QString("file:C:/Users/john/OneDrive/Pulpit/modele/Pierwsze_ramie_inz2.obj"));
    Link1->loader()->setSource(renderFile);
    renderFile = QUrl(QString("file:C:/Users/john/OneDrive/Pulpit/modele/Drugie_ramie_inz2.obj"));
    Link2->loader()->setSource(renderFile);
    Base->transform()->setTranslation(QVector3D(-152,0,-40));
    link1Matrix = Link1->transform()->matrix();

    link2Matrix = Link1->transform()->matrix();
   // Link2->transform()->setTranslation(QVector3D(0,0,93));
    link2Matrix.translate(QVector3D(0,0,93));
    Link2->transform()->setMatrix(link2Matrix);


    q1Rotation = 0;
    q2Rotation = 0;
    //q3Rotation = 90;

   setQ1(0);
   setQ2(0);
   //setQ3(-5);



}


void Manipulator::setQ1(float val)
{
    static float previousQ = 0;

    q1Rotation = val;

    link1Matrix.rotate(q1Rotation-previousQ, 0,1,0);
    //Link1->transform()->matrix();
    Link1->transform()->setMatrix(link1Matrix);

    link2Matrix=link1Matrix;
    link2Matrix.translate(0,0,93);
    link2Matrix.rotate(q2Rotation, 0,1,0);
    //link2Matrix.translate(0,0,-93);
    Link2->transform()->setMatrix(link2Matrix);

    previousQ = q1Rotation;
}

void Manipulator::setQ2(float val)
{
    static int previousQ = 0;
    q2Rotation = val;
    link2Matrix.translate(0,-93,0);
    link2Matrix.rotate(q2Rotation-previousQ, 0,1,0);
    link2Matrix.translate(0,93,0);
    Link2->transform()->setMatrix(link2Matrix);

    previousQ = q2Rotation;
}

void Manipulator::setQ3(int val)
{
    static int previousQ = 0;
    q3Rotation = val;
    //link3Matrix.translate(0,-5,0);
    //link3Matrix.rotate(q3Rotation-previousQ, 1,0,0);
    link3Matrix.translate(0,val,0);
    //link3Transform->setMatrix(link3Matrix);
    previousQ = q3Rotation;
}

