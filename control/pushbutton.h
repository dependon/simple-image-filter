/*
 *用于后续自绘漂亮一点的菜单
*/
#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#ifdef USE_DTK
#include <DPushButton>
DWIDGET_USE_NAMESPACE
typedef  DPushButton PushBtn;
#else
#include <QPushButton>
typedef QPushButton PushBtn;
#endif

class PushButton:public PushBtn
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = nullptr);
};

#endif // ICONBUTTON_H
