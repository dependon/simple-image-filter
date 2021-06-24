#include "scaledialog.h"
#include "ui_scaledialog.h"

#include "application.h"

scaleDialog::scaleDialog(const QImage &img, QWidget *parent) :
    QWidget(parent),
    m_img(img),
    ui(new Ui::scaleDialog)
{
    ui->setupUi(this);
    ui->widthSpinBox->setValue(m_img.width());
    ui->heightSpinBox->setValue(m_img.height());
    setWindowTitle(tr("Scale window"));
}

scaleDialog::~scaleDialog()
{
    delete ui;
}

void scaleDialog::on_okBtn_clicked()
{
    App->sigFilterImage(m_img.scaled(ui->widthSpinBox->value(), ui->heightSpinBox->value()));
    QWidget *widget = static_cast<QWidget *>(parent());
    if (widget) {
        widget->close();
    }
}

void scaleDialog::on_cancelBtn_clicked()
{
    QWidget *widget = static_cast<QWidget *>(parent());
    if (widget) {
        widget->close();
    }
}
