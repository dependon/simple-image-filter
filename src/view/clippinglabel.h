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
#ifndef CLIPPINGLABEL_H
#define CLIPPINGLABEL_H

#include "control/label.h"

#include <QPixmap>
#include <QPen>

enum class OutputShape {
    RECT    = 0,
    ELLIPSE = 1
};

enum class CropperShape {
    DEFAULT     = 0,
    RECT          = 1,
    SQUARE        = 2,
    FIXED_RECT    = 3,
    ELLIPSE       = 4,
    CIRCLE        = 5,
    FIXED_ELLIPSE = 6
};


class ClippingLabel : public Label
{
    Q_OBJECT
public:
    ClippingLabel(int width, int height, QWidget *parent);

    //set original image
    void setOriginalImage(const QPixmap &pixmap);
    void setOutputShape(OutputShape shape);
    QPixmap getCroppedImage();
    QPixmap getCroppedImage(OutputShape shape);

    //Set cropper's shape
    void setRectCropper();
    void setSquareCropper();
    void setEllipseCropper();
    void setCircleCropper();
    void setFixedRectCropper(QSize size);
    void setFixedEllipseCropper(QSize size);
    void setCropper(CropperShape shape, QSize size);    // not recommended

    // Set cropper's fixed size
    void setCropperFixedSize(int fixedWidth, int fixedHeight);
    void setCropperFixedWidth(int fixedWidht);
    void setCropperFixedHeight(int fixedHeight);


    //Set cropper's minimum size
    //default: the twice of minimum of the edge lenght of drag square
    void setCropperMinimumSize(int minWidth, int minHeight);
    void setCropperMinimumWidth(int minWidth);
    void setCropperMinimumHeight(int minHeight);

    //Set the size, color, visibility of rectangular border
    void setShowRectBorder(bool show);
    QPen getBorderPen();
    void setBorderPen(const QPen &pen);

    //Set the size, color of drag square
    void setShowDragSquare(bool show);
    void setDragSquareEdge(int edge);
    void setDragSquareColor(const QColor &color);

    //Opacity Effect
    void setEnableOpacity(bool b = true);
    void setOpacity(double newOpacity);

signals:
    void sigCroppedImageChanged();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;

private:
    //Draw shapes
    void drawFillRect(QPoint centralPoint, int edge, QColor color);
    void drawRectOpacity();
    void drawEllipseOpacity();
    void drawOpacity(const QPainterPath &path);     // shadow effect
    void drawSquareEdge(bool onlyFourCorners);

    //Other utility methods
    int getPosInCropperRect(const QPoint &pt);
    bool isPosNearDragSquare(const QPoint &pt1, const QPoint &pt2);
    void resetCropperPos();
    void changeCursor();

    enum {
        RECT_OUTSIZD = 0,
        RECT_INSIDE = 1,
        RECT_TOP_LEFT,
        RECT_TOP,
        RECT_TOP_RIGHT,
        RECT_RIGHT,
        RECT_BOTTOM_RIGHT,
        RECT_BOTTOM,
        RECT_BOTTOM_LEFT,
        RECT_LEFT
    };



private:
    bool m_onlyFourCorners = true;
    bool m_isShowRectBorder = true;
    bool m_isLButtonPressed = false;
    bool m_isCursorPosCalculated = false;
    bool m_isShowDragSquare = true;
    bool m_isShowOpacityEffect = false;

    QPixmap m_originalImage;
    QPixmap m_tempImage;

    QPen m_borderPen;

    CropperShape m_cropperShape = CropperShape::DEFAULT;
    OutputShape  m_outputShape  = OutputShape::RECT;

    QRect m_imageRect;     // the whole image area in the label (not real size)
    QRect m_cropperNormalRect;     // a rectangle frame to choose image area (not real size)
    QRect m_cropperRealRect;     // cropper rect (real size)

    double m_opacity = 0.6;
    double m_scaledRate = 1.0;

    int m_cursorPosInCropperRect = RECT_OUTSIZD;
    int m_dragSquareEdge = 8;
    int m_cropperMinimumWidth = m_dragSquareEdge * 2;
    int m_cropperMinimumHeight = m_dragSquareEdge * 2;

    QPoint m_lastPos;
    QPoint m_currPos;

    QColor m_dragSquareColor = Qt::white;
};

#endif // CLIPPINGLABEL_H
