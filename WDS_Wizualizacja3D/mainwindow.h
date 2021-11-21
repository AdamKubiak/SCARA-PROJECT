#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "object_widget.h"
#include "device.h"
#include "robot.h"
#include "sequencehandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
/*!
 * \class MainWindow
 * \brief Klasa MainWindow zarządza wszystkimi operacjami na obiektach sceny i elementami GUI
 *
  */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    /*!
     * \brief Metoda dodaje znaleziony porty COM do paska narzędzi, gdzie użytkownik może wybrać
     *  z którego z nich będę pobierane dane.
     */
    void menuAvailablePorts();

    /*!
     * \brief Metoda ustawia wartość rotacji wokół osi OX w stopniach w GUI.
     */
    void setXRotationValue(float value);
    /*!
     * \brief Metoda ustawia wartość rotacji wokół osi OY w stopniach w GUI.
     * \param[in] value - wartość rotacji.
     */
    void setYRotationValue(float value);
    /*!
     * \brief Metoda ustawia wartość rotacji wokół osi OZ w stopniach w GUI.
     * \param[in] value - wartość rotacji.
     */
    void setZRotationValue(float value);

    /*!
     * \brief Metoda ustawia wartość translacji wzdłuż osi OX w GUI.
     * \param[in] value - wartość rotacji.
     */
    void setXTranslationValue(float value);

    /*!
     * \brief Metoda ustawia wartość translacji wzdłuż osi OY w GUI.
     * \param[in] value - wartość translacji.
     */
    void setYTranslationValue(float value);

    /*!
     * \brief Metoda ustawia wartość translacji wzdłuż osi OZ w GUI.
     * \param[in] value - wartość translacji.
     */
    void setZTranslationValue(float value);

    /*!
     * \brief Metoda ustawia połączenie między sygnałami i slotami.
     */
    void addConnections();




public slots:
    /*!
     * \brief Slot przyjmuje quaternion z którego zostają wyliczone kąty
     *  dla każdej z osi i następuje przekazanie tych wartości do GUI.
     *  \param[in] value - wartość rotacji obiektu w quaternionach.
     */
    void setRotationValue(float x , float y,float z);

    /*!
     * \brief Slot przyjmuje strukture QVector3D, który zostaje rozbity na wartości translacji
     *  dla każdej z osi i następuje przekazanie tych wartości do GUI.
     *  \param[in] value - wartość translacji przekazywana w formie wektora.
     */
    void setTranslationValue(const QVector3D &value);
    /*!
     * \brief Slot odpowiedzialny za przypisanie nazwy wybranego portu do pola klasy Device.
     * \param[in] trigger - nazwa portu wybrana przez użytkownika.
     */
    void selectPort(QAction *trigger);

    /*!
     * \brief Slot inicjalizuje widgety z wykresami z pomocą funkcji dostarczonych w bibliotece qcustomplot.
     */
    void makePlot();

    /*!
     * \brief Slot odpowiedzialny za wyświetlenie danych na odpowiednich wykresach.
     * \param[in] Axis - surowe wartości odebrane z czujników w formie wektora3D.
     * \param[in] ID - identyfikator czujnika z którego odbierane są dane.
     */
    void setChartsValue(const QVector3D &Axis,char ID);
    //void setChartsValueAccelerometer(const QVector3D &Axis, char ID);

    void sliderToData(int value);
    void spinboxToData(double value);

    void setSlidersInfo();


private slots:
    /*!
     * \brief Slot odpowiedzialny za zresetowanie pozycji i orientacji obiektu za pomocą
     * przycisku RESET znajdującego się w GUI.
     */
    void on_resetViewButton_clicked();

    void on_motor1L_released();

    void on_motor1R_released();

    void on_motor2L_released();

    void on_motor2R_released();

    void on_motor1L_pressed();

    void on_motor1R_pressed();

    void on_motor2L_pressed();

    void on_motor2R_pressed();

    void on_pushButton_5_clicked();

    void on_positionSetButton_clicked();

signals:
    void infoSliderChanged();

private:
    Ui::MainWindow *ui;
    Object_Scene *objectScene; //!< Wskaźnik na scene 3D.
    Object_Widget *objectWidget;//!< Wskaźnik na widget sceny.
    Device *device; //<! Wskaźnik na urządzenie z którego są zbierane dane.
    DataContainer sliders_Data;
    QVector3D spinbox_Data;
    Robot *robot;
    SequenceHandler seqHandler;

    QVector<double>    Xgyro_y,Ygyro_y,Zgyro_y, dtTime; //!< wektory z danymi z żyroskopu do wyświetlenia na wykresach.
    QVector<double>    Xacc_y,Yacc_y,Zacc_y; //!< wektory z danymi z akcelerometru do wyświetlenia na wykresach.
    double Time;
    float dt; //!< wartość używana do reprezentcji czasu na osiach X wykresów.

};
#endif // MAINWINDOW_H
