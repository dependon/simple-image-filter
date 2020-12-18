#-------------------------------------------------
#
# Project created by QtCreator 2020-12-14T15:11:40
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcess
TEMPLATE = app
CONFIG -= app_bundle
CONFIG += c++11 link_pkgconfig

qtHaveModule(dtkwidget){
   PKGCONFIG += dtkwidget
   DEFINES += USE_DTK
}
!isEmpty(target.path)
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
INCLUDEPATH +=  control \
                thread \
                view \
                api
SOURCES += \
        main.cpp \
        mainwidget.cpp \
        application.cpp \
        control/statusbarwidget.cpp \
        control/pushbutton.cpp \
        thread/imagethread.cpp \
        view/imageview.cpp \
        api/imageapi.cpp \
    control/menu.cpp

HEADERS += \
        mainwidget.h \
        application.h \
        control/statusbarwidget.h \
        control/pushbutton.h \
        thread/imagethread.h \
        view/imageview.h \
        api/imageapi.h \
    control/menu.h

FORMS += \
        mainwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
