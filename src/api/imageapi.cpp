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

#include <omp.h>

#include "imageapi.h"

#include <QPainter>
#include <QDebug>
#include <QDateTime>


typedef struct  {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t L;
} RGBL;

typedef struct {
    float H;
    float S;
    float V;
} HSV;

static void RGB_TO_HSV(const RGBL *input, HSV *output) // convert RGB value to HSV value
{
    float r, g, b, minRGB, maxRGB, deltaRGB;

    r = input->R / 255.0f;
    g = input->G / 255.0f;
    b = input->B / 255.0f;
    minRGB = QImageAPI::RgbMin(r, g, b);
    maxRGB = QImageAPI::RgbMax(r, g, b);
    deltaRGB = maxRGB - minRGB;

    output->V = maxRGB;
    if (maxRGB != 0.0f)
        output->S = deltaRGB / maxRGB;
    else
        output->S = 0.0f;
    if (output->S <= 0.0f) {
        output->H = 0.0f;
    } else {
        if (r == maxRGB) {
            output->H = (g - b) / deltaRGB;
        } else {
            if (g == maxRGB) {
                output->H = 2.0f + (b - r) / deltaRGB;
            } else {
                if (b == maxRGB) {
                    output->H = 4.0f + (r - g) / deltaRGB;
                }
            }
        }
        output->H = output->H * 60.0f;
        if (output->H < 0.0f) {
            output->H += 360;
        }
        output->H /= 360;
    }

}

static void HSV_TO_RGB(HSV *input, RGBL *output) //convert HSV value to RGB value
{
    float R, G, B;
    int k;
    float aa, bb, cc, f;
    if (input->S <= 0.0f)
        R = G = B = input->V;
    else {
        if (input->H == 1.0f)
            input->H = 0.0f;
        input->H *= 6.0f;
        k = (int)floor(input->H);
        f = input->H - k;
        aa = input->V * (1.0f - input->S);
        bb = input->V * (1.0f - input->S * f);
        cc = input->V * (1.0f - (input->S * (1.0f - f)));
        switch (k) {
        case 0:
            R = input->V;
            G = cc;
            B = aa;
            break;
        case 1:
            R = bb;
            G = input->V;
            B = aa;
            break;
        case 2:
            R = aa;
            G = input->V;
            B = cc;
            break;
        case 3:
            R = aa;
            G = bb;
            B = input->V;
            break;
        case 4:
            R = cc;
            G = aa;
            B = input->V;
            break;
        case 5:
            R = input->V;
            G = aa;
            B = bb;
            break;
        }
    }
    output->R = (unsigned char)(R * 255);
    output->G = (unsigned char)(G * 255);
    output->B = (unsigned char)(B * 255);
}

void adjustBrightness(RGBL &rgb_v, int step)
{
    HSV hsv_v;
    RGB_TO_HSV(&rgb_v, &hsv_v);
    rgb_v.L = hsv_v.V;
    rgb_v.L += step;
    if (rgb_v.L <= 0) {
        rgb_v.L = 1;
    } else if (rgb_v.L >= 100) {
        rgb_v.L = 100;
    }

    hsv_v.V = rgb_v.L / 100.0;
    HSV_TO_RGB(&hsv_v, &rgb_v);
}


QImageAPI::QImageAPI()
{

}

int QImageAPI::RgbMax(int red, int green, int blue)
{
    int ret = 0;
    if (red >= green) {
        ret = red;
    } else {
        ret = green;
    }
    if (ret < blue) {
        ret = blue;
    }
    return ret;
}

int QImageAPI::RgbMin(int red, int green, int blue)
{
    int ret = 0;
    if (red >= green) {
        ret = green;
    } else {
        ret = red;
    }
    if (ret > blue) {
        ret = blue;
    }
    return ret;
}

int QImageAPI::Bound(int range_left, int data, int range_right)
{
    int index = data;
    if (data > range_right) {
        index = range_right;
    } else if (data < range_left) {
        index = range_left;
    }
    return index;
}

