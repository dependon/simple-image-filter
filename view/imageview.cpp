#include "imageview.h"
#include "mainwidget.h"
#include "imagethread.h"

#include <QPaintDevice>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileDialog>
#include <QMessageBox>
#include <QThreadPool>
#include <qmath.h>
const qreal MAX_SCALE_FACTOR = 20.0;
const qreal MIN_SCALE_FACTOR = 0.029;
#define devicePixelRatioF  devicePixelRatio
ImageView::ImageView(QWidget *parent):
    QGraphicsView(parent)
{
    setMouseTracking(true);
    setDragMode(ScrollHandDrag);
    QThreadPool::globalInstance()->setMaxThreadCount(1);
    connect(App,&Application::sigFilterImage,this,&ImageView::openFilterImage);
}

void ImageView::openImage(const QString &path)
{
    if(m_currentImage){
        delete m_currentImage;
        m_currentImage=nullptr;
    }
    m_currentImage=new QImage(path);

    if(!m_currentImage->isNull())
    {
        QPixmap pic=QPixmap::fromImage(*m_currentImage);
        App->setStackWidget(1);
        scene()->clear();
        m_pixmapItem=new QGraphicsPixmapItem(pic);
        m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);
        QRectF rect = m_pixmapItem->boundingRect();
        setSceneRect(rect);
        scene()->addItem(m_pixmapItem);
        fitWindow();
        m_currentPath=path;
    }
    else {
//       App->setStackWidget(0);
    }
}

void ImageView::openFilterImage(QImage *img)
{
    if(!img->isNull())
    {
        QPixmap pic=QPixmap::fromImage(*img);
        if(!pic.isNull())
        {
            App->setStackWidget(1);
            scene()->clear();
            m_pixmapItem=new QGraphicsPixmapItem(pic);
            m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);
            QRectF rect = m_pixmapItem->boundingRect();
            setSceneRect(rect);
            scene()->addItem(m_pixmapItem);
            fitWindow();
        }
    }
}
qreal ImageView::windowRelativeScale() const
{
    QRectF bf = sceneRect();
    if (this->window()->isFullScreen()) {
        if (1.0 * (width()) / (height() + 15) > 1.0 * bf.width() / bf.height()) {
            return 1.0 * (height() + 15) / bf.height();
        } else {
            return 1.0 * (width()) / bf.width();
        }
    } else {
        if (1.0 * (width() - 20) / (height() - 180) > 1.0 * bf.width() / bf.height()) {
            return 1.0 * (height() - 180) / bf.height();
        } else {
            return 1.0 * (width() - 20) / bf.width();
        }
    }
}
void ImageView::fitWindow()
{
    qreal wrs = windowRelativeScale();
    m_scal = wrs;
    resetTransform();
    scale(wrs, wrs);

//    if (wrs - 1 > -0.01 && wrs - 1 < 0.01) {
//        emit checkAdaptImageBtn();
//    } else {
//        emit disCheckAdaptImageBtn();
//    }
    m_isFitImage = false;
    m_isFitWindow = true;

}

void ImageView::fitImage()
{
    resetTransform();
    m_scal = 1.0;
    scale(1, 1);
    m_isFitImage = true;
    m_isFitWindow = false;
}

