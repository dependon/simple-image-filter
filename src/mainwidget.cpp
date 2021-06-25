#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "statusbarwidget.h"

#include "toolbutton.h"
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
#include <QClipboard>
#include <QShortcut>

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
        initShortcut();
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

        m_openBtn = new ToolButton(m_statusbarWidget);
        m_openBtn->setShortcut(QKeySequence("Ctrl+O"));
        m_openBtn->setFixedSize(60, 60);
        m_openBtn->setToolTip(tr("open"));
        m_openBtn->setIcon(QIcon(":/icon/open.svg"));
        m_openBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_openBtn);
        connect(m_openBtn, &QToolButton::clicked, this, &MainWidget::openImageFile);

        m_resetBtn = new ToolButton(m_statusbarWidget);
        m_resetBtn->setShortcut(QKeySequence("Ctrl+E"));
        m_resetBtn->setFixedSize(60, 60);
        m_resetBtn->setToolTip(tr("reset"));
        m_resetBtn->setIcon(QIcon(":/icon/reset.svg"));
        m_resetBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_resetBtn);
        connect(m_resetBtn, &QToolButton::clicked, ui->mainImageView, &ImageView::resetImage);

        m_fitImageBtn = new ToolButton(m_statusbarWidget);
        m_fitImageBtn->setShortcut(QKeySequence("Ctrl+R"));
        m_fitImageBtn->setFixedSize(60, 60);
        m_fitImageBtn->setToolTip(tr("fitImage"));
        m_fitImageBtn->setIcon(QIcon(":/icon/dcc_11_36px.svg"));
        m_fitImageBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_fitImageBtn);
        connect(m_fitImageBtn, &QToolButton::clicked, ui->mainImageView, &ImageView::fitImage);

        m_fitWindowBtn = new ToolButton(m_statusbarWidget);
        m_fitWindowBtn->setShortcut(QKeySequence("Ctrl+T"));
        m_fitWindowBtn->setFixedSize(60, 60);
        m_fitWindowBtn->setToolTip(tr("fitWindow"));
        m_fitWindowBtn->setIcon(QIcon(":/icon/dcc_fit_36px.svg"));
        m_fitWindowBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_fitWindowBtn);
        connect(m_fitWindowBtn, &QToolButton::clicked, ui->mainImageView, &ImageView::fitWindow);

        m_rotateLeft = new ToolButton();
        m_rotateLeft->setShortcut(QKeySequence("Ctrl+Left"));
        m_rotateLeft->setFixedSize(60, 60);
        m_rotateLeft->setToolTip(tr("rotate-90"));
        m_rotateLeft->setIcon(QIcon(":/icon/dcc_left_36px.svg"));
        m_rotateLeft->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_rotateLeft);
        connect(m_rotateLeft, &QToolButton::clicked, ui->mainImageView, [ = ] {
            ui->mainImageView->RotateImage(-90);
        });

        m_rotateRight = new ToolButton();
        m_rotateRight->setShortcut(QKeySequence("Ctrl+Right"));
        m_rotateRight->setFixedSize(60, 60);
        m_rotateRight->setToolTip(tr("rotate+90"));
        m_rotateRight->setIcon(QIcon(":/icon/dcc_right_36px.svg"));
        m_rotateRight->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_rotateRight);
        connect(m_rotateRight, &QToolButton::clicked, ui->mainImageView, [ = ] {
            ui->mainImageView->RotateImage(90);
        });



        m_saveBtn = new ToolButton();
        m_saveBtn->setShortcut(QKeySequence("Ctrl+S"));
        m_saveBtn->setFixedSize(60, 60);
        m_saveBtn->setToolTip(tr("save"));
        m_saveBtn->setIcon(QIcon(":/icon/save.svg"));
        m_saveBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_saveBtn);
        connect(m_saveBtn, &QToolButton::clicked, ui->mainImageView, &ImageView::savecurrentPic);


        m_scaleImageBtn = new ToolButton();
        m_scaleImageBtn->setShortcut(QKeySequence("Ctrl+Y"));
        m_scaleImageBtn->setFixedSize(60, 60);
        m_scaleImageBtn->setToolTip(tr("scaleImage"));
        m_scaleImageBtn->setIcon(QIcon(":/icon/scale.svg"));
        m_scaleImageBtn->setIconSize(QSize(36, 36));
        m_Hlayout->addWidget(m_scaleImageBtn);
        connect(m_scaleImageBtn, &QToolButton::clicked, ui->mainImageView, &ImageView::scaleImage);



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
        m_openAction->setText(tr("Open images"));
        m_leftMenu->addAction(m_openAction);
        connect(m_openAction, &QAction::triggered, this, &MainWidget::openImageFile);

        m_copyAction = new QAction(m_leftMenu);
        m_copyAction->setText(tr("Copy"));
        m_leftMenu->addAction(m_copyAction);
        connect(m_copyAction, &QAction::triggered, this, [ = ] {
            QClipboard *cb = qApp->clipboard();
            QMimeData *newMimeData = new QMimeData();
            newMimeData->setImageData(ui->mainImageView->image());
            cb->setMimeData(newMimeData, QClipboard::Clipboard);
        });

        m_saveBAction = new QAction(m_leftMenu);
        m_saveBAction->setText(tr("Save"));
        m_leftMenu->addAction(m_saveBAction);
        connect(m_saveBAction, &QAction::triggered, ui->mainImageView, &ImageView::savecurrentPic);

        m_filterMenu = new Menu(this);
        m_filteraction = new QAction(m_leftMenu);
        m_filteraction->setText(tr("Filter"));
        m_filteraction->setMenu(m_filterMenu);
        m_leftMenu->addAction(m_filteraction);

        m_oldFilter = new QAction(m_filterMenu);
        m_oldFilter->setText(tr("Old photo filter"));
        m_filterMenu->addAction(m_oldFilter);
        connect(m_oldFilter, &QAction::triggered, ui->mainImageView, &ImageView::oldIMage);

        m_beepFilter = new QAction(m_filterMenu);
        m_beepFilter->setText(tr("Bilateral filtering"));
        m_filterMenu->addAction(m_beepFilter);
        connect(m_beepFilter, &QAction::triggered, this, [ = ] {
            ui->mainImageView->BEEPImage();
        });

        m_warnFilter = new QAction(m_filterMenu);
        m_warnFilter->setText(tr("Warm color filter"));
        m_filterMenu->addAction(m_warnFilter);
        connect(m_warnFilter, &QAction::triggered, this, [ = ] {
            ui->mainImageView->warnImage();
        });

        m_coolFilter = new QAction(m_filterMenu);
        m_coolFilter->setText(tr("Cool color filter"));
        m_filterMenu->addAction(m_coolFilter);
        connect(m_coolFilter, &QAction::triggered, this, [ = ] {
            ui->mainImageView->coolImage();
        });

        m_grayScaleFilter = new QAction(m_filterMenu);
        m_grayScaleFilter->setText(tr("Gray filter"));
        m_filterMenu->addAction(m_grayScaleFilter);
        connect(m_grayScaleFilter, &QAction::triggered, ui->mainImageView, &ImageView::GrayScaleImage);

