#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QDir>
#include <sstream>

float calcFcn(QVector3D params, float x)
{
    float y = (-params.x() * x + params.z()) / params.y();

    return y;
}


QVector3D lineFromPoints(std::pair<float,float> P, std::pair<float,float> Q)
{
    float a = Q.second - P.second;
    float b = P.first - Q.first;
    float c = a * (P.first) + b * (P.second);

    return QVector3D(a,b,c);

}

std::vector<float> circle_equation(float x1, float y1, float r)
{
    std::vector<float> abc;
    float a = -2 * x1;

    float b = -2 * y1;

    float c = (r * r) - (x1 * x1) - (y1 * y1);

    abc.push_back(a);
    abc.push_back(b);
    abc.push_back(c);

    return abc;
}

std::pair<float, float> quadradicEq(float x,std::vector<float> abc)
{
    abc[2] = (-abc[2] + (x * x) + abc[0] * x);

    float delta = abc[1] * abc[1] - 4 * abc[2];
    float X1 = (-abc[1] - sqrt(delta)) / 2;
    float X2 = (-abc[1] + sqrt(delta)) / 2;
    std::pair<float, float> essa = std::make_pair(X1, X2);
    return  essa;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->J1sliderValue->setText(QString::number(0));
    ui->J2sliderValue->setText(QString::number(0));
    ui->J3sliderValue->setText(QString::number(0));

    objectWidget = ui->visualizationWidget;
    objectScene = objectWidget->renderScene();
    device = new Device();
    addConnections();
    menuAvailablePorts();

    //objectScene->setPosition(QVector3D(-1.11111,2.11111,-10.11111));
    //
    //objectScene->setOrientation(QQuaternion(QQuaternion::fromEulerAngles(QVector3D(110.0,0,0))));
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



    //connect(objectScene, &Object_Scene::orientationChanged,this, &MainWindow::setRotationValue);

    //connect(objectScene, &Object_Scene::positionChanged, this, &MainWindow::setTranslationValue);


    connect(objectScene, &Object_Scene::orientationChanged, objectScene, &Object_Scene::setOrientation);

   // connect(objectScene, &Object_Scene::positionChanged, objectScene, &Object_Scene::setPosition);
    //connect(device,&Device::orientationChanged,objectScene,&Object_Scene::setOrientation);
    //connect(device,&Device::positionChanged,objectScene,&Object_Scene::setPosition);

    connect(device,&Device::sendEncoderDatatoChart,this,&MainWindow::setChartsValue);
    //connect(device,&Device::sendAccDatatoChart,this,&MainWindow::setChartsValue);

    connect(device, &Device::sendingMessage,device, &Device::sendMessageToDevice);

    connect(ui->horizontalSlider, &QAbstractSlider::valueChanged,this,&MainWindow::sliderToData);
    connect(ui->horizontalSlider_2, &QAbstractSlider::valueChanged,this,&MainWindow::sliderToData);
    connect(ui->horizontalSlider_3, &QAbstractSlider::valueChanged,this,&MainWindow::sliderToData);

    connect(ui->X_SpinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&MainWindow::spinboxToData);
    connect(ui->Y_SpinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&MainWindow::spinboxToData);
    connect(ui->Z_SpinBox,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,&MainWindow::spinboxToData);

    connect(this,&MainWindow::infoSliderChanged,this, &MainWindow::setSlidersInfo);


    connect(ui->actionZa_aduj_model, &QAction::triggered,[=]()
    {
    QString filter = "Dae File (*.dae) ;; Obj File (*.obj)";
    QUrl renderFile =QFileDialog::getOpenFileUrl(this,"Wybierz model formatu .dae",QDir::homePath(),filter);
    //QUrl renderFile =QFileDialog::getOpenFileUrl(this,"Wybierz model formatu .dae",QDir::homePath());
    if(0!=renderFile.toString().length())
    {
        objectScene->setRenderFile(renderFile);
        //on_resetViewButton_clicked();
    }
    });


    connect(ui->actionUsu_model, &QAction::triggered,[=]()
    {

        objectScene->setRenderFile(QUrl(QString("file:C:/Users/john/OneDrive/Pulpit/Pierwsze_ramie_inz.obj")));
        //on_resetViewButton_clicked();
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

    connect(ui->actionDodaj_pozycj, &QAction::triggered,[=]()
    {
        seqHandler.addSeqPosition(robot->J1_angle,robot->J2_angle,robot->J3_angle);
        seqHandlerDraw.addSeqPosition(robot->position.x(),robot->position.y(),robot->position.z());
        std::ostringstream oss;
        oss << "Dodaną następującą liczbę pozycji: " << seqHandler.getSize();

        QMessageBox::information(this,"Sekwencja", QString::fromStdString(oss.str()));

    });

    connect(ui->actionWyczy_sekwencj, &QAction::triggered,[=]()
    {
        seqHandler.clearSequence();
        seqHandlerDraw.clearSequence();

        QMessageBox::information(this,"Sekwencja", "Sekwencja została usunięta");

    });

    connect(ui->actionStart_2, &QAction::triggered,[=]()
    {

        for(seqHandler.index = 0; seqHandler.index<seqHandler.getSize(); seqHandler.index++)
        {
            std::stringstream msg;
            auto tuple =  seqHandler.getList()[seqHandler.index];
            float X, Y, Z;
            std::tie(X, Y, Z) = tuple;

            robot->fromSlidersToPosition(X,Y,Z);


            msg <<"I "<<(int)robot->setpoint_J1_angle << " " << (int)robot->setpoint_J2_angle << " "<< (int)robot->setpoint_J3_angle << " 55R";

            qDebug() << QString::fromStdString(msg.str());
            emit device->sendingMessage(QString::fromStdString(msg.str()));
            seqHandler.delay();

        }
        seqHandler.index = seqHandler.getSize();

    });


    connect(ui->actionStop_2, &QAction::triggered,[=]()
    {
        seqHandler.index = seqHandler.getSize();
    });

    connect(ui->actionNarysuj_linie, &QAction::triggered,[=]()
    {
        float X1, Y1, X2, Y2,Z;
        std::stringstream msg;
        QVector3D params;
        if(seqHandlerDraw.getSize() == 2)
        {
            auto tuple =  seqHandlerDraw.getList()[0];
            std::tie(X1, Y1, Z) = tuple;
            //qDebug() << X1<<Y1;
            tuple =  seqHandlerDraw.getList()[1];
            std::tie(X2, Y2, Z) = tuple;
            //qDebug() << X2<<Y2;

            params = lineFromPoints(std::make_pair(X1,Y1), std::make_pair(X2,Y2));
            //qDebug() << params[0]<<params[1]<<params[2];

            if(X1<X2)
            {
                while(X1<X2)
                {

                    robot->fromSpinboxToPosition(X1,calcFcn(params,X1),Z,ui->checkBox->isChecked());
                    X1 = X1+0.25;
                    msg <<"I "<<(int)robot->setpoint_J1_angle << " " << (int)robot->setpoint_J2_angle << " "<< (int)robot->setpoint_J3_angle << " 55R";//msg <<"I "<<sliders_Data.getX() << " 4.41 5.45 55R";
                    qDebug() << QString::fromStdString(msg.str());
                    emit device->sendingMessage(QString::fromStdString(msg.str()));
                    msg.str(std::string());
                    QTime dieTime= QTime::currentTime().addMSecs(10);
                    while (QTime::currentTime() < dieTime)
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 3000);

                }
            }
        }
    });

    connect(ui->actionNarysuj_kolo, &QAction::triggered,[=]()
    {
        float X1, Y1, X2, Y2,Z;
        Z= 4.0;
        float r = 4.0;

        std::stringstream msg;
        std::vector<float> X_list;
        std::vector<std::pair<float,float>> wyniki;
        if(seqHandlerDraw.getSize() == 1)
        {
            auto tuple =  seqHandlerDraw.getList()[0];
            std::tie(X1, Y1, Z) = tuple;
            std::vector<float> abc = circle_equation(X1, Y1, r);

            float Xmin = X1-r;
            float Xmax = X1+r;

            while(Xmin <= Xmax)
            {
                wyniki.push_back(quadradicEq(Xmin,abc));
                X_list.push_back(Xmin);
                Xmin = Xmin + 0.25;
               /* if(Xmin>Xmax){

                    Xmin = Xmax;
                    wyniki.push_back(quadradicEq(Xmin,abc));
                    X_list.push_back(Xmin);
                }*/
            }


            for(unsigned int i = 0; i<wyniki.size(); i++)
            {
                robot->fromSpinboxToPosition(X_list[i],wyniki[i].first,Z,ui->checkBox->isChecked());
                msg <<"I "<<(int)robot->setpoint_J1_angle << " " << (int)robot->setpoint_J2_angle << " "<< (int)robot->setpoint_J3_angle << " 55R";//msg <<"I "<<sliders_Data.getX() << " 4.41 5.45 55R";
                qDebug() << QString::fromStdString(msg.str());
                emit device->sendingMessage(QString::fromStdString(msg.str()));
                msg.str(std::string());
                QTime dieTime= QTime::currentTime().addMSecs(10);
                while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 3000);

            }

            for(unsigned int i = wyniki.size()-1; i>0; i--)
            {
                qDebug()<<i;
                robot->fromSpinboxToPosition(X_list[i],wyniki[i].second,Z,ui->checkBox->isChecked());
                msg <<"I "<<(int)robot->setpoint_J1_angle << " " << (int)robot->setpoint_J2_angle << " "<< (int)robot->setpoint_J3_angle << " 55R";//msg <<"I "<<sliders_Data.getX() << " 4.41 5.45 55R";
                qDebug() << QString::fromStdString(msg.str());
                emit device->sendingMessage(QString::fromStdString(msg.str()));
                msg.str(std::string());
                QTime dieTime= QTime::currentTime().addMSecs(10);
                while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 3000);

            }



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


void MainWindow::setRotationValue(float x , float y, float z)
{
        setXRotationValue(x);
        setYRotationValue(y);
        setZRotationValue(z);


        objectScene->setOrientation(x,y,z);

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
   //QCPGraph *graph3 = ui->plotZAxis->addGraph();

   //acc graphs
   QCPGraph *graph4 = ui->plotXAxis->addGraph();
   QCPGraph *graph5 = ui->plotYAxis->addGraph();
   //QCPGraph *graph6 = ui->plotZAxis->addGraph();

   //graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
   graph1->setPen(QPen(QColor(10, 140, 70, 160), 2));
   graph2->setPen(QPen(QColor(10, 140, 70, 160), 2));
   //graph3->setPen(QPen(QColor(10, 140, 70, 160), 2));

   graph4->setPen(QPen(QColor(220,20,60), 2));
   graph5->setPen(QPen(QColor(220,20,60), 2));
   //graph6->setPen(QPen(QColor(220,20,60), 2));

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

  /* ui->plotZAxis->xAxis->setBasePen(QPen(Qt::white, 1));
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
   ui->plotZAxis->yAxis->setLabelColor(Qt::white);*/




    // create graph and assign data to it:
    //ui->plotXAxis->addGraph();


    // give the axes some labels:
    ui->plotXAxis->xAxis->setLabel("Czas");
    ui->plotXAxis->yAxis->setLabel("Pozycja enkodera");
    // set axes ranges, so we see all data:




   // ui->plotYAxis->addGraph();

    // give the axes some labels:
    ui->plotYAxis->xAxis->setLabel("Czas");
    ui->plotYAxis->yAxis->setLabel("Pozycja enkodera");


    //ui->plotZAxis->addGraph();

    // give the axes some labels:
    //ui->plotZAxis->xAxis->setLabel("Czas");
    //ui->plotZAxis->yAxis->setLabel("Pozycja enkodera");
    ui->plotXAxis->yAxis->setRange(-320,3600);
    ui->plotYAxis->yAxis->setRange(-4000,4000);
    //ui->plotZAxis->yAxis->setRange(-20000,20000);


}



void MainWindow::setChartsValue(const QVector3D &Axis, char ID)
{
    static int i = 0;



    static QTime prev = QTime::currentTime();
    dt = (float)prev.msecsTo(QTime::currentTime())/1000.0;

    if(ID=='E')
    {
        Xgyro_y.append(Axis.x()-32768);
        Ygyro_y.append(Axis.y()-32768);
        //Zgyro_y.append(Axis.z()-32768);
    }


    dtTime.append(dt);

    ui->plotXAxis->xAxis->setRange(dt-1,dt+0.09);


    ui->plotYAxis->xAxis->setRange(dt-1,dt+0.09);

    if(ID=='E')
    {
        while(dtTime.size()> 2000)
                dtTime.erase(dtTime.begin());

        while(Xgyro_y.size()> 2000)
            Xgyro_y.erase(Xgyro_y.begin());

        while(Ygyro_y.size()> 2000)
            Ygyro_y.erase(Ygyro_y.begin());


    }

    if(i<30)
    {
        i++;
        return;
    }


    ui->plotXAxis->graph(0)->setData(dtTime, Xgyro_y);
    ui->plotYAxis->graph(0)->setData(dtTime, Ygyro_y);
    //ui->plotZAxis->graph(0)->setData(dtTime, Zgyro_y);
    /*
    if(ui->checkBox->isChecked())
    {
    ui->plotXAxis->graph(1)->setData(dtTime, Xacc_y);
    ui->plotYAxis->graph(1)->setData(dtTime, Yacc_y);
    ui->plotZAxis->graph(1)->setData(dtTime, Zacc_y);
    }*/




        ui->plotXAxis->replot();
        ui->plotYAxis->replot();
        //ui->plotZAxis->replot();

        ui->plotXAxis->update();
        ui->plotYAxis->update();
        //ui->plotZAxis->update();
        i=0;

}

void MainWindow::sliderToData(int value)
{
  int x = ui->horizontalSlider->value();
  sliders_Data.setX(x);
  int y = ui->horizontalSlider_2->value();
  sliders_Data.setY(y);
  int z = ui->horizontalSlider_3->value();
  sliders_Data.setZ(z);

  emit infoSliderChanged();
}

void MainWindow::spinboxToData(double value)
{
    //double x =
    spinbox_Data.setX(ui->X_SpinBox->value());
    spinbox_Data.setY(ui->Y_SpinBox->value());
    spinbox_Data.setZ(ui->Z_SpinBox->value());




}

void MainWindow::setSlidersInfo()
{
    ui->J1sliderValue->setText(QString::number(sliders_Data.getX()));
    ui->J2sliderValue->setText(QString::number(sliders_Data.getY()));
    ui->J3sliderValue->setText(QString::number(sliders_Data.getZ()));
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
    msg <<"E 0 0 0 55R";
    robot = new Robot(0,0,0.0,0.0);


    disconnect(robot, &Robot::encoderPositionChanged,this,&MainWindow::setRotationValue);
    disconnect(robot,&Robot::robotPositionChanged,this,&MainWindow::setTranslationValue);
    disconnect(device,&Device::newDeviceValues,robot,&Robot::encToDegree);
    connect(device,&Device::newDeviceValues,robot,&Robot::encToDegree);
    connect(robot, &Robot::encoderPositionChanged,this,&MainWindow::setRotationValue);
    connect(robot,&Robot::robotPositionChanged,this,&MainWindow::setTranslationValue);
    //connect(robot,&Robot::encoderPositionChanged,objectScene, &Object_Scene::setOrientation);



    emit device->sendingMessage(QString::fromStdString(msg.str()));

    QMessageBox::information(this,"Kalibracja", "Pozycja startowa manipulatora pomyślnie skalibrowana");
}

void MainWindow::on_positionSetButton_clicked()
{
    std::stringstream msg;

    robot->fromSpinboxToPosition(spinbox_Data.x(),spinbox_Data.y(),spinbox_Data.z(),ui->checkBox->isChecked());

    msg <<"I "<<(int)robot->setpoint_J1_angle << " " << (int)robot->setpoint_J2_angle << " "<< (int)robot->setpoint_J3_angle << " 55R";
    qDebug() << QString::fromStdString(msg.str());
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}

void MainWindow::on_resetViewButton_clicked()
{

    //device->resetDeviceValues();
    std::stringstream msg;

    robot->fromSlidersToPosition(sliders_Data.getX(),sliders_Data.getY(),sliders_Data.getZ());

    msg <<"I "<<(int)robot->setpoint_J1_angle << " " << (int)robot->setpoint_J2_angle << " "<< (int)robot->setpoint_J3_angle << " 55R";//msg <<"I "<<sliders_Data.getX() << " 4.41 5.45 55R";
    qDebug() << QString::fromStdString(msg.str());
    emit device->sendingMessage(QString::fromStdString(msg.str()));
}
