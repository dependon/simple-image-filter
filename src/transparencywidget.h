#ifndef TRANSPARENCYWIDGET_H
#define TRANSPARENCYWIDGET_H

#include <QWidget>

namespace Ui {
class TransparencyWidget;
}

class TransparencyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransparencyWidget(QWidget *parent = nullptr);
    ~TransparencyWidget();
signals:
    void transparencyChange(const int &index);
private slots:
    void on_okBtn_clicked();

private:
    Ui::TransparencyWidget *ui;
};

#endif // TRANSPARENCYWIDGET_H
