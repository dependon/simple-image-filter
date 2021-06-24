#ifndef IIMAGEAPI_H
#define IIMAGEAPI_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <QVector>
#include <QImage>
#include <QColor>

class QImageAPI
{

public:

    QImageAPI();
    //边界值
    static int Bound(int range_left, int data, int range_right);
    //qimage磨皮
//    static void QImageD_RunBEEPSHorizontalVertical(QImage *img, QImage *imgCopy, double spatialDecay = 0.02, double photometricStandardDeviation = 10);
    static QImage QImageD_RunBEEPSHorizontalVertical(const QImage &img, double spatialDecay, double photometricStandardDeviation);
    //暖色滤镜
    static QImage warnImage(const QImage &img, int index = 30);
    //冷色滤镜
    static QImage coolImage(const QImage &img,  int index = 30);
    //灰度滤镜
    static QImage GrayScaleImage(const QImage &img);
    //亮度和饱和度
    static QImage lightContrastImage(const QImage &img, int light = 100, int Contrast = 150);
    //反色滤镜
    static QImage InverseColorImage(const QImage &img);
    //老照片滤镜
    static QImage oldImage(const QImage &img);

    //拉普拉斯锐化
    static QImage LaplaceSharpen(const QImage &origin);

    //Sobel Edge Detector
    static QImage SobelEdge(const QImage &origin);

    //Greyscale
    static QImage GreyScale(QImage origin);

    //轮廓获取
    static QImage ContourExtraction(const QImage &origin);

    //水平翻转
    static QImage Horizontal(const QImage &origin);

    //竖直翻转
    static QImage Vertical(const QImage &origin);

    //二值化
    static QImage Binaryzation(const QImage &origin);

    //金属拉丝效果
    static QImage Metal(QImage origin);

    //Adjust image brightness
    static QImage Brightness(int delta, QImage origin);

    //透明度
    static QImage transparencyImg(int delta, QImage origin);

};


#endif // IIMAGEAPI_H
