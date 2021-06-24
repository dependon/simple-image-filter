#ifndef LABEL_H
#define LABEL_H

#ifdef USE_DTK
#include <DLabel>
DWIDGET_USE_NAMESPACE
typedef  DLabel MyLabel;
#else
#include <QLabel>
typedef QLabel MyLabel;
#endif
class Label : public MyLabel
{
    Q_OBJECT
public:
    Label(QWidget *parent = nullptr);
};

#endif // LABEL_H
