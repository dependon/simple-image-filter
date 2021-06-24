#include "transparencywidget.h"
#include "ui_transparencywidget.h"

TransparencyWidget::TransparencyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransparencyWidget)
{
    ui->setupUi(this);
}

TransparencyWidget::~TransparencyWidget()
{
    delete ui;
}

void TransparencyWidget::on_okBtn_clicked()
{
    emit transparencyChange(ui->spinBox->value());
    QWidget *widget = static_cast<QWidget *>(parent());
    if (widget) {
        widget->close();
    }
}
