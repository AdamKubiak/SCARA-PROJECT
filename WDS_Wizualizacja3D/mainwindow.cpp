#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QDir>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    objectWidget = ui->visualizationWidget;
    objectScene = objectWidget->renderScene();
    device = new Device();
    addConnections();
    menuAvailablePorts();

    //objectScene->setPosition(QVector3D(-1.11111,2.11111,-10.11111));
    //
    //objectScene->setOrientation(QQuaternion(QVector4D(QVector3D(0,0,1),0.269)));
    makePlot();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::menuAvailablePorts()
{
    ui->menuWybierz_port->clear();
    QList<QSerialPortInfo> devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.size(); i++)
    {
      ui->menuWybierz_port->addAction(new QAction(devices.at(i).portName(), this));

    }
}


void MainWindow::addConnections()
{



    connect(objectScene, &Object_Scene::orientationChanged,this, &MainWindow::setRotationValue);

    connect(objectScene, &Object_Scene::positionChanged, this, &MainWindow::setTranslationValue);

    connect(objectScene, &Object_Scene::orientationChanged, objectScene, &Object_Scene::setOrientation);

    connect(objectScene, &Object_Scene::positionChanged, objectScene, &Object_Scene::setPosition);
    connect(device,&Device::orientationChanged,objectScene,&Object_Scene::setOrientation);
    connect(device,&Device::positionChanged,objectScene,&Object_Scene::setPosition);

    connect(device,&Device::sendGyroDatatoChart,this,&MainWindow::setChartsValue);
    connect(device,&Device::sendAccDatatoChart,this,&MainWindow::setChartsValue);

    connect(device, &Device::sendingMessage,device, &Device::sendMessageToDevice);

    connect(ui->horizontalSlider, &QAbstractSlider::valueChanged,this,&MainWindow::sliderToData);

    connect(ui->actionZa_aduj_model, &QAction::triggered,[=]()
    {
    //QString filter = "Dae File (*.dae) ;; Obj File (*.obj)";
    //QUrl renderFile =QFileDialog::getOpenFileUrl(this,"Wybierz model formatu .dae",QDir::homePath(),filter);
    QUrl renderFile =QFileDialog::getOpenFileUrl(this,"Wybierz model formatu .dae",QDir::homePath());
    if(0!=renderFile.toString().length())
    {
        objectScene->setRenderFile(renderFile);
        on_resetViewButton_clicked();
    }
    });


    connect(ui->actionUsu_model, &QAction::triggered,[=]()
    {

        objectScene->setRenderFile(QUrl(QString("file:C:/Users/john/OneDrive/Pulpit/object1.dae")));
        on_resetViewButton_clicked();
    });

    connect(ui->actionZnajd_port,&QAction::triggered, [=]()
    {
        menuAvailablePorts();
    });

    connect(ui->menuWybierz_port, &QMenu::triggered, this, &MainWindow::selectPort);

    connect(ui->actionStart,&QAction::triggered,device,&Device::startCommunication);

    connect(ui->actionStop, &QAction::triggered, [=]()
    {
        if(device->get_SerialPortObject()->isOpen()) {
          device->get_SerialPortObject()->close();
          qDebug()<<"Zamknięto połączenie.";
        } else {
          qDebug()<<"Port nie jest otwarty!";
          return;
        }
    });


}




void MainWindow::setXRotationValue(float value)
{
    //float temp = toPreci
      //      value
    float nearest = roundf(value * 1000) / 1000;
    ui->xRotationValue->setText(QString::number(nearest));
    ui->xRotationValue->update();
}

void MainWindow::setYRotationValue(float value)
{
    float nearest = roundf(value * 1000) / 1000;
    ui->yRotationValue->setText(QString::number(nearest));
}

void MainWindow::setZRotationValue(float value)
{
    float nearest = roundf(value * 1000) / 1000;
    ui->zRotationValue->setText(QString::number(nearest));
}

void MainWindow::setXTranslationValue(float value)
{
    float nearest = roundf(value * 1000) / 1000;
    ui->xTranslationValue->setText(QString::number(nearest));
}

void MainWindow::setYTranslationValue(float value)
{
    float nearest = roundf(value * 1000) / 1000;
    ui->yTranslationValue->setText(QString::number(nearest));
}

