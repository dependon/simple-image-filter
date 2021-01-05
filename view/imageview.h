/*
 *图像显示界面
*/
#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include "application.h"

#include <QGraphicsView>

class QGraphicsPixmapItem;
class ImageCropperDemo;
//Normal表示正常模式，Basic模式不会接受全局信号
enum ViewId {
    Normal,
    Basic,
};

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    ImageView(QWidget *parent = nullptr ,ViewId id=Normal);
    //通过路径打开图片
    void openImage(const QString& path);
//    //显示该img
//    void openFilterImage(QImage *img);

    //用于鼠标滚轮滑动
    qreal windowRelativeScale() const;
    qreal imageRelativeScale() const;
    void scaleAtPoint(QPoint pos, qreal factor);
    void setScaleValue(qreal v);
    //自适应窗口
    void autoFit();
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event) override;

    //返回当前图片img
    const QImage image();
    //设置view模式
    void setViewId(ViewId id);

public slots:
    //适应窗口大小
    void fitWindow();
    //适应图片大小
    void fitImage();
    //旋转图片，感觉index角度，-为左，+为右
    void RotateImage(const int &index);
    //保存图片
    void savecurrentPic();
    //裁剪图片
    void scaleImage();
    //另存为
    void savecurrentPicAs();
    //打开该图片
    void openImage(QImage* img);
    void openFilterImage(QImage img,isChange is);
    //老照片滤镜
    void oldIMage();
    //重置图片
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
    //窗口大小改变事件
    void resizeEvent(QResizeEvent *event) override;
    //鼠标滚轮事件
    void wheelEvent(QWheelEvent *event) override;

private:
    QString m_currentPath;//当前图片路径
    QGraphicsPixmapItem *m_pixmapItem{nullptr};//当前图像的item
    bool m_isFitImage = false;//是否适应图片
    bool m_isFitWindow = false;//是否适应窗口
    qreal m_scal = 1.0;
    int   m_rotateAngel=0;//旋转角度
    QImage *m_currentImage{nullptr};//当前原始图像
    QImage m_FilterImage{nullptr};//当前处理的图像
    QImage m_lightContrastImage{nullptr};//亮度曝光度图像
    ViewId m_cureentId{Normal};//当前模式
};

#endif // IMAGEVIEW_H
