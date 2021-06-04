#include "scaledialog.h"
#include "ui_scaledialog.h"

#include "application.h"

scaleDialog::scaleDialog(const QImage &img, QWidget *parent) :
    QDialog(parent),
    m_img(img),
    ui(new Ui::scaleDialog)
{
    ui->setupUi(this);
    ui->widthSpinBox->setValue(m_img.width());
    ui->heightSpinBox->setValue(m_img.height());
    setWindowTitle(tr("scale window"));
}

scaleDialog::~scaleDialog()
{
    delete ui;
}

void scaleDialog::on_okBtn_clicked()
{
    App->sigFilterImage(m_img.scaled(ui->widthSpinBox->value(), ui->heightSpinBox->value()));
    close();
}

void scaleDialog::on_cancelBtn_clicked()
{
    close();
}
