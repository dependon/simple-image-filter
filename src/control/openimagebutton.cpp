#include "openimagebutton.h"

openImageButton::openImageButton(QWidget *parent): SuggestBtn(parent)
{

}
openImageButton::openImageButton(const QString &text, QWidget *parent)
    : SuggestBtn(parent)
{
    setText(text);
}
