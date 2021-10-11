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
#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include "application.h"

#include <QGraphicsView>
#include <QList>

class ImageFilterInfo;
class QGraphicsPixmapItem;
class CropperWidget;
class scaleDialog;

//Normal indicates normal mode, and Basic mode does not accept global signals
enum ViewId {
    Normal,
    Basic,
};

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    ImageView(QWidget *parent = nullptr, ViewId id = Normal);
    //Open picture via path
    void openImage(const QString &path);

    //For mouse wheel sliding
    qreal windowRelativeScale() const;
    qreal imageRelativeScale() const;
    void scaleAtPoint(QPoint pos, qreal factor);
    void setScaleValue(qreal v);
    //Adaptive window
    void autoFit();

    void mouseMoveEvent(QMouseEvent *event) override;
    //Returns the current picture img
    const QImage image();
    //Set view mode
    void setViewId(ViewId id);

    void playThread(const ImageFilterInfo &info);
public slots:
    //Fit window size
    void fitWindow();
    //Fit to picture size
    void fitImage();
    //Rotate the picture and feel the index angle, - is left and + is righ
    void RotateImage(const int &index);
    //Save picture
    void savecurrentPic();
    //Crop picture
    void scaleImage();
    //Save as
    void savecurrentPicAs();
    //Open this picture
    void openImage(QImage *img);
    void openFilterImage(QImage img, isChange is);
    //Old photo filter
    void oldIMage();
    //Reset diagram
    void resetImage();
    //qimage Skin grinding
    void BEEPImage(double spatialDecay = 0.02, double photometricStandardDeviation = 10);
    //Warm color filter
    void warnImage(int index = 30);
    //Cool color filter
    void coolImage(int index = 30);
    //Grayscale filter
    void GrayScaleImage();
    //Brightness and saturation
    void lightContrastImage(int light = 100, int Contrast = 150);
    //Anti color filter
    void InverseColorImage();
    //lpls
    void LaplaceSharpenImage();
    //soder
    void soderImage();
    //Flip vertically
    void flipVertical();
    //Flip horizontally
    void flipHorizontal();
    //Contour acquisition
    void ContourExtraction();
    //metal wire-drawing
    void Metal();
    //Crop picture resolution
    void scaled();
    //Set picture transparency
    void SetTransparency();
    //Set as last processed picture
    void setLastImage();
    //Add new cache
    void addhisImage(QImage img, isChange is = Change);
    //Binarization
    void Binaryzation();
protected:

    void resizeEvent(QResizeEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;
private:
    QString m_currentPath;//Current picture path
    QGraphicsPixmapItem *m_pixmapItem{nullptr};//Item of the current image
    bool m_isFitImage = false;//Is it suitable for pictures
    bool m_isFitWindow = false;//Fit window
    qreal m_scal = 1.0;
    int   m_rotateAngel = 0; //Rotation angle
    QImage *m_currentImage{nullptr};//Current original image
    QImage m_FilterImage{nullptr};//Currently processed image
    QImage m_lightContrastImage{nullptr};//Brightness exposure image
    ViewId m_cureentId{Normal};//Current mode
    QList <QImage> m_hisImage;//Historical pictures
};

#endif // IMAGEVIEW_H
