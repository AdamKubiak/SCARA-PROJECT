QT       += core gui serialport
QT       += 3dcore 3drender 3dinput 3dextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    crc16.cpp \
    datacontainer.cpp \
    device.cpp \
    main.cpp \
    mainwindow.cpp \
    manipulator.cpp \
    object_scene.cpp \
    object_widget.cpp \
    qcustomplot.cpp \
    render_object.cpp \
    robot.cpp

HEADERS += \
    checksum.h \
    datacontainer.h \
    device.h \
    mainwindow.h \
    manipulator.h \
    object_scene.h \
    object_widget.h \
    qcustomplot.h \
    render_object.h \
    robot.h

FORMS += \
    mainwindow.ui \
    object_widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