QImage QImageAPI::QImageD_RunBEEPSHorizontalVertical(const QImage &img, double spatialDecay, double photometricStandardDeviation)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    QImage imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);

    double c = -0.5 / (photometricStandardDeviation * photometricStandardDeviation);
    double mu = spatialDecay / (2 - spatialDecay);

    double *exptable = new double[256];
    double *g_table = new double[256];
    #pragma omp parallel for
    for (int i = 0; i <= 255; i++) {
        exptable[i] = (1 - spatialDecay) * exp(c * i * i);
        g_table[i] = mu * i;
    }
    int width = img.width();
    int height = img.height();
    int length = width * height;
    double *data2Red = new double[length];
    double *data2Green = new double[length];
    double *data2Blue = new double[length];


    int size = imgCopy.width() * imgCopy.height();
    uint8_t *rgb = imgCopy.bits();
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        data2Red[i] = rgb[i * 3];
        data2Green[i] = rgb[i * 3 + 1];
        data2Blue[i] = rgb[i * 3 + 2];
    }


    double *gRed = new double[length];
    double *pRed = new double[length];
    double *rRed = new double[length];

    double *gGreen = new double[length];
    double *pGreen = new double[length];
    double *rGreen = new double[length];

    double *gBlue = new double[length];
    double *pBlue = new double[length];
    double *rBlue = new double[length];
    memcpy(pRed, data2Red, sizeof(double) * length);
    memcpy(rRed, data2Red, sizeof(double) * length);

    memcpy(pGreen, data2Green, sizeof(double) * length);
    memcpy(rGreen, data2Green, sizeof(double) * length);

    memcpy(pBlue, data2Blue, sizeof(double) * length);
    memcpy(rBlue, data2Blue, sizeof(double) * length);


    double rho0 = 1.0 / (2 - spatialDecay);
    #pragma omp parallel for
    for (int k2 = 0; k2 < height; ++k2) {
        int startIndex = k2 * width;
        double mu = 0.0;

        for (int k = startIndex + 1, K = startIndex + width; k < K; ++k) {
            int div0Red = fabs(pRed[k] - pRed[k - 1]);
            mu = exptable[div0Red];
            pRed[k] = pRed[k - 1] * mu + pRed[k] * (1.0 - mu);

            int div0Green = fabs(pGreen[k] - pGreen[k - 1]);
            mu = exptable[div0Green];
            pGreen[k] = pGreen[k - 1] * mu + pGreen[k] * (1.0 - mu);

            int div0Blue = fabs(pBlue[k] - pBlue[k - 1]);
            mu = exptable[div0Blue];
            pBlue[k] = pBlue[k - 1] * mu + pBlue[k] * (1.0 - mu);

        }

        for (int k = startIndex + width - 2; startIndex <= k; --k) {
            int div0Red = fabs(rRed[k] - rRed[k + 1]);
            double mu = exptable[div0Red];
            rRed[k] = rRed[k + 1] * mu + rRed[k] * (1.0 - mu);

            int div0Green = fabs(rGreen[k] - rGreen[k + 1]);
            mu = exptable[div0Green];
            rGreen[k] = rGreen[k + 1] * mu + rGreen[k] * (1.0 - mu);

            int div0Blue = fabs(rBlue[k] - rBlue[k + 1]);
            mu = exptable[div0Blue];
            rBlue[k] = rBlue[k + 1] * mu + rBlue[k] * (1.0 - mu);
        }

        for (int k = startIndex, K = startIndex + width; k < K; k++) {
            rRed[k] = (rRed[k] + pRed[k]) * rho0 - g_table[(int)data2Red[k]];
            rGreen[k] = (rGreen[k] + pGreen[k]) * rho0 - g_table[(int)data2Green[k]];
            rBlue[k] = (rBlue[k] + pBlue[k]) * rho0 - g_table[(int)data2Blue[k]];
        }
    }

    int m = 0;

    for (int k2 = 0; k2 < height; k2++) {
        int n = k2;

        for (int k1 = 0; k1 < width; k1++) {
            gRed[n] = rRed[m];
            gGreen[n] = rGreen[m];
            gBlue[n] = rBlue[m];
            m++;
            n += height;
        }
    }

    memcpy(pRed, gRed, sizeof(double) * height * width);
    memcpy(rRed, gRed, sizeof(double) * height * width);

    memcpy(pGreen, gGreen, sizeof(double) * height * width);
    memcpy(rGreen, gGreen, sizeof(double) * height * width);

    memcpy(pBlue, gBlue, sizeof(double) * height * width);
    memcpy(rBlue, gBlue, sizeof(double) * height * width);

    #pragma omp parallel for
    for (int k1 = 0; k1 < width; ++k1) {
        int startIndex = k1 * height;
        double mu = 0.0;

        for (int k = startIndex + 1, K = startIndex + height; k < K; ++k) {
            int div0Red = fabs(pRed[k] - pRed[k - 1]);
            mu = exptable[div0Red];
            pRed[k] = pRed[k - 1] * mu + pRed[k] * (1.0 - mu);

            int div0Green = fabs(pGreen[k] - pGreen[k - 1]);
            mu = exptable[div0Green];
            pGreen[k] = pGreen[k - 1] * mu + pGreen[k] * (1.0 - mu);

            int div0Blue = fabs(pBlue[k] - pBlue[k - 1]);
            mu = exptable[div0Blue];
            pBlue[k] = pBlue[k - 1] * mu + pBlue[k] * (1.0 - mu);
        }

        for (int k = startIndex + height - 2; startIndex <= k; --k) {
            int div0Red = fabs(rRed[k] - rRed[k + 1]);
            mu = exptable[div0Red];
            rRed[k] = rRed[k + 1] * mu + rRed[k] * (1.0 - mu);

            int div0Green = fabs(rGreen[k] - rGreen[k + 1]);
            mu = exptable[div0Green];
            rGreen[k] = rGreen[k + 1] * mu + rGreen[k] * (1.0 - mu);

            int div0Blue = fabs(rBlue[k] - rBlue[k + 1]);
            mu = exptable[div0Blue];
            rBlue[k] = rBlue[k + 1] * mu + rBlue[k] * (1.0 - mu);
        }
    }

    double init_gain_mu = spatialDecay / (2 - spatialDecay);

    #pragma omp parallel for
    for (int k = 0; k < length; ++k) {
        rRed[k] = (rRed[k] + pRed[k]) * rho0 - gRed[k] * init_gain_mu;

        rGreen[k] = (rGreen[k] + pGreen[k]) * rho0 - gGreen[k] * init_gain_mu;

        rBlue[k] = (rBlue[k] + pBlue[k]) * rho0 - gBlue[k] * init_gain_mu;

    }

    m = 0;
    int nRowBytes = (width * 24 + 31) / 32 * 4;
    int  lineNum_24 = 0;

    for (int k1 = 0; k1 < width; ++k1) {
        int n = k1;
        for (int k2 = 0; k2 < height; ++k2) {
            data2Red[n] = rRed[m];
            data2Green[n] = rGreen[m];
            data2Blue[n] = rBlue[m];
            lineNum_24 = k2 * nRowBytes;
            rgb[lineNum_24 + k1 * 3] = data2Red[n];
            rgb[lineNum_24 + k1 * 3 + 1] = data2Green[n];
            rgb[lineNum_24 + k1 * 3 + 2] = data2Blue[n];
            m++;
            n += width;
        }
    }
    delete []data2Red;
    data2Red = nullptr;
    delete []data2Green ;
    data2Green = nullptr;
    delete []data2Blue;
    data2Blue = nullptr;

    delete []pRed;
    pRed = nullptr;
    delete []rRed;
    rRed = nullptr;
    delete []gRed;
    gRed = nullptr;

    delete []pGreen;
    pGreen = nullptr;
    delete []rGreen;
    rGreen = nullptr;
    delete []gGreen;
    gGreen = nullptr;

    delete []pBlue;
    pBlue = nullptr;
    delete []rBlue;
    rBlue = nullptr;
    delete []gBlue;
    gBlue = nullptr;

    delete []exptable;
    exptable = nullptr;
    delete []g_table;
    g_table = nullptr;

    qDebug() << "磨皮结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}