void MainWindow::setZTranslationValue(float value)
{
    float nearest = roundf(value * 1000) / 1000;
    ui->zTranslationValue->setText(QString::number(nearest));
}


void MainWindow::setRotationValue(const QQuaternion &value)
{
    QVector3D eulerAngles = value.toEulerAngles();

        setXRotationValue(eulerAngles.x());
        setYRotationValue(eulerAngles.y());
        setZRotationValue(eulerAngles.z());
}

void MainWindow::setTranslationValue(const QVector3D &value)
{
    setXTranslationValue(value.x());
    setYTranslationValue(value.y());
    setZTranslationValue(value.z());
}

void MainWindow::selectPort(QAction *trigger)
{
    device->setSerialName(trigger->iconText());
    QString mess = "Wybrany port: ";
    mess.append(trigger->iconText());
    ui->statusbar->showMessage(mess);
    // device->get_SerialPortObject()->setPortName(trigger->iconText());
}

void MainWindow::makePlot()
{
    //gyro graphs
   QCPGraph *graph1 = ui->plotXAxis->addGraph();
   QCPGraph *graph2 = ui->plotYAxis->addGraph();
   QCPGraph *graph3 = ui->plotZAxis->addGraph();

   //acc graphs
   QCPGraph *graph4 = ui->plotXAxis->addGraph();
   QCPGraph *graph5 = ui->plotYAxis->addGraph();
   QCPGraph *graph6 = ui->plotZAxis->addGraph();

   //graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
   graph1->setPen(QPen(QColor(10, 140, 70, 160), 2));
   graph2->setPen(QPen(QColor(10, 140, 70, 160), 2));
   graph3->setPen(QPen(QColor(10, 140, 70, 160), 2));

   graph4->setPen(QPen(QColor(220,20,60), 2));
   graph5->setPen(QPen(QColor(220,20,60), 2));
   graph6->setPen(QPen(QColor(220,20,60), 2));

   // set some pens, brushes and backgrounds:
   ui->plotXAxis->xAxis->setBasePen(QPen(Qt::white, 1));
   ui->plotXAxis->yAxis->setBasePen(QPen(Qt::white, 1));
   ui->plotXAxis->xAxis->setTickPen(QPen(Qt::white, 1));
   ui->plotXAxis->yAxis->setTickPen(QPen(Qt::white, 1));
   ui->plotXAxis->xAxis->setSubTickPen(QPen(Qt::white, 1));
   ui->plotXAxis->yAxis->setSubTickPen(QPen(Qt::white, 1));
   ui->plotXAxis->xAxis->setTickLabelColor(Qt::white);
   ui->plotXAxis->yAxis->setTickLabelColor(Qt::white);
   ui->plotXAxis->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
   ui->plotXAxis->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
   ui->plotXAxis->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
   ui->plotXAxis->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
   ui->plotXAxis->xAxis->grid()->setSubGridVisible(true);
   ui->plotXAxis->yAxis->grid()->setSubGridVisible(true);
   ui->plotXAxis->xAxis->grid()->setZeroLinePen(Qt::NoPen);
   ui->plotXAxis->yAxis->grid()->setZeroLinePen(Qt::NoPen);
   ui->plotXAxis->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
   ui->plotXAxis->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
   QLinearGradient plotGradient;
   plotGradient.setStart(0, 0);
   plotGradient.setFinalStop(0, 350);
   plotGradient.setColorAt(0, QColor(80, 80, 80));
   plotGradient.setColorAt(1, QColor(50, 50, 50));
   ui->plotXAxis->setBackground(plotGradient);
   QLinearGradient axisRectGradient;
   axisRectGradient.setStart(0, 0);
   axisRectGradient.setFinalStop(0, 350);
   axisRectGradient.setColorAt(0, QColor(80, 80, 80));
   axisRectGradient.setColorAt(1, QColor(30, 30, 30));
   ui->plotXAxis->axisRect()->setBackground(axisRectGradient);
   ui->plotXAxis->xAxis->setLabelColor(Qt::white);
   ui->plotXAxis->yAxis->setLabelColor(Qt::white);

   ui->plotYAxis->xAxis->setBasePen(QPen(Qt::white, 1));
   ui->plotYAxis->yAxis->setBasePen(QPen(Qt::white, 1));
   ui->plotYAxis->xAxis->setTickPen(QPen(Qt::white, 1));
   ui->plotYAxis->yAxis->setTickPen(QPen(Qt::white, 1));
   ui->plotYAxis->xAxis->setSubTickPen(QPen(Qt::white, 1));
   ui->plotYAxis->yAxis->setSubTickPen(QPen(Qt::white, 1));
   ui->plotYAxis->xAxis->setTickLabelColor(Qt::white);
   ui->plotYAxis->yAxis->setTickLabelColor(Qt::white);
   ui->plotYAxis->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
   ui->plotYAxis->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
   ui->plotYAxis->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
   ui->plotYAxis->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
   ui->plotYAxis->xAxis->grid()->setSubGridVisible(true);
   ui->plotYAxis->yAxis->grid()->setSubGridVisible(true);
   ui->plotYAxis->xAxis->grid()->setZeroLinePen(Qt::NoPen);
   ui->plotYAxis->yAxis->grid()->setZeroLinePen(Qt::NoPen);
   ui->plotYAxis->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
   ui->plotYAxis->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
   ui->plotYAxis->setBackground(plotGradient);
   ui->plotYAxis->axisRect()->setBackground(axisRectGradient);
   ui->plotYAxis->xAxis->setLabelColor(Qt::white);
   ui->plotYAxis->yAxis->setLabelColor(Qt::white);

   ui->plotZAxis->xAxis->setBasePen(QPen(Qt::white, 1));
   ui->plotZAxis->yAxis->setBasePen(QPen(Qt::white, 1));
   ui->plotZAxis->xAxis->setTickPen(QPen(Qt::white, 1));
   ui->plotZAxis->yAxis->setTickPen(QPen(Qt::white, 1));
   ui->plotZAxis->xAxis->setSubTickPen(QPen(Qt::white, 1));
   ui->plotZAxis->yAxis->setSubTickPen(QPen(Qt::white, 1));
   ui->plotZAxis->xAxis->setTickLabelColor(Qt::white);
   ui->plotZAxis->yAxis->setTickLabelColor(Qt::white);
   ui->plotZAxis->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
   ui->plotZAxis->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
   ui->plotZAxis->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
   ui->plotZAxis->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
   ui->plotZAxis->xAxis->grid()->setSubGridVisible(true);
   ui->plotZAxis->yAxis->grid()->setSubGridVisible(true);
   ui->plotZAxis->xAxis->grid()->setZeroLinePen(Qt::NoPen);
   ui->plotZAxis->yAxis->grid()->setZeroLinePen(Qt::NoPen);
   ui->plotZAxis->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
   ui->plotZAxis->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
   ui->plotZAxis->setBackground(plotGradient);
   ui->plotZAxis->axisRect()->setBackground(axisRectGradient);
   ui->plotZAxis->xAxis->setLabelColor(Qt::white);
   ui->plotZAxis->yAxis->setLabelColor(Qt::white);




    // create graph and assign data to it:
    //ui->plotXAxis->addGraph();


    // give the axes some labels:
    ui->plotXAxis->xAxis->setLabel("Czas");
    ui->plotXAxis->yAxis->setLabel("Dane z urządzenia");
    // set axes ranges, so we see all data:




   // ui->plotYAxis->addGraph();

    // give the axes some labels:
    ui->plotYAxis->xAxis->setLabel("Czas");
    ui->plotYAxis->yAxis->setLabel("Dane z urządzenia");


    //ui->plotZAxis->addGraph();

    // give the axes some labels:
    ui->plotZAxis->xAxis->setLabel("Czas");
    ui->plotZAxis->yAxis->setLabel("Dane z urządzenia");
    ui->plotXAxis->yAxis->setRange(-20000,20000);
    ui->plotYAxis->yAxis->setRange(-20000,20000);
    ui->plotZAxis->yAxis->setRange(-20000,20000);


}



