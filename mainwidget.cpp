#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "statusbarwidget.h"
#include "pushbutton.h"
#include "imageview.h"
#include "application.h"
#include "menu.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QDragEnterEvent>
#include <QDebug>
#include <QMimeData>
#include <QPropertyAnimation>
#include <QSlider>
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    Init();
    qApp->installEventFilter(this);
}

bool MainWidget::Init()
{
    bool iRet = false;
    this->setAcceptDrops(true);//启用拖动事件
    if (!ui->mainImageView->scene()) {
        QGraphicsScene *scene = new QGraphicsScene(ui->mainImageView);
        ui->mainImageView->setScene(scene);

        QGraphicsScene *basicScene = new QGraphicsScene(ui->basicImageView);
        ui->basicImageView->setScene(basicScene);
        ui->basicImageView->setViewId(ViewId::Basic);
    }
    if (!m_statusbarWidget) {
        m_statusbarWidget = new StatusBarWidget(this);


        m_Vlayout = new QVBoxLayout(m_statusbarWidget);
        m_layoutframe = new QFrame(m_statusbarWidget);
        m_Hlayout = new QHBoxLayout(m_layoutframe);
        m_layoutframe->setLayout(m_Hlayout);
        m_Vlayout->addWidget(m_layoutframe);
        m_statusbarWidget->setLayout(m_Vlayout);
        m_statusbarWidget->setFixedSize(6 * 90, 170);
        m_statusbarWidget->move((this->width() - m_statusbarWidget->width()) / 2,
                                this->height() - m_statusbarWidget->height());

        initBtn();
        initConnect();
        initMenu();
        iRet = true;
    }
    setCurrentWidget(0);
    return iRet;
}

void MainWidget::initBtn()
{
    if (m_statusbarWidget) {
        m_lightSlider = new QSlider(Qt::Horizontal, this); //这个初始值，是让这个控件水平布局
        m_lightSlider->setRange(0, 300);
        m_lightSlider->setValue(100);
        m_contrastSlider = new QSlider(Qt::Horizontal, this); //这个初始值，是让这个控件水平布局
        m_contrastSlider->setRange(0, 300);
        m_contrastSlider->setValue(150);

        connect(m_lightSlider, &QSlider::sliderReleased, this, [ = ] {
            ui->mainImageView->lightContrastImage(m_lightSlider->value(), m_contrastSlider->value());
        });
        connect(m_contrastSlider, &QSlider::sliderReleased, this, [ = ] {
            ui->mainImageView->lightContrastImage(m_lightSlider->value(), m_contrastSlider->value());
        });
        m_Vlayout->addWidget(m_lightSlider);
        m_Vlayout->addWidget(m_contrastSlider);

        m_openBtn = new PushButton(m_statusbarWidget);
        m_openBtn->setFixedSize(60, 60);
        m_openBtn->setToolTip(tr("open"));
        m_openBtn->setIcon(QIcon(":/icon/open.svg"));
        m_openBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_openBtn);
        connect(m_openBtn, &QPushButton::clicked, this, &MainWidget::openImageFile);

        m_resetBtn = new PushButton(m_statusbarWidget);
        m_resetBtn->setFixedSize(60, 60);
        m_resetBtn->setToolTip(tr("reset"));
        m_resetBtn->setIcon(QIcon(":/icon/reset.svg"));
        m_resetBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_resetBtn);
        connect(m_resetBtn, &QPushButton::clicked, ui->mainImageView, &ImageView::resetImage);

        m_fitImageBtn = new PushButton(m_statusbarWidget);
        m_fitImageBtn->setFixedSize(60, 60);
        m_fitImageBtn->setToolTip(tr("fitImage"));
        m_fitImageBtn->setIcon(QIcon(":/icon/dcc_11_36px.svg"));
        m_fitImageBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_fitImageBtn);
        connect(m_fitImageBtn, &QPushButton::clicked, ui->mainImageView, &ImageView::fitImage);

        m_fitWindowBtn = new PushButton(m_statusbarWidget);
        m_fitWindowBtn->setFixedSize(60, 60);
        m_fitWindowBtn->setToolTip(tr("fitWindow"));
        m_fitWindowBtn->setIcon(QIcon(":/icon/dcc_fit_36px.svg"));
        m_fitWindowBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_fitWindowBtn);
        connect(m_fitWindowBtn, &QPushButton::clicked, ui->mainImageView, &ImageView::fitWindow);

        m_rotateLeft = new PushButton();
        m_rotateLeft->setFixedSize(60, 60);
        m_rotateLeft->setToolTip(tr("rotate-90"));
        m_rotateLeft->setIcon(QIcon(":/icon/dcc_left_36px.svg"));
        m_rotateLeft->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_rotateLeft);
        connect(m_rotateLeft, &QPushButton::clicked, ui->mainImageView, [ = ] {
            ui->mainImageView->RotateImage(-90);
        });

        m_rotateRight = new PushButton();
        m_rotateRight->setFixedSize(60, 60);
        m_rotateRight->setToolTip(tr("rotate+90"));
        m_rotateRight->setIcon(QIcon(":/icon/dcc_right_36px.svg"));
        m_rotateRight->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_rotateRight);
        connect(m_rotateRight, &QPushButton::clicked, ui->mainImageView, [ = ] {
            ui->mainImageView->RotateImage(90);
        });



        m_saveBtn = new PushButton();
        m_saveBtn->setFixedSize(60, 60);
        m_saveBtn->setToolTip(tr("save"));
        m_saveBtn->setIcon(QIcon(":/icon/save.svg"));
        m_saveBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_saveBtn);
        connect(m_saveBtn, &QPushButton::clicked, ui->mainImageView, &ImageView::savecurrentPic);


        m_scaleImageBtn = new PushButton();
        m_scaleImageBtn->setFixedSize(60, 60);
        m_scaleImageBtn->setToolTip(tr("scaleImage"));
        m_scaleImageBtn->setIcon(QIcon(":/icon/scale.svg"));
        m_scaleImageBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_scaleImageBtn);
        connect(m_scaleImageBtn, &QPushButton::clicked, ui->mainImageView, &ImageView::scaleImage);



    }

}

