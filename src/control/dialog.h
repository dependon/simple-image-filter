#ifndef DIALOG_H
#define DIALOG_H

#ifdef USE_DTK
#include <DWidget>
DWIDGET_USE_NAMESPACE
typedef  DWidget MyDialog;
#else
#include <QWidget>
typedef  QWidget MyDialog;
#endif
class Dialog : public MyDialog
{
    Q_OBJECT
public:
    Dialog(QWidget *parent = nullptr);
};

#endif // DIALOG_H
