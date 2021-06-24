#ifndef DIALOG_H
#define DIALOG_H

#ifdef USE_DTK
#include <DWidget>
DWIDGET_USE_NAMESPACE
typedef  DWidget MyDialog;
#else
#include <QDialog>
typedef  QDialog MyDialog;
#endif
class Dialog : public DWidget
{
    Q_OBJECT
public:
    Dialog(QWidget *parent = nullptr);
};

#endif // DIALOG_H
