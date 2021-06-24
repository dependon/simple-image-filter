#ifndef LINEEDIT_H
#define LINEEDIT_H

#ifdef USE_DTK
#include <DLineEdit>
DWIDGET_USE_NAMESPACE
typedef  DLineEdit MyLineEdit;
#else
#include <QLineEdit>
typedef QLineEdit MyLineEdit;
#endif

class LineEdit: public MyLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget *parent = nullptr);

};

#endif // LINEEDIT_H
