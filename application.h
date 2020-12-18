#ifndef APPLICATION_H
#define APPLICATION_H
#include "mainwidget.h"
#include <QCoreApplication>

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
    Application(int& argc, char **argv);

signals:
    void setStackWidget(const int& index);
    void sigFilterImage(QImage *);
    void sigMouseMove();
private:

};

#endif // APPLICATION_H
