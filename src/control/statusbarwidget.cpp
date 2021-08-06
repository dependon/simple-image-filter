#include "statusbarwidget.h"
#include <QMimeData>

StatusBarWidget::StatusBarWidget(QWidget *parent) : FloatWidget(parent)
{
    this->setAcceptDrops(true);//启用拖动事件
#ifdef USE_DTK
    setBlurBackgroundEnabled(true);
#endif
}
