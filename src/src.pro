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
isEmpty(PREFIX){
    PREFIX = /usr
}

APPSHAREDIR = $${PREFIX}/share/simple-image-filter
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = $${PREFIX}/bin

translations.path = $$APPSHAREDIR/translations
translations.files = $$PWD/translations/*.qm

desktop.path = $${PREFIX}/share/applications/
desktop.files = $$PWD/install/simple-image-filter.desktop

icon.path =$${PREFIX}/share/icons
icon.files=$$PWD/icon/simple-image-filter.png

!isEmpty(target.path): INSTALLS += target translations icon desktop

RESOURCES += \
    qrc.qrc
TRANSLATIONS += \
    translations/simple-image-filter_en.ts \ #English
    translations/simple-image-filter_zh_CN.ts \ #中文
    translations/simple-image-filter_zh_TW.ts \ #中文繁体
    translations/simple-image-filter_zh_HK.ts \ #粤语
    translations/simple-image-filter_es.ts \ #西班牙语
    translations/simple-image-filter_pl.ts \ #波兰语
    translations/simple-image-filter_ja.ts \ #日语
    translations/simple-image-filter_de.ts \ #德语
    translations/simple-image-filter_ko.ts \ #韩语
    translations/simple-image-filter_it.ts \ #意大利语
    translations/simple-image-filter_fr.ts \ #法语
    translations/simple-image-filter_ru.ts \ #俄语
    translations/simple-image-filter_pt.ts \ #葡萄牙语
    translations/simple-image-filter_pt_BR.ts \  #葡萄牙-巴西
    translations/simple-image-filter_fi.ts \ #芬兰语
    translations/simple-image-filter_vi.ts \ #越南
    translations/simple-image-filter_tr.ts \ #土耳其
    translations/simple-image-filter_th.ts \ #泰国
    translations/simple-image-filter_hu.ts \ #匈牙利
    translations/simple-image-filter_ro.ts \ #罗马尼亚
    translations/simple-image-filter_nl.ts \ #荷兰语
    translations/simple-image-filter_et.ts \ #爱沙尼亚语
    translations/simple-image-filter_bg.ts \ #保加利亚语
    translations/simple-image-filter_cs.ts \ #捷克语
    translations/simple-image-filter_da.ts \ #丹麦语
    translations/simple-image-filter_el.ts \ #希腊语