//        m_lightContrastImage = new QAction(m_leftMenu);
//        m_lightContrastImage->setText(tr("lightContrastImage"));
//        m_leftMenu->addAction(m_lightContrastImage);
//        connect(m_lightContrastImage, &QAction::triggered, this, [ = ] {
//            ui->mainImageView->lightContrastImage();
//        });
        m_inverseColorFilter = new QAction(m_filterMenu);
        m_inverseColorFilter->setText(tr("Anti color filter"));
        m_filterMenu->addAction(m_inverseColorFilter);
        connect(m_inverseColorFilter, &QAction::triggered, ui->mainImageView, &ImageView::InverseColorImage);

        m_Metal = new QAction(m_filterMenu);
        m_Metal->setText(tr("Metal filter"));
        m_filterMenu->addAction(m_Metal);
        connect(m_Metal, &QAction::triggered, ui->mainImageView, &ImageView::Metal);


        m_sharpeningMenu = new Menu(this);
        m_sharpeningAction = new QAction(m_leftMenu);
        m_sharpeningAction->setText(tr("Sharpening"));
        m_sharpeningAction->setMenu(m_sharpeningMenu);
        m_leftMenu->addAction(m_sharpeningAction);

        m_laplaceSharpen = new QAction(m_sharpeningMenu);
        m_laplaceSharpen->setText(tr("Laplacian sharpening"));
        m_sharpeningMenu->addAction(m_laplaceSharpen);
        connect(m_laplaceSharpen, &QAction::triggered, ui->mainImageView, &ImageView::LaplaceSharpenImage);

        m_soder = new QAction(m_sharpeningMenu);
        m_soder->setText(tr("Soder sharpening"));
        m_sharpeningMenu->addAction(m_soder);
        connect(m_soder, &QAction::triggered, ui->mainImageView, &ImageView::soderImage);




        m_flip = new QAction(m_leftMenu);
        m_flip->setText(tr("Flip"));
        m_leftMenu->addAction(m_flip);
        m_leftFlip = new Menu(this);
        m_flip->setMenu(m_leftFlip);

        m_flipVertical = new QAction(m_leftMenu);
        m_flipVertical->setText(tr("Flip Vertical"));
        m_leftFlip->addAction(m_flipVertical);
        connect(m_flipVertical, &QAction::triggered, ui->mainImageView, &ImageView::flipVertical);

        m_flipHorizontal = new QAction(m_leftMenu);
        m_flipHorizontal->setText(tr("Flip Horizontal"));
        m_leftFlip->addAction(m_flipHorizontal);
        connect(m_flipHorizontal, &QAction::triggered, ui->mainImageView, &ImageView::flipHorizontal);

        m_scale = new QAction(m_leftMenu);
        m_scale->setText(tr("Crop a picture"));
        m_leftMenu->addAction(m_scale);
        connect(m_scale, &QAction::triggered, ui->mainImageView, &ImageView::scaled);

        m_transparency = new QAction(m_leftMenu);
        m_transparency->setText(tr("Set Transparency"));
        m_leftMenu->addAction(m_transparency);
        connect(m_transparency, &QAction::triggered, ui->mainImageView, &ImageView::SetTransparency);

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

void MainWidget::initShortcut()
{
    QShortcut *sc = new QShortcut(QKeySequence("Ctrl+="), this);
    sc->setContext(Qt::WindowShortcut);
    connect(sc, &QShortcut::activated, this, [ = ] {
        ui->mainImageView->setScaleValue(1.1);
    });
    sc = new QShortcut(QKeySequence("Ctrl+-"), this);
    sc->setContext(Qt::WindowShortcut);
    connect(sc, &QShortcut::activated, this, [ = ] {
        ui->mainImageView->setScaleValue(0.9);
    });
    sc = new QShortcut(QKeySequence("Ctrl+C"), this);
    connect(sc, &QShortcut::activated, this, [ = ] {
        QClipboard *cb = qApp->clipboard();
        QMimeData *newMimeData = new QMimeData();
        newMimeData->setImageData(ui->mainImageView->image());
        cb->setMimeData(newMimeData, QClipboard::Clipboard);
    });
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