void MainWidget::initConnect()
{

    connect(App, &Application::setStackWidget, this, &MainWidget::setCurrentWidget);
    connect(App, &Application::sigMouseMove, this, [ = ] {
        if (window()->isFullScreen() && m_statusbarWidget)
        {
            QPoint pos = mapFromGlobal(QCursor::pos());
            if (height() - 20 < pos.y() && height() > pos.y() && height() == m_statusbarWidget->y()) {
                QPropertyAnimation *animation = new QPropertyAnimation(m_statusbarWidget, "pos");
                animation->setDuration(200);
                animation->setEasingCurve(QEasingCurve::NCurveTypes);
                animation->setStartValue(
                    QPoint((width() - m_statusbarWidget->width()) / 2, m_statusbarWidget->y()));
                animation->setEndValue(QPoint((width() - m_statusbarWidget->width()) / 2,
                                              height() - m_statusbarWidget->height() - 10));
                animation->start(QAbstractAnimation::DeleteWhenStopped);
            } else if (height() - m_statusbarWidget->height() - 10 > pos.y() &&
                       height() - m_statusbarWidget->height() - 10 == m_statusbarWidget->y()) {
                QPropertyAnimation *animation = new QPropertyAnimation(m_statusbarWidget, "pos");
                animation->setDuration(200);
                animation->setEasingCurve(QEasingCurve::NCurveTypes);
                animation->setStartValue(
                    QPoint((width() - m_statusbarWidget->width()) / 2, m_statusbarWidget->y()));
                animation->setEndValue(QPoint((width() - m_statusbarWidget->width()) / 2, height()));
                animation->start(QAbstractAnimation::DeleteWhenStopped);
            }
        }
    });
}

