#include "mainwidget.h"
#include "application.h"

#ifdef USE_DTK
#include <DApplication>
#include <DMainWindow>
#include <DTitlebar>
DWIDGET_USE_NAMESPACE
using namespace Dtk::Core;

int main(int argc, char *argv[])
{

    qRegisterMetaType<isChange>("isChange");
    Application a(argc, argv);
    a.loadTranslator();
    DMainWindow w;
    MainWidget widget(&w);
    for (int i = 1; i < argc; ++i) {
        widget.openImage(argv[i]);
        break;
    }
    a.setApplicationDescription(
        QObject::tr(
            "<a href='https://github.com/dependon/simple-image-filter/'>github/simple-image-filter</a><br/>"
            "<span style='font-size:12pt;'>mail: liuminghang0821@gmail.com</span><br/><br/>"
            "Published under GPL V3"
        )
    );
    a.setApplicationVersion(DApplication::buildVersion("1.0.0"));
    w.setWindowIcon(QIcon(":/icon/icon.png"));
    w.titlebar()->setIcon(QIcon(":/icon/icon.png"));
    w.setCentralWidget(&widget);
    w.resize(800, 600);
    w.show();

    return a.exec();
}
#else
int main(int argc, char *argv[])
{
    Application a(argc, argv);
    qRegisterMetaType<isChange>("isChange");
    MainWidget w;
    w.resize(800, 600);
    w.show();

    return a.exec();
}
#endif
