/*
 *application，并且带有全局信号，通过App获得指针
*/
#ifndef APPLICATION_H
#define APPLICATION_H
#include "mainwidget.h"
#include <QCoreApplication>
enum MenuItemId {
    IdNormal,
    IdBEEP,
    IdWarn,
    IdCool,
    IdGrayScale,
    IdInverseColor,
    Idold,
    IdlightContrast,
    IdLaplaceSharpen,
    IdSobel,
    IdHorizontal,
    IdVertical,
    IdContourExtraction,
    IdMetal

};
enum isChange {
    Change,
    UnChange,
};

#ifdef USE_DTK
#include <DApplication>
DWIDGET_USE_NAMESPACE
typedef  DApplication AppS;
#else
#include <QApplication>
typedef QApplication AppS;
#endif

#define App (static_cast<Application*>(QCoreApplication::instance()))
class Application : public AppS
{
    Q_OBJECT
public:
    Application(int &argc, char **argv);

signals:
    //设置栈窗口(0为打开的初始界面,1为图像显示界面)
    void setStackWidget(const int &index);
    //图形处理信号，用于线程处理完毕发送给UI显示
//    void sigFilterImage(QImage *);
    void sigFilterImage(QImage, isChange is = Change);
    //鼠标移动信号，由于在view里面鼠标移动后，事件没有渗透到主窗口，特此写了一个信号
    void sigMouseMove();
private:

};

#endif // APPLICATION_H