void MainWindow::setChartsValue(const QVector3D &Axis, char ID)
{
    static int i = 0;



    static QTime prev = QTime::currentTime();
    dt = (float)prev.msecsTo(QTime::currentTime())/1000.0;

    if(ID=='G')
    {
        Xgyro_y.append(Axis.x());
        Ygyro_y.append(Axis.y());
        Zgyro_y.append(Axis.z());
    }

    if(ID=='A')
    {
        Xacc_y.append(Axis.x());
        Yacc_y.append(Axis.y());
        Zacc_y.append(Axis.z());
    }

    dtTime.append(dt);

    ui->plotXAxis->xAxis->setRange(dt-1,dt+0.09);


    ui->plotYAxis->xAxis->setRange(dt-1,dt+0.09);


    ui->plotZAxis->xAxis->setRange(dt-1,dt+0.09);



    //while(dtTime.size()> 200)
      //      dtTime.erase(dtTime.begin());

    if(ID=='G')
    {
        while(dtTime.size()> 200)
                dtTime.erase(dtTime.begin());

        while(Xgyro_y.size()> 200)
            Xgyro_y.erase(Xgyro_y.begin());

        while(Ygyro_y.size()> 200)
            Ygyro_y.erase(Ygyro_y.begin());

        while(Zgyro_y.size()> 200)
            Zgyro_y.erase(Zgyro_y.begin());


    }

    if(ID=='A')
    {
        while(dtTime.size()> 200)
                dtTime.erase(dtTime.begin());

        while(Xacc_y.size()> 200)
            Xacc_y.erase(Xacc_y.begin());

        while(Yacc_y.size()> 200)
            Yacc_y.erase(Yacc_y.begin());

        while(Zacc_y.size()> 200)
            Zacc_y.erase(Zacc_y.begin());


    }

    if(i<6)
    {
        i++;
        return;
    }

   /* if(ui->checkBox_2->isChecked())
    {
    ui->plotXAxis->graph(0)->setData(dtTime, Xgyro_y);
    ui->plotYAxis->graph(0)->setData(dtTime, Ygyro_y);
    ui->plotZAxis->graph(0)->setData(dtTime, Zgyro_y);
    }
    if(ui->checkBox->isChecked())
    {
    ui->plotXAxis->graph(1)->setData(dtTime, Xacc_y);
    ui->plotYAxis->graph(1)->setData(dtTime, Yacc_y);
    ui->plotZAxis->graph(1)->setData(dtTime, Zacc_y);
    }*/




        ui->plotXAxis->replot();
        ui->plotYAxis->replot();
        ui->plotZAxis->replot();

        ui->plotXAxis->update();
        ui->plotYAxis->update();
        ui->plotZAxis->update();
        i=0;

}

