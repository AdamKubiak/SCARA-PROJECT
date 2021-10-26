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
    void setQ1(int val);
    /*!
     * \brief Set Q1 - angle in second joint
     * \param val - angle in degrees
     */
    void setQ2(int val);
    /*!
     * \brief Set Q1 - angle in second joint
     * \param val - angle in degrees
     */
    void setQ3(int val);

private:

    Qt3DCore::QEntity* sceneRoot; //!< Pointer to parent 3D scene entity

    Qt3DExtras::QCylinderMesh* baseMesh;  //!< Mesh for base of manipulator
    Qt3DExtras::QCylinderMesh* baseMesh1;  //!< Mesh for base of manipulator
    Qt3DExtras::QCuboidMesh* link1Mesh;    //!< Mesh for first link of manipulator
    Qt3DExtras::QCuboidMesh* link2Mesh;    //!< Mesh for second link of manipulator
    Qt3DExtras::QCuboidMesh* link3Mesh;    //!< Mesh for third link of manipulator

    Qt3DCore::QEntity* base;    //!< Base entity
    Qt3DCore::QEntity* base1;   //!< Base entity
    Qt3DCore::QEntity* link1;  //!< First link entity
    Qt3DCore::QEntity* link2;  //!< Second link entity
    Qt3DCore::QEntity* link3;  //!< Third link entity

    Qt3DCore::QTransform* base1Transform;
    Qt3DCore::QTransform* link1Transform;  //!< Transform object for first link
    Qt3DCore::QTransform* link2Transform;  //!< Transform object for second link
    Qt3DCore::QTransform* link3Transform;  //!< Transform object for third link

    QMatrix4x4 base1Matrix;
    QMatrix4x4 link1Matrix;    //!< Rotation matrix for first link
    QMatrix4x4 link2Matrix;    //!< Rotation matrix for second link
    QMatrix4x4 link3Matrix;    //!< Rotation matrix for third link

    int q1Rotation; //!< Angle in first joint
    int q2Rotation; //!< Angle in second joint
    int q3Rotation; //!< Angle in third joint

};

#endif // MANIPULATOR_H