void ImageView::RotateImage(const int &index)
{
    if(!m_pixmapItem) return;
    QPixmap pixmap = m_pixmapItem->pixmap();
    QMatrix rotate;
    rotate.rotate(index);

    pixmap = pixmap.transformed(rotate, Qt::FastTransformation);
    pixmap.setDevicePixelRatio(devicePixelRatioF());
    scene()->clear();
    resetTransform();
    m_pixmapItem = new QGraphicsPixmapItem(pixmap);
    m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    // Make sure item show in center of view after reload
    QRectF rect = m_pixmapItem->boundingRect();
    setSceneRect(rect);
    scene()->addItem(m_pixmapItem);

    autoFit();
    m_rotateAngel += index;
    if(!m_FilterImage)
    {
        m_FilterImage=new QImage(image());
    }else {
        delete m_FilterImage;
        m_FilterImage=nullptr;
        m_FilterImage=new QImage(image());
    }

}
void ImageView::savecurrentPic()
{
    image().save(m_currentPath);
}
void ImageView::savecurrentPicAs()
{
    QFileDialog fileDialog;
    QString fileName = fileDialog.getSaveFileName(this,tr("Open File"),"/home",tr("png"));
    if(fileName == "")
    {
        return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
        return;
    }
    else
    {
        image().save(fileName,"png");
    }
}

void ImageView::openImage(QImage *img)
{
    if(!img->isNull())
    {
        QPixmap pic=QPixmap::fromImage(*img);
        if(!pic.isNull())
        {
            App->setStackWidget(1);
            scene()->clear();
            m_pixmapItem=new QGraphicsPixmapItem(pic);
            m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);
            QRectF rect = m_pixmapItem->boundingRect();
            setSceneRect(rect);
            scene()->addItem(m_pixmapItem);
            fitWindow();
        }
    }
}

qreal ImageView::imageRelativeScale() const
{
    return transform().m11() / devicePixelRatioF();
}
void ImageView::autoFit()
{
    if (image().isNull())
        return;

    QSize image_size = image().size();

    // change some code in graphicsitem.cpp line100.

    if ((image_size.width() >= width() || image_size.height() >= height() - 150) && width() > 0 &&
            height() > 0) {
        fitWindow();
    } else {
        fitImage();
    }

}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    App->sigMouseMove();
    return QGraphicsView::mouseMoveEvent(event);
}

void ImageView::oldIMage()
{
    if(m_currentImage){
        if(!m_FilterImage){
            m_FilterImage=new QImage(image());
        }
        ImageFilterInfo info;
        info.id=MenuItemId::Idold;
        ImageRunnable* imgThread=new ImageRunnable();
        imgThread->setData(m_FilterImage,info);
        QThreadPool::globalInstance()->start(imgThread);
    }
}

void ImageView::resetImage()
{
    openImage(m_currentImage);
    if(m_FilterImage){
        delete m_FilterImage;
        m_FilterImage=nullptr;
    }
}

void ImageView::BEEPImage(double spatialDecay, double photometricStandardDeviation)
{
    if(m_currentImage){
        if(!m_FilterImage){
            m_FilterImage=new QImage(image());
        }
        ImageFilterInfo info;
        info.id=MenuItemId::IdBEEP;
        info.spatialDecay=spatialDecay;
        info.photometricStandardDeviation=photometricStandardDeviation;
        ImageRunnable* imgThread=new ImageRunnable();
        imgThread->setData(m_FilterImage,info);
        QThreadPool::globalInstance()->start(imgThread);
    }
}

void ImageView::warnImage(int index)
{
    if(m_currentImage){
        if(!m_FilterImage){
            m_FilterImage=new QImage(image());
        }
        ImageFilterInfo info;
        info.id=MenuItemId::IdWarn;
        info.warnImageDecay=index;
        ImageRunnable* imgThread=new ImageRunnable();
        imgThread->setData(m_FilterImage,info);
        QThreadPool::globalInstance()->start(imgThread);
    }
}

void ImageView::coolImage(int index)
{
    if(m_currentImage){
        if(!m_FilterImage){
            m_FilterImage=new QImage(image());
        }
        ImageFilterInfo info;
        info.id=MenuItemId::IdCool;
        info.coolImageDecay=index;
        ImageRunnable* imgThread=new ImageRunnable();
        imgThread->setData(m_FilterImage,info);
        QThreadPool::globalInstance()->start(imgThread);
    }
}

