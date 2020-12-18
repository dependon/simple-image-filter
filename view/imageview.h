#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include "application.h"

#include <QGraphicsView>

class QGraphicsPixmapItem;

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    ImageView(QWidget *parent = nullptr);
    void openImage(const QString& path);
    void openFilterImage(QImage *img);

    qreal windowRelativeScale() const;
    qreal imageRelativeScale() const;
    void scaleAtPoint(QPoint pos, qreal factor);
    void setScaleValue(qreal v);
    void autoFit();
    void mouseMoveEvent(QMouseEvent *event) override;

    const QImage image();

public slots:
    void fitWindow();
    void fitImage();
    void RotateImage(const int &index);
    void savecurrentPic();
    void savecurrentPicAs();
    void openImage(QImage* img);
    void oldIMage();
    void resetImage();
    //qimage磨皮
    void BEEPImage(double spatialDecay=0.02,double photometricStandardDeviation=10);
    //暖色滤镜
    void warnImage(int index=30);
    //冷色滤镜
    void coolImage(int index=30);
    //灰度滤镜
    void GrayScaleImage();
    //亮度和饱和度
    void lightContrastImage(int light=100,int Contrast=150);
    //反色滤镜
    void InverseColorImage();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QString m_currentPath;
    QGraphicsPixmapItem *m_pixmapItem{nullptr};
    bool m_isFitImage = false;
    bool m_isFitWindow = false;
    qreal m_scal = 1.0;
    int   m_rotateAngel=0;
    QImage *m_currentImage{nullptr};
    QImage *m_FilterImage{nullptr};
};

#endif // IMAGEVIEW_H
