#ifndef MANIPULATOR_H
#define MANIPULATOR_H

/*!
 * \file
 * \brief File contains 3D simple manipulator mode class
 */

#include <QCuboidMesh>
#include <QCylinderMesh>
#include <QObject>
#include <QTransform>
#include <Qt3DCore/QEntity>
#include <memory>
#include <render_object.h>

/*!
 * \class Manipulator
 * \brief The Manipulator class
 * Manipulator is built from 3 links + base, and has 3 joints - 3DOF
 */
class Manipulator : public QObject
{

    Q_OBJECT
public:
    /*!
     * \brief Manipulator constructor
     * \param scene - Parent 3D scene entity
     * \param parent - Parent object
     */
    Manipulator(Qt3DCore::QEntity* scene, QObject *parent = nullptr);

    ~Manipulator();
    /*!
     * \brief Creates (render) 3D manipulator
     */
    void createManipulator();

public slots:
    /*!
     * \brief Set Q1 - angle in first joint
     * \param val - angle in degrees
     */
    void setQ1(float val);
    /*!
     * \brief Set Q1 - angle in second joint
     * \param val - angle in degrees
     */
    void setQ2(float val);
    /*!
     * \brief Set Q1 - angle in second joint
     * \param val - angle in degrees
     */
    void setQ3(int val);

private:

    Qt3DCore::QEntity* sceneRoot; //!< Pointer to parent 3D scene entity

    Render_Object* Base;
    Render_Object* Link1;
    Render_Object* Link2;
    Render_Object* Link3;
    QUrl renderFile;

    QMatrix4x4 link1Matrix;    //!< Rotation matrix for first link
    QMatrix4x4 link2Matrix;    //!< Rotation matrix for second link
    QMatrix4x4 link3Matrix;    //!< Rotation matrix for third link

    float q1Rotation; //!< Angle in first joint
    float q2Rotation; //!< Angle in second joint
    int q3Rotation; //!< Angle in third joint

};

#endif // MANIPULATOR_H
