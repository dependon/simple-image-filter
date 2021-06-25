#include "imageapi.h"
#include <QPainter>

QImageAPI::QImageAPI()
{

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

    QImage imgCopy = QImage(img);

    double c = -0.5 / (photometricStandardDeviation * photometricStandardDeviation); //-1/2 *光度标准偏差的平方
    double mu = spatialDecay / (2 - spatialDecay);

    double *exptable = new double[256];
    double *g_table = new double[256];
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

    int i = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb rgb = imgCopy.pixel(x, y);
            data2Red[i] = qRed(rgb);
            data2Green[i] = qGreen(rgb);
            data2Blue[i] = qBlue(rgb);
            i++;
        }
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
    for (int k2 = 0; k2 < height; ++k2) {
        int startIndex = k2 * width;
        double mu = 0.0;
        for (int k = startIndex + 1, K = startIndex + width; k < K; ++k) {
            int div0Red = fabs(pRed[k] - pRed[k - 1]);
            mu = exptable[div0Red];
            pRed[k] = pRed[k - 1] * mu + pRed[k] * (1.0 - mu);//公式1

            int div0Green = fabs(pGreen[k] - pGreen[k - 1]);
            mu = exptable[div0Green];
            pGreen[k] = pGreen[k - 1] * mu + pGreen[k] * (1.0 - mu);//公式1

            int div0Blue = fabs(pBlue[k] - pBlue[k - 1]);
            mu = exptable[div0Blue];
            pBlue[k] = pBlue[k - 1] * mu + pBlue[k] * (1.0 - mu);//公式1

        }

        for (int k = startIndex + width - 2; startIndex <= k; --k) {
            int div0Red = fabs(rRed[k] - rRed[k + 1]);
            double mu = exptable[div0Red];
            rRed[k] = rRed[k + 1] * mu + rRed[k] * (1.0 - mu);//公式3

            int div0Green = fabs(rGreen[k] - rGreen[k + 1]);
            mu = exptable[div0Green];
            rGreen[k] = rGreen[k + 1] * mu + rGreen[k] * (1.0 - mu);//公式3

            int div0Blue = fabs(rBlue[k] - rBlue[k + 1]);
            mu = exptable[div0Blue];
            rBlue[k] = rBlue[k + 1] * mu + rBlue[k] * (1.0 - mu);//公式3
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
    for (int k = 0; k < length; ++k) {
        rRed[k] = (rRed[k] + pRed[k]) * rho0 - gRed[k] * init_gain_mu;

        rGreen[k] = (rGreen[k] + pGreen[k]) * rho0 - gGreen[k] * init_gain_mu;

        rBlue[k] = (rBlue[k] + pBlue[k]) * rho0 - gBlue[k] * init_gain_mu;

    }

    m = 0;
    for (int k1 = 0; k1 < width; ++k1) {
        int n = k1;
        for (int k2 = 0; k2 < height; ++k2) {

            data2Red[n] = rRed[m];
            data2Green[n] = rGreen[m];
            data2Blue[n] = rBlue[m];
            imgCopy.setPixel(k1, k2, qRgb(data2Red[n], data2Green[n], data2Blue[n]));
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


    return imgCopy;
}

QImage QImageAPI::warnImage(const QImage &img, int index)
{
    QImage imgCopy = QImage(img);

    QRgb *line;
    QColor frontColor;
    for (int y = 0; y < img.height(); y++) {
        line = (QRgb *)img.scanLine(y);
        for (int x = 0; x < img.width(); x++) {
            frontColor = line[x];
            float r = frontColor.red() + index;
            float g = frontColor.green() + index;
            float b = frontColor.blue();
            r = Bound(0, r, 255);
            g = Bound(0, g, 255);
            imgCopy.setPixel(x, y, qRgb(r, g, b));
        }

    }
    return imgCopy;
}

QImage QImageAPI::coolImage(const QImage &img,  int index)
{
    QImage imgCopy = QImage(img);

    QRgb *line;
    QColor frontColor;
    for (int y = 0; y < img.height(); y++) {
        line = (QRgb *)img.scanLine(y);
        for (int x = 0; x < img.width(); x++) {
            frontColor = line[x];
            float r = frontColor.red();
            float g = frontColor.green();
            float b = frontColor.blue() + index;
            b = Bound(0, b, 255);
            imgCopy.setPixel(x, y, qRgb(r, g, b));
        }

    }
    return imgCopy;
}

QImage QImageAPI::GrayScaleImage(const QImage &img)
{
    QImage imgCopy = QImage(img);

    QRgb *line;
    for (int y = 0; y < img.height(); y++) {
        line = (QRgb *)img.scanLine(y);
        for (int x = 0; x < img.width(); x++) {
            int average = (qRed(line[x]) + qGreen(line[x]) + qBlue(line[x])) / 3;
            imgCopy.setPixel(x, y, qRgb(average, average, average));
        }

    }
    return imgCopy;
}

QImage QImageAPI::lightContrastImage(const QImage &img,  int light, int Contrast)
{
    QImage imgCopy = QImage(img);

    QRgb *line;
    for (int y = 0; y < img.height(); y++) {
        line = (QRgb *)img.scanLine(y);
        for (int x = 0; x < img.width(); x++) {
            float r = light * 0.01 * qRed(line[x]) - 150 + Contrast;
            float g = light * 0.01 * qGreen(line[x]) - 150 + Contrast;
            float b = light * 0.01 * qBlue(line[x]) - 150 + Contrast;
            r = Bound(0, r, 255);
            g = Bound(0, g, 255);
            b = Bound(0, b, 255);
            imgCopy.setPixel(x, y, qRgb(r, g, b));
        }

    }
    return imgCopy;
}

QImage QImageAPI::InverseColorImage(const QImage &img)
{
    QImage imgCopy = QImage(img);

    QRgb *line;
    for (int y = 0; y < img.height(); y++) {
        line = (QRgb *)img.scanLine(y);
        for (int x = 0; x < img.width(); x++) {

            imgCopy.setPixel(x, y, qRgb(255 - qRed(line[x]), 255 - qGreen(line[x]), 255 - qBlue(line[x])));
        }

    }
    return imgCopy;
}

QImage QImageAPI::oldImage(const QImage &img)
{
    QImage imgCopy = QImage(img);
    QRgb *line;
    for (int y = 0; y < img.height(); y++) {
        line = (QRgb *)img.scanLine(y);
        for (int x = 0; x < img.width(); x++) {
            float r = 0.393 * qRed(line[x]) + 0.769 * qGreen(line[x]) + 0.189 * qBlue(line[x]);
            float g = 0.349 * qRed(line[x]) + 0.686 * qGreen(line[x]) + 0.168 * qBlue(line[x]);
            float b = 0.272 * qRed(line[x]) + 0.534 * qGreen(line[x]) + 0.131 * qBlue(line[x]);
            r = Bound(0, r, 255);
            g = Bound(0, g, 255);
            b = Bound(0, b, 255);
            imgCopy.setPixel(x, y, qRgb(r, g, b));
        }

    }
    return imgCopy;
}


QImage QImageAPI::LaplaceSharpen(const QImage &origin)
{
    int width = origin.width();
    int height = origin.height();
    QImage newImage = QImage(width, height, QImage::Format_RGB888);
    int window[3][3] = {0, -1, 0, -1, 4, -1, 0, -1, 0};

    for (int x = 1; x < width; x++) {
        for (int y = 1; y < height; y++) {
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            //对每一个像素使用模板

            for (int m = x - 1; m <= x + 1; m++)
                for (int n = y - 1; n <= y + 1; n++) {
                    if (m >= 0 && m < width && n < height) {
                        sumR += QColor(origin.pixel(m, n)).red() * window[n - y + 1][m - x + 1];
                        sumG += QColor(origin.pixel(m, n)).green() * window[n - y + 1][m - x + 1];
                        sumB += QColor(origin.pixel(m, n)).blue() * window[n - y + 1][m - x + 1];
                    }
                }


            int old_r = QColor(origin.pixel(x, y)).red();
            sumR += old_r;
            sumR = qBound(0, sumR, 255);

            int old_g = QColor(origin.pixel(x, y)).green();
            sumG += old_g;
            sumG = qBound(0, sumG, 255);

            int old_b = QColor(origin.pixel(x, y)).blue();
            sumB += old_b;
            sumB = qBound(0, sumB, 255);


            newImage.setPixel(x, y, qRgb(sumR, sumG, sumB));
        }
    }

    return newImage;
}

QImage QImageAPI::SobelEdge(const QImage &origin)
{
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
    QImage grayImage = GreyScale(origin);
    int height = grayImage.height();
    int width = grayImage.width();
    QImage newImage = QImage(width, height, QImage::Format_RGB888);

    /* 改写下面这行解决：某些编译器下不支持“变长数组”的问题 */
    // float sobel_norm[width*height];


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
                        value_gx += Gx[p * 3 + k] * qRed(pixel);
                        value_gy += Gy[p * 3 + k] * qRed(pixel);
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
            newImage.setPixel(i, j, my_color.rgb());
        }
    }
    delete[] sobel_norm;
    return newImage;
}


QImage QImageAPI::GreyScale(QImage origin)
{
    QImage *newImage = new QImage(origin.width(), origin.height(),
                                  QImage::Format_ARGB32);
    QColor oldColor;

    for (int x = 0; x < newImage->width(); x++) {
        for (int y = 0; y < newImage->height(); y++) {
            oldColor = QColor(origin.pixel(x, y));
            int average = (oldColor.red() * 299 + oldColor.green() * 587 + oldColor.blue() * 114 + 500) / 1000;
            newImage->setPixel(x, y, qRgb(average, average, average));
        }
    }

    return *newImage;

}

/*****************************************************************************
 *                                 二值化
 * **************************************************************************/
QImage QImageAPI::Binaryzation(const QImage &origin)
{


    int width = origin.width();
    int height = origin.height();
    QImage newImg = QImage(width, height, QImage::Format_RGB888);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int gray = qGray(origin.pixel(x, y));
            int newGray;
            if (gray > 128)
                newGray = 255;
            else
                newGray = 0;
            newImg.setPixel(x, y, qRgb(newGray, newGray, newGray));
        }
    }
    return newImg;
}