QImage QImageAPI::warnImage(const QImage &img, int index)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QImage imgCopy;
    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();
    if (nullptr == rgb) {
        return QImage();
    }
    QColor frontColor;
    int size = img.width() * img.height();
    #pragma omp parallel for
    for (int i = 0; i < size ; i++) {
        int r = rgb[i * 3] + index;
        int g = rgb[i * 3 + 1] + index;
        int b = rgb[i * 3 + 2] ;

        rgb[i * 3] = r > 255 ? 255 : r;
        rgb[i * 3 + 1] = g > 255 ? 255 : g;
        rgb[i * 3 + 2] = b > 255 ? 255 : b;
    }
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}

QImage QImageAPI::coolImage(const QImage &img,  int index)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QImage imgCopy;
    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();
    if (nullptr == rgb) {
        return QImage();
    }
    QColor frontColor;
    int size = img.width() * img.height();
    #pragma omp parallel for
    for (int i = 0; i < size ; i++) {
        int r = rgb[i * 3] ;
        int g = rgb[i * 3 + 1] ;
        int b = rgb[i * 3 + 2] + index;

        rgb[i * 3] = r > 255 ? 255 : r;
        rgb[i * 3 + 1] = g > 255 ? 255 : g;
        rgb[i * 3 + 2] = b > 255 ? 255 : b;
    }

    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}

