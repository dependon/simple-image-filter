#ifndef OPENIMAGEBUTTON_H
#define OPENIMAGEBUTTON_H

#ifdef USE_DTK
#include <DSuggestButton>
DWIDGET_USE_NAMESPACE
typedef  DSuggestButton SuggestBtn;
#else
#include <QPushButton>
typedef QPushButton SuggestBtn;
#endif

class openImageButton: public SuggestBtn
{
    Q_OBJECT
public:
    explicit openImageButton(QWidget *parent = nullptr);

    explicit openImageButton(const QString &text, QWidget *parent = nullptr);
};
#endif // OPENIMAGEBUTTON_H
