/*
 *用于后续自绘漂亮一点的菜单
*/
#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H

#ifdef USE_DTK
#include <DFloatingWidget>
DWIDGET_USE_NAMESPACE
typedef  DFloatingWidget FloatWidget;
#else
#include <QWidget>
typedef QWidget FloatWidget;
#endif
#include <QDragEnterEvent>
class StatusBarWidget : public FloatWidget
{
    Q_OBJECT
public:
    explicit StatusBarWidget(QWidget *parent = nullptr);

signals:

public slots:

};

#endif // STATUSBARWIDGET_H
