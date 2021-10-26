#ifndef DATACONTAINER_H
#define DATACONTAINER_H
#include <stdint.h>
#include <QDataStream>
class DataContainer
{
public:
    DataContainer();
    /*!
     * \brief Konstruktor klasy DataContainer.
     * \param[in] ID_ - znak identyfikujący sensor.
     */
    DataContainer(char _ID,int _X,int _Y,int _Z);

    /*!
     * \brief Metoda ustawia wartość pola ID.
     * \param[in] ID_ - znak identyfikujący sensor.
     */
    void setID(char &ID_);
    /*!
     * \brief Metoda ustawia wartość pola X.
     * \param[in] X_ - wartość dla osi X.
     */
    void setX(int &X_);
    /*!
     * \brief Metoda ustawia wartość pola Y.
     * \param[in] Y_ -  wartość dla osi Y.
     */
    void setY(int &Y_);
    /*!
     * \brief Metoda ustawia wartość pola Z.
     * \param[in] Z_ - wartość dla osi Z.
     */
    void setZ(int &Z_);

    /*!
     * \brief Metoda zwraca wartość pola X.
     * \return wartość dla osi X.
     */
    int getX();
    /*!
     * \brief Metoda zwraca wartość pola Y.
     * \return wartość dla osi Y.
     */
    int getY();
    /*!
     * \brief Metoda zwraca wartość pola Z.
     * \return wartość dla osi Z.
     */
    int getZ();
    /*!
     * \brief Metoda zwraca wartość pola ID.
     * \return wartość identyfikatora.
     */
    char getID();

private:
    /*!
     * \brief Pole przechowujące id czujnika.
     *
     */
    char ID;
    /*!
     * \brief Pole przechowujące wartość prędkości/przyśpieszenia osi OX z czujników.
     *
     */
    int X;
    /*!
     * \brief Pole przechowujące wartość prędkości/przyśpieszenia osi OY z czujników.
     *
     */
    int Y;
    /*!
     * \brief Pole przechowujące wartość prędkości/przyśpieszenia osi OZ z czujników.
     *
     */
    int Z;

};

#endif // DATACONTAINER_H
