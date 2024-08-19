QT       += core gui
QT       += network

# additional modules are pulled in via arcgisruntime.pri
QT += opengl qml quick
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 15) {
        error("$$TARGET requires Qt 5.15.2")
    }
        equals(QT_MINOR_VERSION, 15) : lessThan(QT_PATCH_VERSION, 2) {
                error("$$TARGET requires Qt 5.15.2")
        }
}

equals(QT_MAJOR_VERSION, 6) {
  error("This version of the ArcGIS Runtime SDK for Qt is incompatible with Qt 6")
}

ARCGIS_RUNTIME_VERSION = 100.11.2
include($$PWD/arcgisruntime.pri)


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mymaparcgis.cpp \
    mymavlink.cpp \
    mytcpsocket.cpp \
    myyolothread.cpp \
    tcpserverclass.cpp \
    videocaptureworker.cpp

HEADERS += \
    mainwindow.h \
    mymaparcgis.h \
    mymavlink.h \
    mytcpsocket.h \
    myyolothread.h \
    parameters.h \
    paramsloader.h \
    structs.h \
    tcpserverclass.h \
    videocaptureworker.h

FORMS += \
    mainwindow.ui




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    picture.qrc

DISTFILES += \
    arcgisruntime.pri \
    esri_runtime_qt.pri

unix|win32: LIBS += -LC:/opencv/install/x64/vc16/lib/ -lopencv_world453

INCLUDEPATH += C:/opencv/install/include
DEPENDPATH += C:/opencv/install/include

INCLUDEPATH +=C:/Users/moudp/Downloads/eigen-3.4.0/eigen-3.4.0/Eigen