QImage QImageAPI::GrayScaleImage(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QImage imgCopy;
    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();
    if (nullptr == rgb) {
        return QImage();
    }
    QColor frontColor;
    int size = img.width() * img.height();

    #pragma omp parallel for
    for (int i = 0; i < size ; i++) {
        int average = (rgb[i * 3] + rgb[i * 3 + 1] + rgb[i * 3 + 2]) / 3;
        rgb[i * 3] = average > 255 ? 255 : average;
        rgb[i * 3 + 1] = average > 255 ? 255 : average;
        rgb[i * 3 + 2] = average > 255 ? 255 : average;
    }
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}

QImage QImageAPI::lightContrastImage(const QImage &img,  int light, int Contrast)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QImage imgCopy;
    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();
    if (nullptr == rgb) {
        return QImage();
    }

    int size = img.width() * img.height();
    #pragma omp parallel for
    for (int i = 0; i < size ; i++) {
        int r;
        int g;
        int b;
        r = light * 0.01 * rgb[i * 3] - 150 + Contrast;
        g = light * 0.01 * rgb[i * 3 + 1] - 150 + Contrast;
        b = light * 0.01 * rgb[i * 3 + 2]  - 150 + Contrast;

        rgb[i * 3] = Bound(0, r, 255);
        rgb[i * 3 + 1] = Bound(0, g, 255);
        rgb[i * 3 + 2] = Bound(0, b, 255);

    }

    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}

QImage QImageAPI::InverseColorImage(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QImage imgCopy;
    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();
    if (nullptr == rgb) {
        return QImage();
    } int size = img.width() * img.height();
    #pragma omp parallel for
    for (int i = 0; i < size ; i++) {
        rgb[i * 3] = 255 - rgb[i * 3] ;
        rgb[i * 3 + 1] = 255 - rgb[i * 3 + 1]  ;
        rgb[i * 3 + 2] = 255 - rgb[i * 3 + 2]  ;
    }
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}

QImage QImageAPI::oldImage(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QImage imgCopy;
    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();
    if (nullptr == rgb) {
        return QImage();
    } int size = img.width() * img.height();
    #pragma omp parallel for
    for (int i = 0; i < size ; i++) {
        float r = 0.393 * rgb[i * 3] + 0.769 * rgb[i * 3 + 1] + 0.189 * rgb[i * 3 + 2];
        float g = 0.349 * rgb[i * 3] + 0.686 * rgb[i * 3 + 1] + 0.168 * rgb[i * 3 + 2];
        float b = 0.272 * rgb[i * 3] + 0.534 * rgb[i * 3 + 1] + 0.131 * rgb[i * 3 + 2];
        r = Bound(0, r, 255);
        g = Bound(0, g, 255);
        b = Bound(0, b, 255);
        rgb[i * 3] = r;
        rgb[i * 3 + 1] = g ;
        rgb[i * 3 + 2] = b  ;
    }
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}


