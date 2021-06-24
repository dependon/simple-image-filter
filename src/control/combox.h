#ifndef COMBOX_H
#define COMBOX_H

#ifdef USE_DTK
#include <DComboBox>
DWIDGET_USE_NAMESPACE
typedef  DComboBox MyComboBox;
#else
#include <QComboBox>
typedef QComboBox MyComboBox;
#endif
class combox: public MyComboBox
{
    Q_OBJECT
public:
    explicit combox(QWidget *parent = nullptr);
};

#endif // COMBOX_H
