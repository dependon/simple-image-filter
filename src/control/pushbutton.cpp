#include "pushbutton.h"

PushButton::PushButton(QWidget *parent): PushBtn(parent)
{

}

PushButton::PushButton(const QString &text, QWidget *parent)
    : PushBtn(parent)
{
    setText(text);
}
