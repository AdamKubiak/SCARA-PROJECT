#ifndef DEVICE_H
#define DEVICE_H


#include <QSerialPort>
#include <QObject>
#include <QQueue>
#include <datacontainer.h>
#include <checksum.h>
#include <fstream>
#include <QVector3D>
#include <QQuaternion>
#include <QTime>
#include <QMessageBox>

class Device : public QObject
{
    Q_OBJECT
public:

    Device(QObject* parent = nullptr);
    ~Device();

    /*!
     * \brief Metoda umożliwia dostanie się do pola serialName poza klasą Device.
     * \return Zwraca ciąg znaków z nazwą portu.
     */
    QString& getSerialPortName();

    //bool getIsConnected();

    /*!
     * \brief Metoda umożliwia dostanie się do pola serialPort klasy Device.
     * \return Wskaźnik na pole serialPort klasy Device.
     */
    QSerialPort *get_SerialPortObject();
    /*!
     * \brief Metoda resetuje wartości pól przechowujących aktualną pozycję obiektu do pozycji zerowej.
     */
    void resetDeviceValues();

    /*!
     * \brief Pole klasy QMessegeBox odpowiedzialne za wyświetlenie komunikatu trwania kalibracji.
     */
    QMessageBox *info;
public slots:

    /*!
     * \brief Slot ustawia wartość pola serialName.
     *
     * \param[in] name - Ciąg znaków z nazwą portu szeregowego.
     */
    void setSerialName(const QString name);

    /*!
     * \brief Slot odpowiedzialny za odbieranie danych z portu szeregowego.
     */
    void getData();
    /*!
     * \brief Slot odpowiedzialny za rozpoczęcie komunikacji z wybranym portem szeregowym.
     */
    void startCommunication();

    void sendMessageToDevice(QString message);

signals:
    /*!
     * \brief Sygnał wysyłany przy odebraniu i sprawdzeniu poprawności przesyłu danych.
     */
    void newDeviceValues(int x, int y);
    /*!
     * \brief Sygnał wysyłany po zakończeniu kalibracji żyroskopu.
     */
    void gyroCalibrated();
    /*!
     * \brief Sygnał wysyłany po obliczeniu orientacji obiektu z otrzymanych danych z żyroskopu.
     * \param[in] QQuaternion - wyliczone dane w postaci quaternionu.
     *
     */
    void orientationChanged(const QQuaternion&);
    /*!
     * \brief Sygnał wysyłany po obliczeniu translacji obiektu z otrzymanych danych z akcelerometru.
     * \param[in] QVector3D - wyliczone dane w postaci struktury QVector3D.
     */

    void sendingMessage(QString message);

    void positionChanged(const QVector3D&);
    /*!
     * \brief Sygnał wysyła surowe dane z każdej osi żyroskopu do odpowiednich wykresów.
     * \param[in] QVector3D - surowe dane w postaci wektora3D.
     * \param[in] char - identyfikator czujnika.
     */
    void sendEncoderDatatoChart(const QVector3D&,char);


private:
    int XgyroMAX; //!< maks przedziału pomijalnej wartości obliczonej z  kalibracji żyroskopu dla osi OX.
    int XgyroMIN; //!< minimum przedziału pomijalnej wartości obliczonej z  kalibracji żyroskopu dla osi OX.
    int YgyroMAX;//!< maks przedziału pomijalnej wartości obliczonej z  kalibracji żyroskopu dla osi OY.
    int YgyroMIN;//!< minimum przedziału pomijalnej wartości obliczonej z  kalibracji żyroskopu dla osi OY.
    int ZgyroMAX;//!< maks przedziału pomijalnej wartości obliczonej z  kalibracji żyroskopu dla osi OZ.
    int ZgyroMIN;//!< minimum przedziału pomijalnej wartości obliczonej z  kalibracji żyroskopu dla osi OZ.

    int XgyroERROR;//!< wartość wyliczona z kalibracji żyroskopu, która jest odejmowana od otrzymanych danych z osi X.
    int YgyroERROR;//!< wartość wyliczona z kalibracji żyroskopu, która jest odejmowana od otrzymanych danych z osi Y.
    int ZgyroERROR;//!< wartość wyliczona z kalibracji żyroskopu, która jest odejmowana od otrzymanych danych z osi Z.

    int XaccMAX; //!< maks przedziału pomijalnej wartości obliczonej z  kalibracji akcelerometru dla osi OX.
    int XaccMIN; //!< minimum przedziału pomijalnej wartości obliczonej z  kalibracji akcelerometru dla osi OX.
    int YaccMAX; //!< maks przedziału pomijalnej wartości obliczonej z  kalibracji akcelerometru dla osi OY.
    int YaccMIN; //!< minimum przedziału pomijalnej wartości obliczonej z  kalibracji akcelerometru dla osi OY.
    int ZaccMAX; //!< maks przedziału pomijalnej wartości obliczonej z  kalibracji akcelerometru dla osi OZ.
    int ZaccMIN; //!< minimum przedziału pomijalnej wartości obliczonej z  kalibracji akcelerometru dla osi OZ.

    float correctionX; //!< w polu przechowywany jest wynik otrzymany z przeprowadzanie filtracji otrzymanych danych dla osi OX
    float correctionY; //!< w polu przechowywany jest wynik otrzymany z przeprowadzanie filtracji otrzymanych danych dla osi OY
    float correctionZ; //!< w polu przechowywany jest wynik otrzymany z przeprowadzanie filtracji otrzymanych danych dla osi OZ

    int XaccERROR;//!< wartość wyliczona z kalibracji akcelerometru, która jest odejmowana od otrzymanych danych z osi OX.
    int YaccERROR;//!< wartość wyliczona z kalibracji akcelerometru, która jest odejmowana od otrzymanych danych z osi OY.
    int ZaccERROR;//!< wartość wyliczona z kalibracji akcelerometru, która jest odejmowana od otrzymanych danych z osi OZ.

    QString serialName;//!< pole przechowuje nazwę portu z którym użytkownik chce się połaczyć.
    QSerialPort serialPort;//!< pole klasy QSerialPort odpowiedzialne za komunikację z portem szeregowym.
    DataContainer *dataContainer;//!< pole klasy DataContainer odpowiedzialne za przechowywanie odebranych wartości z sensorów.
    std::string data;//!< pole odpowiedzialne za tymczasowe przechowywanie odebranych danych w formie ciągu znaków.
    int Check;//!< pole pełniące rolę flagi do sprawdzania warunków.


    QVector3D rawAccelerometer; //!< pole wykorzystywane do tymczasowego przechowywania surowych wartości otrzymanych z akcelerometru dla wszystkich trzech osi.
    QVector3D orientationVector;//!< pole wykorzystywane do obliczenia orientacji obiektu przy pomocy reprezentacji Roll-Pitch-Yaw z danych z żyroskopu.
    QVector3D orientationAccVector;//!< pole wykorzystywane do obliczenia orientacji obiektu przy pomocy reprezentacji Roll-Pitch-Yaw z danych z akcelerometru.
    QVector3D gyroscopeData;//!< pole przechowujące najświeższe odebrane dane z żyroskopu.
    QVector3D accelerometerData;//!< pole przechowujące najświeższe odebrane dane z akcelerometru.
    QVector3D positionVector;//!< pole przechowujące aktualą pozycję obiektu w postaci Quaternionu.
    QVector3D positionVelocityVector;//!< pole przechowujące obliczone prędkości z akcelerometru w postaci wektora3D.
    QQuaternion orientationQauternion;//!< pole przechowujące aktualą orientacje obiektu w postaci Quaternionu.




};

#endif // DEVICE_H
