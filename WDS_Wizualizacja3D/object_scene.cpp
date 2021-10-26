#include "object_scene.h"

#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QEntity>
#include <QTransform>
#include <QScreen>
#include <QLayout>
#include <QCheckBox>
#include <QScreen>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QPointLight>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>

#include <QPushButton>
#include <QLabel>
#include <Qt3DExtras/QForwardRenderer>


Object_Scene::Object_Scene(QWidget *parent)
    : QWidget(parent), objectEntity(nullptr)
{

    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0xFFDFD3)));
    QWidget *container = QWidget::createWindowContainer(view);


    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(400, 400));
    container->setMaximumSize(screenSize);
    container->setFocusPolicy(Qt::NoFocus);

    // Root entity
    rootEntity = new Qt3DCore::QEntity();

    // Camera
    cameraEntity = view->camera();

    // For the Blender model:
        // X+ -> right
        // Y+ -> away
        // Z+ -> up
        cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
        cameraEntity->setPosition(QVector3D(0, -10, 5.0f));
        cameraEntity->setUpVector(QVector3D(0, 0, 1));
        cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);//moje
    camController->setCamera(cameraEntity);//moje

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    //lightTransform->setTranslation(cameraEntity->position());
    //lightEntity->addComponent(lightTransform);
    lightTransform->setTranslation(QVector3D(30,-100, -20));
    lightEntity->addComponent(lightTransform);
    renderFile = QUrl(QString("file:C:/Users/john/OneDrive/Pulpit/object1.dae"));


    // Set root object of the scene
    createRenderObject();
    view->setRootEntity(rootEntity);
    //createRenderableObject(rootEntity);
    this->setLayout(new QGridLayout);
    layout()->addWidget(container);
}



void Object_Scene::createRenderObject()
{
    if(objectEntity)
    {
        delete objectEntity;
    }

    /*objectEntity = new Render_Object(rootEntity);
    objectEntity->loader()->setSource(renderFile);
    objectEntity->transform()->setScale(0.35);*/
    manip = new Manipulator(rootEntity);
    manip->createManipulator();
}

void Object_Scene::setRenderFile(QUrl renderFile)
{
    this->renderFile = renderFile;
    createRenderObject();
}

void Object_Scene::setTextureFile(QUrl textureFile)
{
    this->textureFile = textureFile;
    createRenderObject();
}

Render_Object *Object_Scene::get_RenderObject()
{
    return objectEntity;
}

void Object_Scene::setOrientation(const QQuaternion &orientation)
{
    /*if(this->orientation!=orientation)
    {
    this->orientation = orientation;

    objectEntity->transform()->setRotation(this->orientation);

    emit orientationChanged(orientation);
    }*/

    if(this->orientation!=orientation)
        {
        this->orientation = orientation;


            QVector3D rot = this->orientation.toEulerAngles();
            manip->setQ1(rot.x());
            manip->setQ2(rot.y());
            manip->setQ3(rot.z());
            emit orientationChanged(orientation);
        }


}



void Object_Scene::setPosition(const QVector3D &position)
{
    if(this->position!=position)
    {
    this->position = position;

    objectEntity->transform()->setTranslation(this->position);
    emit positionChanged(position);
    }
}