QImage QImageAPI::LaplaceSharpen(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QImage imgCopy;
    int width = img.width();
    int height = img.height();
    int window[3][3] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(width, height, QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    QImage imgCopyrgbImg = QImage(img).convertToFormat(QImage::Format_RGB888);
    uint8_t *rgbImg = imgCopyrgbImg.bits();
    uint8_t *rgb = imgCopy.bits();

    int nRowBytes = (width * 24 + 31) / 32 * 4;

    #pragma omp parallel for
    for (int x = 1; x < img.width(); x++) {
        for (int y = 1; y < img.height(); y++) {
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;
            int lineNum_24 = 0;

            for (int m = x - 1; m <= x + 1; m++)
                for (int n = y - 1; n <= y + 1; n++) {
                    if (m >= 0 && m < width && n < height) {
                        lineNum_24 = n * nRowBytes;
                        sumR += rgbImg[lineNum_24 + m * 3] * window[n - y + 1][m - x + 1];
                        sumG += rgbImg[lineNum_24 + m * 3 + 1] * window[n - y + 1][m - x + 1];
                        sumB += rgbImg[lineNum_24 + m * 3 + 2] * window[n - y + 1][m - x + 1];
                    }
                }


            int old_r = rgbImg[lineNum_24 + x * 3];
            sumR += old_r;
            sumR = qBound(0, sumR, 255);

            int old_g = rgbImg[lineNum_24 + x * 3 + 1];
            sumG += old_g;
            sumG = qBound(0, sumG, 255);

            int old_b = rgbImg[lineNum_24 + x * 3 + 2];
            sumB += old_b;
            sumB = qBound(0, sumB, 255);
            lineNum_24 = y * nRowBytes;
            rgb[lineNum_24 + x * 3] = sumR;
            rgb[lineNum_24 + x * 3 + 1] = sumG;
            rgb[lineNum_24 + x * 3 + 2] = sumB;
        }
    }

    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}

QImage QImageAPI::SobelEdge(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    double *Gx = new double[9];
    double *Gy = new double[9];

    /* Sobel */
    Gx[0] = 1.0; Gx[1] = 0.0; Gx[2] = -1.0;
    Gx[3] = 2.0; Gx[4] = 0.0; Gx[5] = -2.0;
    Gx[6] = 1.0; Gx[7] = 0.0; Gx[8] = -1.0;

    Gy[0] = -1.0; Gy[1] = -2.0; Gy[2] = - 1.0;
    Gy[3] = 0.0; Gy[4] = 0.0; Gy[5] = 0.0;
    Gy[6] = 1.0; Gy[7] = 2.0; Gy[8] = 1.0;

    QRgb pixel;
    QImage grayImage = GreyScale(img);
    int height = grayImage.height();
    int width = grayImage.width();
    QImage imgCopy = QImage(width, height, QImage::Format_RGB888);

    uint8_t *rgbImg = grayImage.bits();
    uint8_t *rgb = imgCopy.bits();

    int nRowBytes = (width * 24 + 31) / 32 * 4;

    float *sobel_norm = new float[width * height];
    float max = 0.0;
    QColor my_color;


    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double value_gx = 0.0;
            double value_gy = 0.0;

            for (int k = 0; k < 3; k++) {
                for (int p = 0; p < 3; p++) {
                    if ((x + 1 + 1 - k < width) && (y + 1 + 1 - p < height)) {
                        pixel = grayImage.pixel(x + 1 + 1 - k, y + 1 + 1 - p);
                        int  lineNum_24 = (y + 1 + 1 - p) * nRowBytes;
                        value_gx += Gx[p * 3 + k] * rgbImg[lineNum_24 + (x + 1 + 1 - k) * 3];
                        value_gy += Gy[p * 3 + k] * rgbImg[lineNum_24 + (x + 1 + 1 - k) * 3];
                    }
                }
                sobel_norm[x + y * width] = abs(value_gx) + abs(value_gy);

                max = sobel_norm[x + y * width] > max ? sobel_norm[x + y * width] : max;
            }
        }
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            my_color.setHsv(0, 0, 255 - int(255.0 * sobel_norm[i + j * width] / max));

            int lineNum_24 = j * nRowBytes;
            rgb[lineNum_24 + i * 3] = my_color.red();
            rgb[lineNum_24 + i * 3 + 1] = my_color.green();
            rgb[lineNum_24 + i * 3 + 2] = my_color.blue();
        }
    }
    delete[] sobel_norm;

    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}