/*****************************************************************************
 *                                 轮廓提取法
 * **************************************************************************/
QImage QImageAPI::ContourExtraction(const QImage &origin)
{
    int width = origin.width();
    int height = origin.height();
    int pixel[8];   // 当前像素周围的8个像素的像素值
//    int *pixel = new int[9];
    QImage binImg = Binaryzation(origin);
    QImage newImg = QImage(width, height, QImage::Format_RGB888);
    newImg.fill(Qt::white);

    for (int y = 1; y < height; y++) {
        for (int x = 1; x < width; x++) {
            memset(pixel, 0, 8);

            if (QColor(binImg.pixel(x, y)).red() == 0) {
                newImg.setPixel(x, y, qRgb(0, 0, 0));
                pixel[0] = QColor(binImg.pixel(x - 1, y - 1)).red();
                pixel[1] = QColor(binImg.pixel(x - 1, y)).red();
                pixel[2] = QColor(binImg.pixel(x - 1, y + 1)).red();
                pixel[3] = QColor(binImg.pixel(x, y - 1)).red();
                pixel[4] = QColor(binImg.pixel(x, y + 1)).red();
                pixel[5] = QColor(binImg.pixel(x + 1, y - 1)).red();
                pixel[6] = QColor(binImg.pixel(x + 1, y)).red();
                pixel[7] = QColor(binImg.pixel(x + 1, y + 1)).red();
                if (pixel[0] + pixel[1] + pixel[2] + pixel[3] + pixel[4] + pixel[5] + pixel[6] + pixel[7] == 0)
                    newImg.setPixel(x, y, qRgb(255, 255, 255));
            }
        }
    }

    return newImg;
}


