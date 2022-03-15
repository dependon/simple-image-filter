#-------------------------------------------------
#
# Project created by QtCreator 2020-12-14T15:11:40
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple-image-filter
TEMPLATE = app
CONFIG -= app_bundle
CONFIG += c++11 link_pkgconfig

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

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
    control/menu.cpp \
    control/openimagebutton.cpp \
    scaledialog.cpp \
    control/dialog.cpp \
    control/combox.cpp \
    control/lineedit.cpp \
    control/label.cpp \
    control/spinbox.cpp \
    transparencywidget.cpp \
    control/toolbutton.cpp \
    shortcut/shortcut.cpp \
    help/helpwidget.cpp

HEADERS += \
        mainwidget.h \
        application.h \
        control/statusbarwidget.h \
        control/pushbutton.h \
        thread/imagethread.h \
        view/imageview.h \
        api/imageapi.h \
    control/menu.h \
    control/openimagebutton.h \
    scaledialog.h \
    control/dialog.h \
    control/combox.h \
    control/lineedit.h \
    control/label.h \
    control/spinbox.h \
    transparencywidget.h \
    control/toolbutton.h \
    shortcut/shortcut.h \
    help/helpwidget.h

FORMS += \
        mainwidget.ui \
    scaledialog.ui \
    transparencywidget.ui \
    help/helpwidget.ui



#./translation/
#TRANSLATIONS += simple-image-filter_zh_CN.ts

CONFIG(release, debug|release) {
    TRANSLATIONS = $$files($$PWD/translations/*.ts)
    #遍历目录中的ts文件，调用lrelease将其生成为qm文件
    for(tsfile, TRANSLATIONS) {
        qmfile = $$replace(tsfile, .ts$, .qm)
        system(lrelease $$tsfile -qm $$qmfile) | error("Failed to lrelease")
    }
}

APPSHAREDIR = /usr/share/simple-image-filter
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin

translations.path = $$APPSHAREDIR/translations
translations.files = $$PWD/translations/*.qm

desktop.path = /usr/share/applications/
desktop.files = $$PWD/install/simple-image-filter.desktop

icon.path =/usr/share/icons
icon.files=$$PWD/icon/simple-image-filter.png

!isEmpty(target.path): INSTALLS += target translations icon desktop

RESOURCES += \
    qrc.qrc
TRANSLATIONS += \
    translations/simple-image-filter_zh_CN.ts