void MainWidget::initMenu()
{
    if (!m_leftMenu) {
        m_leftMenu = new Menu(this);
        m_openAction = new QAction(m_leftMenu);
        m_openAction->setText(tr("open"));
        m_leftMenu->addAction(m_openAction);
        connect(m_openAction, &QAction::triggered, this, &MainWidget::openImageFile);

        m_fitImageAction = new QAction(m_leftMenu);
        m_fitImageAction->setText(tr("fitImage"));
        m_leftMenu->addAction(m_fitImageAction);
        connect(m_fitImageAction, &QAction::triggered, ui->mainImageView, &ImageView::fitImage);

        m_fitWindowAction = new QAction(m_leftMenu);
        m_fitWindowAction->setText(tr("fitWindow"));
        m_leftMenu->addAction(m_fitWindowAction);
        connect(m_fitWindowAction, &QAction::triggered, ui->mainImageView, &ImageView::fitWindow);

        m_rotateRightAction = new QAction(m_leftMenu);
        m_rotateRightAction->setText(tr("rotate+90"));
        m_leftMenu->addAction(m_rotateRightAction);
        connect(m_rotateRightAction, &QAction::triggered, ui->mainImageView, [ = ] {
            ui->mainImageView->RotateImage(90);
        });
        m_rotateLeftAction = new QAction(m_leftMenu);
        m_rotateLeftAction->setText(tr("rotate-90"));
        m_leftMenu->addAction(m_rotateLeftAction);
        connect(m_rotateLeftAction, &QAction::triggered, ui->mainImageView, [ = ] {
            ui->mainImageView->RotateImage(-90);
        });

        m_saveBAction = new QAction(m_leftMenu);
        m_saveBAction->setText(tr("save"));
        m_leftMenu->addAction(m_saveBAction);
        connect(m_saveBAction, &QAction::triggered, ui->mainImageView, &ImageView::savecurrentPic);

        m_oldFilter = new QAction(m_leftMenu);
        m_oldFilter->setText(tr("oldFilter"));
        m_leftMenu->addAction(m_oldFilter);
        connect(m_oldFilter, &QAction::triggered, ui->mainImageView, &ImageView::oldIMage);

        m_beepFilter = new QAction(m_leftMenu);
        m_beepFilter->setText(tr("beepFilter"));
        m_leftMenu->addAction(m_beepFilter);
        connect(m_beepFilter, &QAction::triggered, this, [ = ] {
            ui->mainImageView->BEEPImage();
        });

        m_warnFilter = new QAction(m_leftMenu);
        m_warnFilter->setText(tr("warnFilter"));
        m_leftMenu->addAction(m_warnFilter);
        connect(m_warnFilter, &QAction::triggered, this, [ = ] {
            ui->mainImageView->warnImage();
        });

        m_coolFilter = new QAction(m_leftMenu);
        m_coolFilter->setText(tr("coolFilter"));
        m_leftMenu->addAction(m_coolFilter);
        connect(m_coolFilter, &QAction::triggered, this, [ = ] {
            ui->mainImageView->coolImage();
        });

        m_grayScaleFilter = new QAction(m_leftMenu);
        m_grayScaleFilter->setText(tr("grayScaleFilter"));
        m_leftMenu->addAction(m_grayScaleFilter);
        connect(m_grayScaleFilter, &QAction::triggered, ui->mainImageView, &ImageView::GrayScaleImage);

//        m_lightContrastImage = new QAction(m_leftMenu);
//        m_lightContrastImage->setText(tr("lightContrastImage"));
//        m_leftMenu->addAction(m_lightContrastImage);
//        connect(m_lightContrastImage, &QAction::triggered, this, [ = ] {
//            ui->mainImageView->lightContrastImage();
//        });

        m_inverseColorFilter = new QAction(m_leftMenu);
        m_inverseColorFilter->setText(tr("inverseColorFilter"));
        m_leftMenu->addAction(m_inverseColorFilter);
        connect(m_inverseColorFilter, &QAction::triggered, ui->mainImageView, &ImageView::InverseColorImage);

    }

}

bool MainWidget::Uninit()
{
    bool iRet = false;
    if (m_statusbarWidget) {
        m_statusbarWidget->deleteLater();
        m_statusbarWidget = nullptr;
        iRet = true;
    }
    return iRet;
}

void MainWidget::setCurrentWidget(const int &index)
{
    switch (index) {
    case 0:
        ui->stackedWidget->setCurrentIndex(0);
        m_statusbarWidget->hide();
        break;
    case 1:
        ui->stackedWidget->setCurrentIndex(1);
        if (m_statusbarWidget) {
            m_statusbarWidget->show();
            m_statusbarWidget->move((this->width() - m_statusbarWidget->width()) / 2,
                                    this->height() - m_statusbarWidget->height());
        }
        break;
    default:
        break;
    }

}

void MainWidget::openImage(const QString &path)
{
    ui->mainImageView->openImage(path);
    ui->basicImageView->openImage(path);
    setWindowTitle(QFileInfo(path).fileName());
}

MainWidget::~MainWidget()
{
    Uninit();
    delete ui;
}

void MainWidget::showEvent(QShowEvent *event)
{
    if (m_statusbarWidget) {
        if (m_statusbarWidget->isVisible()) {
            m_statusbarWidget->move((this->width() - m_statusbarWidget->width()) / 2,
                                    this->height() - m_statusbarWidget->height());
        }
    }
    return QWidget::showEvent(event);
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    if (m_statusbarWidget) {
        if (m_statusbarWidget->isVisible()) {
            m_statusbarWidget->move((this->width() - m_statusbarWidget->width()) / 2,
                                    this->height() - m_statusbarWidget->height());
        }
    }
    return QWidget::resizeEvent(event);
}


void MainWidget::openImageFile()
{
    QString strPath = QFileDialog::getOpenFileName();
    openImage(strPath);

}

bool MainWidget::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj)
    if (event->type() == QEvent::Drop) {

    }
    if (event->type() == QEvent::DragMove) {
        event->accept();
    }
    if (event->type() == QEvent::DragEnter) {
        QList<QUrl> urls = static_cast<QDropEvent *>(event)->mimeData()->urls();
        if (urls.isEmpty()) {
            return false;
        }

        QStringList paths;
        for (QUrl url : urls) {
            const QString path = url.toLocalFile();
            paths << path;
        }

        if (!paths.isEmpty()) {
            ui->mainImageView->openImage(paths.at(0));
            ui->basicImageView->openImage(paths.at(0));
        }

        event->accept();
    }

    if (event->type() == QEvent::Resize && this->isVisible()) {

    }

    return false;
}

void MainWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (1 == ui->stackedWidget->currentIndex() && m_leftMenu) {
        m_leftMenu->exec(QCursor::pos());

        event->accept();
    }

}

void MainWidget::on_openPicture_clicked()
{
    openImageFile();
}