/*****************************************************************************
 *                                   Flip
 * **************************************************************************/
QImage QImageAPI::Horizontal(const QImage &origin)
{
    QImage *newImage = new QImage(QSize(origin.width(), origin.height()),
                                  QImage::Format_ARGB32);
    QColor tmpColor;
    int r, g, b;
    for (int x = 0; x < newImage->width(); x++) {
        for (int y = 0; y < newImage->height(); y++) {
            tmpColor = QColor(origin.pixel(x, y));
            r = tmpColor.red();
            g = tmpColor.green();
            b = tmpColor.blue();

            newImage->setPixel(newImage->width() - x - 1, y, qRgb(r, g, b));

        }
    }
    return *newImage;
}


/*****************************************************************************
 *                                 金属拉丝效果
 * **************************************************************************/
QImage QImageAPI::Metal(QImage origin)
{
    QImage *baseImage = new QImage(origin);
    QImage darkImage = QImageAPI::Brightness(-100, origin);
    QImage greyImage = QImageAPI::GreyScale(darkImage);
    QPainter painter;

    QImage newImage = baseImage->scaled(QSize(origin.width(), origin.height()));

    painter.begin(&newImage);
    painter.setOpacity(0.5);
    painter.drawImage(0, 0, greyImage);
    painter.end();

    return newImage;
}

/*****************************************************************************
 *                          Adjust image brightness
 * **************************************************************************/
QImage QImageAPI::Brightness(int delta, QImage origin)
{
    QImage *newImage = new QImage(origin.width(), origin.height(),
                                  QImage::Format_ARGB32);

    QColor oldColor;
    int r, g, b;

    for (int x = 0; x < newImage->width(); x++) {
        for (int y = 0; y < newImage->height(); y++) {
            oldColor = QColor(origin.pixel(x, y));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue() + delta;

            // Check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            newImage->setPixel(x, y, qRgb(r, g, b));
        }
    }
    return *newImage;
}

QImage QImageAPI::transparencyImg(int delta, QImage origin)
{
    QImage *newImage = new QImage(origin.width(), origin.height(),
                                  QImage::Format_ARGB32);
    QColor oldColor;
    int r, g, b;
    for (int x = 0; x < newImage->width(); x++) {
        for (int y = 0; y < newImage->height(); y++) {
            oldColor = QColor(origin.pixel(x, y));

            r = oldColor.red() ;
            g = oldColor.green() ;
            b = oldColor.blue() ;

            newImage->setPixel(x, y, qRgba(r, g, b, delta));
        }
    }

    return *newImage;
}


QImage QImageAPI::Vertical(const QImage &origin)
{
    QImage *newImage = new QImage(QSize(origin.width(), origin.height()),
                                  QImage::Format_ARGB32);
    QColor tmpColor;
    int r, g, b;
    for (int x = 0; x < newImage->width(); x++) {
        for (int y = 0; y < newImage->height(); y++) {
            tmpColor = QColor(origin.pixel(x, y));
            r = tmpColor.red();
            g = tmpColor.green();
            b = tmpColor.blue();

            newImage->setPixel(x, newImage->height() - y - 1, qRgb(r, g, b));

        }
    }
    return *newImage;
}
