#include "object_widget.h"
#include "ui_object_widget.h"

#include <QDebug>
Object_Widget::Object_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Object_Widget)
{
    qDebug() << "STWORZYŁEM SCENE\n";
    ui->setupUi(this);
}

Object_Widget::~Object_Widget()
{
    delete ui;
}


Object_Scene *Object_Widget::renderScene()
{
    return ui->renderScene;
}