QImage QImageAPI::GreyScale(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QImage imgCopy;

    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();
    int size = img.width() * img.height();
    #pragma omp parallel for
    for (int i = 0; i < size ; i++) {
        int average = (rgb[i * 3] * 299 + rgb[i * 3 + 1] * 587 + rgb[i * 3 + 1] * 114 + 500) / 1000;
        rgb[i * 3] = average;
        rgb[i * 3 + 1] = average;
        rgb[i * 3 + 2] = average;
    }
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;

}


QImage QImageAPI::Binaryzation(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    QImage imgCopy;

    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();

    int size = img.width() * img.height();
    #pragma omp parallel for
    for (int i = 0; i < size ; i++) {
        int  gray = (rgb[i * 3] + rgb[i * 3 + 1] + rgb[i * 3 + 2]) / 3;
        int newGray = 0;
        if (gray > 128)
            newGray = 255;
        else
            newGray = 0;
        rgb[i * 3] = newGray;
        rgb[i * 3 + 1] = newGray;
        rgb[i * 3 + 2] = newGray;
    }
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;

    return imgCopy;
}


QImage QImageAPI::ContourExtraction(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    int width = img.width();
    int height = img.height();

    QImage binImg = Binaryzation(img);
    QImage newImg = QImage(width, height, QImage::Format_RGB888);
    newImg.fill(Qt::white);

    uint8_t *rgb = newImg.bits();
    uint8_t *binrgb = binImg.bits();
    int nRowBytes = (width * 24 + 31) / 32 * 4;
    #pragma omp parallel for
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int pixel[8];
            memset(pixel, 0, 8);
            int lineNum_24 = y * nRowBytes;
            if (binrgb[lineNum_24 + x * 3] == 0) {
                rgb[lineNum_24 + x * 3] = 0;
                rgb[lineNum_24 + x * 3 + 1] = 0;
                rgb[lineNum_24 + x * 3 + 2] = 0;
                pixel[0] = binrgb[(y - 1) * nRowBytes + (x - 1) * 3];
                pixel[1] = binrgb[(y) * nRowBytes + (x - 1) * 3];
                pixel[2] = binrgb[(y + 1) * nRowBytes + (x - 1) * 3];
                pixel[3] = binrgb[(y - 1) * nRowBytes + (x) * 3];
                pixel[4] = binrgb[(y + 1) * nRowBytes + (x) * 3];
                pixel[5] = binrgb[(y - 1) * nRowBytes + (x + 1) * 3];
                pixel[6] = binrgb[(y) * nRowBytes + (x + 1) * 3];
                pixel[7] = binrgb[(y + 1) * nRowBytes + (x + 1) * 3];

                if (pixel[0] + pixel[1] + pixel[2] + pixel[3] + pixel[4] + pixel[5] + pixel[6] + pixel[7] == 0) {
                    rgb[lineNum_24 + x * 3] = 255;
                    rgb[lineNum_24 + x * 3 + 1] = 255;
                    rgb[lineNum_24 + x * 3 + 2] = 255;
                }

            }
        }
    }
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return newImg;
}


