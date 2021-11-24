#ifndef OBJECT_SCENE_H
#define OBJECT_SCENE_H

#include "render_object.h"
#include "manipulator.h"
#include<Qt3DRender/QSceneLoader>
#include <QVector3D>
#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QDiffuseMapMaterial>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QTextureImage>


/*!
 * \class Object_Scene
 * \brief Klasa ma na celu stworzenie scene, kamerę, oświetlenie dla wybranego modelu 3D
 */
class Object_Scene : public QWidget
{
    Q_OBJECT
public:

    explicit Object_Scene(QWidget *parent = nullptr);

    /*!
     * \brief Metoda renderuje obiekt który aktualnie znajduję się w zmiennej
     * local_loader obiektu klasy Render_Object.
     */
    void createRenderObject();

    /*!
     * \brief Metoda odpowiedzialna za przypisywanie wartości pola typu QUrl i wyrenderowanie
     * obiektu z podanej ścieżki.
     *  \param[in]  renderFile - zmienna typu QUrl która przechowuje ścieżkę do obiektu 3D.
     */
    void setRenderFile(QUrl renderFile);

    /*!
     * \brief Metoda zwraca wskaźnik do pola klasy Object_Scene.
     * \return pole typu Render_Object.
     */
    Render_Object *get_RenderObject();

    void setTextureFile(QUrl textureFile);


    void setSceneLoader(QUrl renderFile);

    Render_Object* getObjectEntity(){return objectEntity;};

public slots:
    /*!
     * \brief Slot zmienia orientacje obiektu na podstawie otrzymanych danych.
     *  \param[in]  orientation - zmienna typu QQuaternion z wartościami rotacji obiektu 3D.
     */
    void setOrientation(float x,float y,float z);

    /*!
     * \brief Slot zmienia pozycje obiektu na podstawie otrzymanych danych.
     * \param[in] position - zmienna typu QVector3D z wartościami translacji obiektu.
     */
    void setPosition(const QVector3D &position);

signals:

    /*!
     * \brief Sygnał jest emitowany gdy orientacja obiektu ulega zmianie.
     * \param[in]QQuaternion - aktualna pozycja obiektu typu QQuaternion.
     */
    void orientationChanged(float x,float y,float z);


    /*!
     * \brief Sygnał jest emitowany gdy pozycja obiektu ulega zmianie.
     * \param QVector3D - aktualna pozycja obiektu typu QVector.
     */
    void positionChanged(const QVector3D&);




private:

    Qt3DCore::QEntity *rootEntity; //!< Główny obiekt sceny do której podpięte są wszystkie gałęzie które są elementami klasy Qt3D.

    Qt3DRender::QCamera *cameraEntity;//!<Kamera sceny.

    Render_Object* objectEntity;//!<pole klasy Render_Object w której przechowywane są informacje o aktualnie wyświetlanym obiektcie 3D.

    Manipulator* manip; //!< Manipulator object

    QQuaternion orientation; //!< aktualna orientacja obiektu w postaci quaternionu.

    QVector3D position;//!< aktualna pozycja obiektu w postaci wektora3D.

    QUrl renderFile; //!< Ścieżka do pliku który zawiera wybrany obiekt 3D.

    QUrl textureFile;

};

#endif // OBJECT_VIEW_SCENE_H
