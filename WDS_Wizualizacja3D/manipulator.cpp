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
    if(base)
    {
        delete base;
    }

    if(base1)
    {
        delete base1;
    }

    if(link1)
    {
        delete link1;
    }
    if(link2)
    {
        delete link2;
    }
    if(link3)
    {
        delete link3;
    }
}

void Manipulator::createManipulator()
{
    /*!
     * \brief cuboid mesh
     */
    Qt3DExtras::QPhongMaterial *manipulatorMaterial = new Qt3DExtras::QPhongMaterial();
    manipulatorMaterial->setDiffuse(QColor(QRgb(0xB23149)));
    baseMesh = new Qt3DExtras::QCylinderMesh();
    baseMesh->setLength(1);
    baseMesh->setRadius(5);
    base = new Qt3DCore::QEntity();
    base->addComponent(baseMesh);
    base->addComponent(manipulatorMaterial);
    base->setParent(sceneRoot);

    /*!
     * \brief cuboid mesh base 1
     */
    baseMesh1 = new Qt3DExtras::QCylinderMesh();
    baseMesh1->setLength(11);
    baseMesh1->setRadius(1.5);
    //baseMesh1->setXExtent(2);
    //baseMesh1->setYExtent(10);
    //baseMesh1->setZExtent(2);
    base1 = new Qt3DCore::QEntity();
    base1Transform = new Qt3DCore::QTransform();
    base1Transform->setTranslation(QVector3D(0,5.5,0));
    base1Matrix = base1Transform->matrix();

    base1->addComponent(baseMesh1);
    base1->addComponent(manipulatorMaterial);
    base1->addComponent(base1Transform);
    base1->setParent(sceneRoot);

    /*!
     * First joint
     */
    link1Mesh = new Qt3DExtras::QCuboidMesh();
    link1Mesh->setXExtent(2);
    link1Mesh->setYExtent(10);
    link1Mesh->setZExtent(2);

    link1Matrix = base1Transform->matrix();
    link1Matrix.translate(QVector3D(0,9.5,0));
    link1Transform = new Qt3DCore::QTransform();
    link1Transform->setMatrix(link1Matrix);

    link1Matrix.translate(0,-5,0);
    link1Matrix.rotate(90, 1,0,0);
    link1Matrix.translate(0,5,0);
    link1Transform->setMatrix(link1Matrix);

    link1 = new Qt3DCore::QEntity();
    link1->addComponent(link1Mesh);
    link1->addComponent(link1Transform);
    link1->addComponent(manipulatorMaterial);
    link1->setParent(sceneRoot);

    /*!
     * Second joint
     */
    link2Mesh = new Qt3DExtras::QCuboidMesh();
    link2Mesh->setXExtent(2);
    link2Mesh->setYExtent(10);
    link2Mesh->setZExtent(2);



    link2Matrix = link1Transform->matrix();
    link2Matrix.translate(QVector3D(0,10,0));
    link2Transform = new Qt3DCore::QTransform();
    link2Transform->setMatrix(link2Matrix);


    link2 = new Qt3DCore::QEntity();
    link2->addComponent(link1Mesh);
    link2->addComponent(link2Transform);
    link2->addComponent(manipulatorMaterial);
    link2->setParent(sceneRoot);

    /*!
     * Third joint
     */
    link3Mesh = new Qt3DExtras::QCuboidMesh();
    link3Mesh->setXExtent(2);
    link3Mesh->setYExtent(10);
    link3Mesh->setZExtent(2);

    link3Transform = new Qt3DCore::QTransform();
    link3Transform->setTranslation(QVector3D(0,25,0));
    link3Matrix = link3Transform->matrix();

    link3 = new Qt3DCore::QEntity();
    link3->addComponent(link3Mesh);
    link3->addComponent(link3Transform);
    link3->addComponent(manipulatorMaterial);
    link3->setParent(sceneRoot);

    q1Rotation = 0;
    q2Rotation = 0;
    //q3Rotation = 90;

   setQ1(45);
   setQ2(25);
   setQ3(-5);



}


void Manipulator::setQ1(int val)
{
    static int previousQ = 0;
    q1Rotation = val;
    link1Matrix.translate(0,-5,0);
    link1Matrix.rotate(q1Rotation-previousQ, 0,0,1);
    link1Matrix.translate(0,5,0);
    link1Transform->setMatrix(link1Matrix);

    link2Matrix=link1Matrix;
    link2Matrix.translate(0,5,0);
    link2Matrix.rotate(q2Rotation, 0,0,1);
    link2Matrix.translate(0,5,0);
    link2Transform->setMatrix(link2Matrix);

    link3Matrix=link2Matrix;
    link3Matrix.translate(0,5,0);
    link3Matrix.rotate(90, 1,0,0);
    link3Matrix.translate(0,5,0);
    link3Transform->setMatrix(link3Matrix);

    previousQ = q1Rotation;
}

void Manipulator::setQ2(int val)
{
    static int previousQ = 0;
    q2Rotation = val;
    link2Matrix.translate(0,-5,0);
    link2Matrix.rotate(q2Rotation-previousQ, 0,0,1);
    link2Matrix.translate(0,5,0);
    link2Transform->setMatrix(link2Matrix);

    link3Matrix=link2Matrix;
    link3Matrix.translate(0,5,0);
    link3Matrix.rotate(90, 1,0,0);
    link3Matrix.translate(0,5,0);
    link3Transform->setMatrix(link3Matrix);
    previousQ = q2Rotation;
}

void Manipulator::setQ3(int val)
{
    static int previousQ = 0;
    q3Rotation = val;
    //link3Matrix.translate(0,-5,0);
    //link3Matrix.rotate(q3Rotation-previousQ, 1,0,0);
    link3Matrix.translate(0,val,0);
    link3Transform->setMatrix(link3Matrix);
    previousQ = q3Rotation;
}

