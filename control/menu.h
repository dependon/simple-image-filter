#ifndef MENU_H
#define MENU_H

#ifdef USE_DTK
#include <DMenu>
DWIDGET_USE_NAMESPACE
typedef  DMenu MyMunu;
#else
#include <QMenu>
typedef QMenu MyMunu;
#endif
class Menu :public MyMunu
{
public:
    explicit Menu(QWidget *parent = nullptr);
};

#endif // MENU_H
