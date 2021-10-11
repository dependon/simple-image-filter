/*
 * Copyright (C) 2020 ~ 2021 LiuMingHang.
 *
 * Author:     LiuMingHang <liuminghang0821@gmail.com>
 *
 * Maintainer: LiuMingHang <liuminghang0821@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
    //Take RGB maximum
    static int RgbMax(int red, int green, int blue);
    //Take RGB minimum
    static int RgbMin(int red, int green, int blue);
    //Boundary judgment
    static int Bound(int range_left, int data, int range_right);
    //qimage Skin grinding
//    static void QImageD_RunBEEPSHorizontalVertical(QImage *img, QImage *imgCopy, double spatialDecay = 0.02, double photometricStandardDeviation = 10);
    static QImage QImageD_RunBEEPSHorizontalVertical(const QImage &img, double spatialDecay, double photometricStandardDeviation);
    //Warm color filter
    static QImage warnImage(const QImage &img, int index = 30);
    //Cool color filter
    static QImage coolImage(const QImage &img,  int index = 30);
    //Grayscale filter
    static QImage GrayScaleImage(const QImage &img);
    //Brightness and saturation
    static QImage lightContrastImage(const QImage &img, int light = 100, int Contrast = 150);
    //Anti color filter
    static QImage InverseColorImage(const QImage &img);
    //Old photo filter
    static QImage oldImage(const QImage &img);

    //laplacian sharpening
    static QImage LaplaceSharpen(const QImage &origin);

    //Sobel Edge Detector
    static QImage SobelEdge(const QImage &origin);

    //Greyscale
    static QImage GreyScale(QImage origin);

    //Contour acquisition
    static QImage ContourExtraction(const QImage &origin);

    //Flip horizontally
    static QImage Horizontal(const QImage &origin);

    //Flip vertical
    static QImage Vertical(const QImage &origin);

    //Binarization
    static QImage Binaryzation(const QImage &origin);

    //Metal wire drawing effect
    static QImage Metal(QImage origin);

    //Adjust image brightness
    static QImage Brightness(int delta, QImage origin);

    //Transparency
    static QImage transparencyImg(int delta, QImage origin);

    //Saturation (- 100 - 100)
    static QImage StaurationImg(const QImage &origin, int saturation);
};


#endif // IIMAGEAPI_H
