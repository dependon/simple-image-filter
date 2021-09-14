#include "mainwidget.h"
#include "application.h"

#ifdef USE_DTK
#include <DApplication>
#include <DMainWindow>
#include <DTitlebar>
#include <DWidgetUtil>
#include <DMenu>
#include <DDialog>
#include "help/helpwidget.h"
DWIDGET_USE_NAMESPACE
using namespace Dtk::Core;
using namespace Dtk::Widget;

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
    w.setWindowIcon(QIcon(":/icon/simple-image-filter.png"));
    w.titlebar()->setIcon(QIcon(":/icon/simple-image-filter.png"));
    w.setCentralWidget(&widget);
    w.resize(800, 600);
    w.show();
    DMenu *menu = new DMenu(&w);
    QAction *action = new QAction(menu);
    action->setText(QObject::tr("Help"));
    QObject::connect(action, &QAction::triggered, [ = ] {
        helpWidget *widget = new helpWidget();
        widget->show();
        DDialog ss;
        ss.setIcon(QIcon(":/icon/simple-image-filter.png"));
        ss.setFixedSize(430, 520);
        widget->setParent(&ss);
        ss.exec();
    });
    menu->addAction(action);
    w.titlebar()->setMenu(menu);
//看图居中打开
    Dtk::Widget::moveToCenter(&w);
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