/*****************************************************************************
 *                                   Flip
 * **************************************************************************/
QImage QImageAPI::Horizontal(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    QImage copyImage(QSize(img.width(), img.height()), QImage::Format_ARGB32);
    copyImage = img.mirrored(true, false);
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return copyImage;

}


QImage QImageAPI::Metal(const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    QImage *baseImage = new QImage(img);
    QImage darkImage = QImageAPI::Brightness(-100, img);
    QImage greyImage = QImageAPI::GreyScale(darkImage);
    QPainter painter;

    QImage newImage = baseImage->scaled(QSize(img.width(), img.height()));

    painter.begin(&newImage);
    painter.setOpacity(0.5);
    painter.drawImage(0, 0, greyImage);
    painter.end();
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return newImage;
}


QImage QImageAPI::Brightness(int delta, const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();



    QImage imgCopy;

    if (img.format() != QImage::Format_RGB888) {
        imgCopy = QImage(img).convertToFormat(QImage::Format_RGB888);
    } else {
        imgCopy = QImage(img);
    }
    uint8_t *rgb = imgCopy.bits();
    int size = img.width() * img.height();
    #pragma omp parallel for
    for (int i = 0; i < size ; i++) {
        int r = rgb[i * 3] + delta;
        int g = rgb[i * 3 + 1] + delta;
        int b = rgb[i * 3 + 2] + delta;
        r = qBound(0, r, 255);
        g = qBound(0, g, 255);
        b = qBound(0, b, 255);
        rgb[i * 3] = r ;
        rgb[i * 3 + 1] =  g ;
        rgb[i * 3 + 2] =  b ;
    }

    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return imgCopy;
}

QImage QImageAPI::transparencyImg(int delta, const QImage &img)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    QImage newImage(img.width(), img.height(),
                    QImage::Format_ARGB32);
    QColor oldColor;
    int r, g, b;
    for (int x = 0; x < newImage.width(); x++) {
        for (int y = 0; y < newImage.height(); y++) {
            oldColor = QColor(img.pixel(x, y));

            r = oldColor.red() ;
            g = oldColor.green() ;
            b = oldColor.blue() ;

            newImage.setPixel(x, y, qRgba(r, g, b, delta));
        }
    }
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return newImage;
}

QImage QImageAPI::StaurationImg(const QImage &origin, int saturation)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    int r, g, b, rgbMin, rgbMax;
    float k = saturation / 100.0f * 128;
    int alpha = 0;

    QImage newImage(origin);
    QColor tmpColor;

    for (int x = 0; x < newImage.width(); x++) {
        for (int y = 0; y < newImage.height(); y++) {
            tmpColor = QColor(origin.pixel(x, y));
            r = tmpColor.red();
            g = tmpColor.green();
            b = tmpColor.blue();

            rgbMin = RgbMin(r, g, b);
            rgbMax = RgbMax(r, g, b);

            int delta = (rgbMax - rgbMin);
            int value = (rgbMax + rgbMin);
            if (delta == 0) {
                continue;
            }
            int L = value >> 1;
            int S = L < 128 ? (delta << 7) / value : (delta << 7) / (510 - value);
            if (k >= 0) {
                alpha = k + S >= 128 ? S : 128 - k;
                alpha = 128 * 128 / alpha - 128;
            } else
                alpha = k;
            r = r + ((r - L) * alpha >> 7);
            g = g + ((g - L) * alpha >> 7);
            b = b + ((b - L) * alpha >> 7);
            r = Bound(0, r, 255);
            g = Bound(0, g, 255);
            b = Bound(0, b, 255);
            newImage.setPixel(x, y, qRgb(r, g, b));

        }
    }
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return newImage;

}


QImage QImageAPI::Vertical(const QImage &origin)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    QImage newImage(QSize(origin.width(), origin.height()), QImage::Format_ARGB32);
    newImage = origin.mirrored(false, true);
    qDebug() << "结束:" << QDateTime::currentMSecsSinceEpoch() - startTime;
    return newImage;
}
