#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QWidget>

namespace Ui {
class scaleDialog;
}

class scaleDialog : public QWidget
{
    Q_OBJECT

public:
    explicit scaleDialog(const QImage &img, QWidget *parent = nullptr);
    ~scaleDialog();

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::scaleDialog *ui;
    QImage m_img;
};

#endif // SCALEDIALOG_H