void MainWindow::sliderToData(int value)
{
 sliders_Data.setX(value);
}


void MainWindow::on_resetViewButton_clicked()
{
    objectScene->setOrientation(QQuaternion(QVector4D(QVector3D(0,0,0),0)));
    objectScene->setPosition(QVector3D(0,0,0));
    device->resetDeviceValues();
    std::stringstream msg;
    msg <<"I "<<sliders_Data.getX() << " 4.41 5.45 55R";
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}
void MainWindow::on_motor1L_pressed()
{
    std::stringstream msg;
    msg <<"M 1 1 55R";
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}

void MainWindow::on_motor1R_pressed()
{
    std::stringstream msg;
    msg <<"M 1 2 55R";
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}

void MainWindow::on_motor2L_pressed()
{
    std::stringstream msg;
    msg <<"M 2 1 55R";
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}

void MainWindow::on_motor2R_pressed()
{
    std::stringstream msg;
    msg <<"M 2 2 55R";
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}


void MainWindow::on_motor1L_released()
{
    std::stringstream msg;
    msg <<"S 55R";
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}

void MainWindow::on_motor1R_released()
{
    std::stringstream msg;
    msg <<"S 55R";
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}

void MainWindow::on_motor2L_released()
{
    std::stringstream msg;
    msg <<"S 55R";
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}

void MainWindow::on_motor2R_released()
{
    std::stringstream msg;
    msg <<"S 55R";
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}


void MainWindow::on_pushButton_5_clicked()
{
    std::stringstream msg;
    msg <<"E 0 0 55R";
    robot = new Robot(32768.0,32768.0,0.0,0.0);
    connect(device,&Device::newDeviceValues,robot,&Robot::encToDegree);
    //connect(robot, &Robot::encoderPositionChanged, )
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}
