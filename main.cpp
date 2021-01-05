#include "mainwidget.h"
#include "application.h"

#ifdef USE_DTK
#include <DApplication>
#include <DMainWindow>
DWIDGET_USE_NAMESPACE
using namespace Dtk::Core;

int main(int argc, char *argv[])
{

    qRegisterMetaType<isChange>("isChange");
    Application a(argc, argv);

    DMainWindow w;
    MainWidget widget(&w);
    for(int i = 1; i < argc; ++i)
    {
        widget.openImage(argv[i]);
        break;
    }

    w.setCentralWidget(&widget);
    w.resize(800,600);
    w.show();

    return a.exec();
}
#else
int main(int argc, char *argv[])
{
    Application a(argc, argv);

    MainWidget w;
    w.resize(800,600);
    w.show();

    return a.exec();
}
#endif
