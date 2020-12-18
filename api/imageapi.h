#ifndef IIMAGEAPI_H
#define IIMAGEAPI_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <QVector>
#include <QImage>

class QImageAPI {

public:

    QImageAPI();
    //边界值
    static int Bound(int range_left,int data,int range_right);
    //qimage磨皮
    static void QImageD_RunBEEPSHorizontalVertical(QImage *img,QImage *imgCopy,double spatialDecay=0.02,double photometricStandardDeviation=10);
    //暖色滤镜
    static void warnImage(QImage *img,QImage *imgCopy,int index=30);
    //冷色滤镜
    static void coolImage(QImage *img,QImage *imgCopy,int index=30);
    //灰度滤镜
    static void GrayScaleImage(QImage *img,QImage *imgCopy);
    //亮度和饱和度
    static void lightContrastImage(QImage *img,QImage *imgCopy,int light=100,int Contrast=150);
    //反色滤镜
    static void InverseColorImage(QImage *img,QImage *imgCopy);
    //老照片滤镜
    static void oldImage(QImage *img,QImage *imgCopy);
};


#endif // IIMAGEAPI_H
