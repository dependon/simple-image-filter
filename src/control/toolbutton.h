#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H


#ifdef USE_DTK
#include <DToolButton>
DWIDGET_USE_NAMESPACE
typedef  DToolButton ToolBtn;
#else
#include <QToolButton>
typedef QToolButton ToolBtn;
#endif

class ToolButton: public ToolBtn
{
    Q_OBJECT
public:
    explicit ToolButton(QWidget *parent = nullptr);

    explicit ToolButton(const QString &text, QWidget *parent = nullptr);
};

#endif // TOOLBUTTON_H
