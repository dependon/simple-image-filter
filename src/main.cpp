/*
 * Copyright (C) 2020 ~ 2021 LiuMingHang.
 *
 * Author:     LiuMingHang <liuminghang0821@gmail.com>
 *
 * Maintainer: LiuMingHang <liuminghang0821@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
#include <QDir>
#include <QDirIterator>
#include <QTranslator>
#define TRANSALTIONPATH "/usr/share/simple-image-filter/translations"
int main(int argc, char *argv[])
{
    Application a(argc, argv);
#ifdef Q_OS_LINUX
    QDir dir(TRANSALTIONPATH);
    if (dir.exists()) {
        QDirIterator qmIt(TRANSALTIONPATH, QStringList() << QString("*%1.qm").arg(QLocale::system().name()), QDir::Files);
        while (qmIt.hasNext()) {
            qmIt.next();
            QFileInfo finfo = qmIt.fileInfo();
            QTranslator *translator = new QTranslator;
            if (translator->load(finfo.baseName(), finfo.absolutePath())) {
                qApp->installTranslator(translator);
            }
        }
    }
#endif
    qRegisterMetaType<isChange>("isChange");
    MainWidget w;
    w.resize(800, 600);
    w.show();

    return a.exec();
}
#endif
