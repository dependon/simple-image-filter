#ifndef SPINBOX_H
#define SPINBOX_H

#ifdef USE_DTK
#include <DSpinBox>
DWIDGET_USE_NAMESPACE
typedef  DSpinBox MySpinBox;
#else
#include <QSpinBox>
typedef QSpinBox MySpinBox;
#endif
class SpinBox: public MySpinBox
{
    Q_OBJECT
public:
    SpinBox(QWidget *parent = nullptr);
};

#endif // SPINBOX_H
