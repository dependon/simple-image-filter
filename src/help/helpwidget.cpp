#include "helpwidget.h"
#include "ui_helpwidget.h"

helpWidget::helpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::helpWidget)
{
    ui->setupUi(this);
}

helpWidget::~helpWidget()
{
    delete ui;
}
