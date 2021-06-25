#include "toolbutton.h"

ToolButton::ToolButton(QWidget *parent): ToolBtn(parent)
{

}

ToolButton::ToolButton(const QString &text, QWidget *parent)
    : ToolBtn(parent)
{
    setText(text);
}