void ImageView::GrayScaleImage()
{
    if(m_currentImage){
        if(!m_FilterImage){
            m_FilterImage=new QImage(image());
        }
        ImageFilterInfo info;
        info.id=MenuItemId::IdGrayScale;
        ImageRunnable* imgThread=new ImageRunnable();
        imgThread->setData(m_FilterImage,info);
        QThreadPool::globalInstance()->start(imgThread);
    }
}

void ImageView::lightContrastImage(int light, int Contrast)
{
    if(m_currentImage){
        if(!m_FilterImage){
            m_FilterImage=new QImage(image());
        }
        ImageFilterInfo info;
        info.id=MenuItemId::IdlightContrast;
        info.lightDecay=light;
        info.ContrastDecay=Contrast;
        ImageRunnable* imgThread=new ImageRunnable();
        imgThread->setData(m_FilterImage,info);
        QThreadPool::globalInstance()->start(imgThread);
    }
}

void ImageView::InverseColorImage()
{
    if(m_currentImage){
        if(!m_FilterImage){
            m_FilterImage=new QImage(image());
        }
        ImageFilterInfo info;
        info.id=MenuItemId::IdInverseColor;
        ImageRunnable* imgThread=new ImageRunnable();
        imgThread->setData(m_FilterImage,info);
        QThreadPool::globalInstance()->start(imgThread);
    }
}
const QImage ImageView::image()
{
    /*lmh0807,解决崩溃的问题*/
    if (m_pixmapItem) {
        return m_pixmapItem->pixmap().toImage();
    }
    else {
        return QImage();
    }
}
void ImageView::resizeEvent(QResizeEvent *event)
{

    return QGraphicsView::resizeEvent(event);
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    qreal factor = qPow(1.2, event->delta() / 240.0);
    scaleAtPoint(event->pos(), factor);

    event->accept();
}
void ImageView::scaleAtPoint(QPoint pos, qreal factor)
{
    // Remember zoom anchor point.
    const QPointF targetPos = pos;
    const QPointF targetScenePos = mapToScene(targetPos.toPoint());

    // Do the scaling.
    setScaleValue(factor);
    // Restore the zoom anchor point.
    //
    // The Basic idea here is we don't care how the scene is scaled or transformed,
    // we just want to restore the anchor point to the target position we've
    // remembered, in the coordinate of the view/viewport.
    const QPointF curPos = mapFromScene(targetScenePos);
    const QPointF centerPos = QPointF(width() / 2.0, height() / 2.0) + (curPos - targetPos);
    const QPointF centerScenePos = mapToScene(centerPos.toPoint());
    centerOn(static_cast<int>(centerScenePos.x()), static_cast<int>(centerScenePos.y()));
}
void ImageView::setScaleValue(qreal v)
{
    //由于矩阵被旋转，通过矩阵获取缩放因子，计算缩放比例错误，因此记录过程中的缩放因子来判断缩放比例
    m_scal *=v;
    qDebug() << m_scal;
    scale(v, v);
    //const qreal irs = imageRelativeScale() * devicePixelRatioF();
    // Rollback
    if (v < 1 && /*irs <= MIN_SCALE_FACTOR)*/m_scal<0.03) {
        const qreal minv = MIN_SCALE_FACTOR / m_scal;
        // if (minv < 1.09) return;
        scale(minv, minv);
        m_scal *=minv;
    } else if (v > 1 && /*irs >= MAX_SCALE_FACTOR*/m_scal>20) {
        const qreal maxv = MAX_SCALE_FACTOR / m_scal;
        scale(maxv, maxv);
        m_scal *=maxv;
    } else {
        m_isFitImage = false;
        m_isFitWindow = false;
    }

    qreal rescale = imageRelativeScale() * devicePixelRatioF();
//    if (rescale - 1 > -0.01 && rescale - 1 < 0.01) {
//        emit checkAdaptImageBtn();
//    } else {
//        emit disCheckAdaptImageBtn();
//    }


}
