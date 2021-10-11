#include "clippinglabel.h"

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QDebug>
#include <QBitmap>

ClippingLabel::ClippingLabel(int width, int height, QWidget *parent) :
    Label(parent)
{
    this->setFixedSize(width, height);
    this->setAlignment(Qt::AlignCenter);
    this->setMouseTracking(true);

    m_borderPen.setWidth(1);
    m_borderPen.setColor(Qt::white);
    m_borderPen.setDashPattern(QVector<qreal>() << 3 << 3 << 3 << 3);
}

void ClippingLabel::setOriginalImage(const QPixmap &pixmap)
{
    m_originalImage = pixmap;

    int imgWidth = pixmap.width();
    int imgHeight = pixmap.height();
    int labelWidth = this->width();
    int labelHeight = this->height();
    int imgWidthInLabel;
    int imgHeightInLabel;

    if (imgWidth * labelHeight < imgHeight * labelWidth) {
        m_scaledRate = labelHeight / double(imgHeight);
        imgHeightInLabel = labelHeight;
        imgWidthInLabel = int(m_scaledRate * imgWidth);
        m_imageRect.setRect((labelWidth - imgWidthInLabel) / 2, 0,
                            imgWidthInLabel, imgHeightInLabel);
    } else {
        m_scaledRate = labelWidth / double(imgWidth);
        imgWidthInLabel = labelWidth;
        imgHeightInLabel = int(m_scaledRate * imgHeight);
        m_imageRect.setRect(0, (labelHeight - imgHeightInLabel) / 2,
                            imgWidthInLabel, imgHeightInLabel);
    }

    m_tempImage = m_originalImage.scaled(imgWidthInLabel, imgHeightInLabel,
                                         Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(m_tempImage);

    if (m_cropperShape >= CropperShape::FIXED_RECT) {
        m_cropperNormalRect.setWidth(int(m_cropperRealRect.width() * m_scaledRate));
        m_cropperNormalRect.setHeight(int(m_cropperRealRect.height() * m_scaledRate));
    }
    resetCropperPos();
}

void ClippingLabel::setOutputShape(OutputShape shape)
{
    m_outputShape = shape;
}


/*****************************************
 * set cropper's shape (and size)
*****************************************/
void ClippingLabel::setRectCropper()
{
    m_cropperShape = CropperShape::RECT;
    resetCropperPos();
}

void ClippingLabel::setSquareCropper()
{
    m_cropperShape = CropperShape::SQUARE;
    resetCropperPos();
}

void ClippingLabel::setEllipseCropper()
{
    m_cropperShape = CropperShape::ELLIPSE;
    resetCropperPos();
}

void ClippingLabel::setCircleCropper()
{
    m_cropperShape = CropperShape::CIRCLE;
    resetCropperPos();
}

void ClippingLabel::setFixedRectCropper(QSize size)
{
    m_cropperShape = CropperShape::FIXED_RECT;
    m_cropperRealRect.setSize(size);
    resetCropperPos();
}

void ClippingLabel::setFixedEllipseCropper(QSize size)
{
    m_cropperShape = CropperShape::FIXED_ELLIPSE;
    m_cropperRealRect.setSize(size);
    resetCropperPos();
}

// not recommended
void ClippingLabel::setCropper(CropperShape shape, QSize size)
{
    m_cropperShape = shape;
    m_cropperRealRect.setSize(size);
    resetCropperPos();
}

/*****************************************************************************
     * Set cropper's fixed size
    *****************************************************************************/
void ClippingLabel::setCropperFixedSize(int fixedWidth, int fixedHeight)
{
    m_cropperRealRect.setSize(QSize(fixedWidth, fixedHeight));
    resetCropperPos();
}

void ClippingLabel::setCropperFixedWidth(int fixedWidth)
{
    m_cropperRealRect.setWidth(fixedWidth);
    resetCropperPos();
}

void ClippingLabel::setCropperFixedHeight(int fixedHeight)
{
    m_cropperRealRect.setHeight(fixedHeight);
    resetCropperPos();
}

void ClippingLabel::setCropperMinimumSize(int minWidth, int minHeight)
{
    m_cropperMinimumWidth = minWidth; m_cropperMinimumHeight = minHeight;
}

void ClippingLabel::setCropperMinimumWidth(int minWidth)
{
    m_cropperMinimumWidth = minWidth;
}

void ClippingLabel::setCropperMinimumHeight(int minHeight)
{
    m_cropperMinimumHeight = minHeight;
}

void ClippingLabel::setShowRectBorder(bool show)
{
    m_isShowRectBorder = show;
}

QPen ClippingLabel::getBorderPen()
{
    return m_borderPen;
}

void ClippingLabel::setBorderPen(const QPen &pen)
{
    m_borderPen = pen;
}

void ClippingLabel::setShowDragSquare(bool show)
{
    m_isShowDragSquare = show;
}

void ClippingLabel::setDragSquareEdge(int edge)
{
    m_dragSquareEdge = (edge >= 3 ? edge : 3);
}

void ClippingLabel::setDragSquareColor(const QColor &color)
{
    m_dragSquareColor = color;
}

void ClippingLabel::setEnableOpacity(bool b)
{
    m_isShowOpacityEffect = b;
}

void ClippingLabel::setOpacity(double newOpacity)
{
    m_opacity = newOpacity;
}

/**********************************************
 * Move cropper to the center of the image
 * And resize to default
**********************************************/
void ClippingLabel::resetCropperPos()
{
    int labelWidth = this->width();
    int labelHeight = this->height();

    if (m_cropperShape == CropperShape::FIXED_RECT || m_cropperShape == CropperShape::FIXED_ELLIPSE) {
        m_cropperNormalRect.setWidth(int(m_cropperRealRect.width() * m_scaledRate));
        m_cropperNormalRect.setHeight(int(m_cropperRealRect.height() * m_scaledRate));
    }

    switch (m_cropperShape) {
    case CropperShape::DEFAULT:
        break;
    case CropperShape::FIXED_RECT:
    case CropperShape::FIXED_ELLIPSE: {
        m_cropperNormalRect.setRect((labelWidth - m_cropperNormalRect.width()) / 2,
                                    (labelHeight - m_cropperNormalRect.height()) / 2,
                                    m_cropperNormalRect.width(), m_cropperNormalRect.height());
        break;
    }
    case CropperShape::RECT:
    case CropperShape::SQUARE:
    case CropperShape::ELLIPSE:
    case CropperShape::CIRCLE: {
        int imgWidth = m_tempImage.width();
        int imgHeight = m_tempImage.height();
        int edge = int((imgWidth > imgHeight ? imgHeight : imgWidth) * 3 / 4.0);
        m_cropperNormalRect.setRect((labelWidth - edge) / 2, (labelHeight - edge) / 2, edge, edge);
        break;
    }
    }
}

QPixmap ClippingLabel::getCroppedImage()
{
    return getCroppedImage(m_outputShape);
}

QPixmap ClippingLabel::getCroppedImage(OutputShape shape)
{
    int startX = int((m_cropperNormalRect.left() - m_imageRect.left()) / m_scaledRate);
    int startY = int((m_cropperNormalRect.top() - m_imageRect.top()) / m_scaledRate);
    int croppedWidth = int(m_cropperNormalRect.width() / m_scaledRate);
    int croppedHeight = int(m_cropperNormalRect.height() / m_scaledRate);

    QPixmap resultImage(croppedWidth, croppedHeight);
    resultImage = m_originalImage.copy(startX, startY, croppedWidth, croppedHeight);

    // Set ellipse mask (cut to ellipse shape)
    if (shape == OutputShape::ELLIPSE) {
        QSize size(croppedWidth, croppedHeight);
        QBitmap mask(size);
        QPainter painter(&mask);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
        painter.setBrush(QColor(0, 0, 0));
        painter.drawRoundRect(0, 0, size.width(), size.height(), 99, 99);
        resultImage.setMask(mask);
    }

    return resultImage;
}


void ClippingLabel::paintEvent(QPaintEvent *event)
{
    // Draw original image
    QLabel::paintEvent(event);

    // Draw cropper and set some effects
    switch (m_cropperShape) {
    case CropperShape::DEFAULT:
        break;
    case CropperShape::FIXED_RECT:
        drawRectOpacity();
        break;
    case CropperShape::FIXED_ELLIPSE:
        drawEllipseOpacity();
        break;
    case CropperShape::RECT:
        drawRectOpacity();
        drawSquareEdge(!m_onlyFourCorners);
        break;
    case CropperShape::SQUARE:
        drawRectOpacity();
        drawSquareEdge(m_onlyFourCorners);
        break;
    case CropperShape::ELLIPSE:
        drawEllipseOpacity();
        drawSquareEdge(!m_onlyFourCorners);
        break;
    case CropperShape::CIRCLE:
        drawEllipseOpacity();
        drawSquareEdge(m_onlyFourCorners);
        break;
    }

    // Draw cropper rect
    if (m_isShowRectBorder) {
        QPainter painter(this);
        painter.setPen(m_borderPen);
        painter.drawRect(m_cropperNormalRect);
    }
}

void ClippingLabel::drawSquareEdge(bool onlyFourCorners)
{
    if (!m_isShowDragSquare)
        return;

    // Four corners
    drawFillRect(m_cropperNormalRect.topLeft(), m_dragSquareEdge, m_dragSquareColor);
    drawFillRect(m_cropperNormalRect.topRight(), m_dragSquareEdge, m_dragSquareColor);
    drawFillRect(m_cropperNormalRect.bottomLeft(), m_dragSquareEdge, m_dragSquareColor);
    drawFillRect(m_cropperNormalRect.bottomRight(), m_dragSquareEdge, m_dragSquareColor);

    // Four edges
    if (!onlyFourCorners) {
        int centralX = m_cropperNormalRect.left() + m_cropperNormalRect.width() / 2;
        int centralY = m_cropperNormalRect.top() + m_cropperNormalRect.height() / 2;
        drawFillRect(QPoint(m_cropperNormalRect.left(), centralY), m_dragSquareEdge, m_dragSquareColor);
        drawFillRect(QPoint(centralX, m_cropperNormalRect.top()), m_dragSquareEdge, m_dragSquareColor);
        drawFillRect(QPoint(m_cropperNormalRect.right(), centralY), m_dragSquareEdge, m_dragSquareColor);
        drawFillRect(QPoint(centralX, m_cropperNormalRect.bottom()), m_dragSquareEdge, m_dragSquareColor);
    }
}

void ClippingLabel::drawFillRect(QPoint centralPoint, int edge, QColor color)
{
    QRect rect(centralPoint.x() - edge / 2, centralPoint.y() - edge / 2, edge, edge);
    QPainter painter(this);
    painter.fillRect(rect, color);
}

// Opacity effect
void ClippingLabel::drawOpacity(const QPainterPath &path)
{
    QPainter painterOpac(this);
    painterOpac.setOpacity(m_opacity);
    painterOpac.fillPath(path, QBrush(Qt::black));
}

void ClippingLabel::drawRectOpacity()
{
    if (m_isShowOpacityEffect) {
        QPainterPath p1, p2, p;
        p1.addRect(m_imageRect);
        p2.addRect(m_cropperNormalRect);
        p = p1.subtracted(p2);
        drawOpacity(p);
    }
}

void ClippingLabel::drawEllipseOpacity()
{
    if (m_isShowOpacityEffect) {
        QPainterPath p1, p2, p;
        p1.addRect(m_imageRect);
        p2.addEllipse(m_cropperNormalRect);
        p = p1.subtracted(p2);
        drawOpacity(p);
    }
}

bool ClippingLabel::isPosNearDragSquare(const QPoint &pt1, const QPoint &pt2)
{
    return abs(pt1.x() - pt2.x()) * 2 <= m_dragSquareEdge
           && abs(pt1.y() - pt2.y()) * 2 <= m_dragSquareEdge;
}

int ClippingLabel::getPosInCropperRect(const QPoint &pt)
{
    if (isPosNearDragSquare(pt, QPoint(m_cropperNormalRect.right(), m_cropperNormalRect.center().y())))
        return RECT_RIGHT;
    if (isPosNearDragSquare(pt, m_cropperNormalRect.bottomRight()))
        return RECT_BOTTOM_RIGHT;
    if (isPosNearDragSquare(pt, QPoint(m_cropperNormalRect.center().x(), m_cropperNormalRect.bottom())))
        return RECT_BOTTOM;
    if (isPosNearDragSquare(pt, m_cropperNormalRect.bottomLeft()))
        return RECT_BOTTOM_LEFT;
    if (isPosNearDragSquare(pt, QPoint(m_cropperNormalRect.left(), m_cropperNormalRect.center().y())))
        return RECT_LEFT;
    if (isPosNearDragSquare(pt, m_cropperNormalRect.topLeft()))
        return RECT_TOP_LEFT;
    if (isPosNearDragSquare(pt, QPoint(m_cropperNormalRect.center().x(), m_cropperNormalRect.top())))
        return RECT_TOP;
    if (isPosNearDragSquare(pt, m_cropperNormalRect.topRight()))
        return RECT_TOP_RIGHT;
    if (m_cropperNormalRect.contains(pt, true))
        return RECT_INSIDE;
    return RECT_OUTSIZD;
}

/*************************************************
 *
 *  Change mouse cursor type
 *      Arrow, SizeHor, SizeVer, etc...
 *
*************************************************/

void ClippingLabel::changeCursor()
{
    switch (m_cursorPosInCropperRect) {
    case RECT_OUTSIZD:
        setCursor(Qt::ArrowCursor);
        break;
    case RECT_BOTTOM_RIGHT: {
        switch (m_cropperShape) {
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            setCursor(Qt::SizeFDiagCursor);
            break;
        default:
            break;
        }
        break;
    }
    case RECT_RIGHT: {
        switch (m_cropperShape) {
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            setCursor(Qt::SizeHorCursor);
            break;
        default:
            break;
        }
        break;
    }
    case RECT_BOTTOM: {
        switch (m_cropperShape) {
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            setCursor(Qt::SizeVerCursor);
            break;
        default:
            break;
        }
        break;
    }
    case RECT_BOTTOM_LEFT: {
        switch (m_cropperShape) {
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            break;
        }
        break;
    }
    case RECT_LEFT: {
        switch (m_cropperShape) {
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            setCursor(Qt::SizeHorCursor);
            break;
        default:
            break;
        }
        break;
    }
    case RECT_TOP_LEFT: {
        switch (m_cropperShape) {
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            setCursor(Qt::SizeFDiagCursor);
            break;
        default:
            break;
        }
        break;
    }
    case RECT_TOP: {
        switch (m_cropperShape) {
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            setCursor(Qt::SizeVerCursor);
            break;
        default:
            break;
        }
        break;
    }
    case RECT_TOP_RIGHT: {
        switch (m_cropperShape) {
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            break;
        }
        break;
    }
    case RECT_INSIDE: {
        setCursor(Qt::SizeAllCursor);
        break;
    }
    }
}

/*****************************************************
 *
 *  Mouse Events
 *
*****************************************************/

void ClippingLabel::mousePressEvent(QMouseEvent *e)
{
    m_currPos = m_lastPos = e->pos();
    m_isLButtonPressed = true;
}

void ClippingLabel::mouseMoveEvent(QMouseEvent *e)
{
    m_currPos = e->pos();
    if (!m_isCursorPosCalculated) {
        m_cursorPosInCropperRect = getPosInCropperRect(m_currPos);
        changeCursor();
    }

    if (!m_isLButtonPressed)
        return;
    if (!m_imageRect.contains(m_currPos))
        return;

    m_isCursorPosCalculated = true;

    int xOffset = m_currPos.x() - m_lastPos.x();
    int yOffset = m_currPos.y() - m_lastPos.y();
    m_lastPos = m_currPos;

    int disX = 0;
    int disY = 0;

    // Move cropper
    switch (m_cursorPosInCropperRect) {
    case RECT_OUTSIZD:
        break;
    case RECT_BOTTOM_RIGHT: {
        disX = m_currPos.x() - m_cropperNormalRect.left();
        disY = m_currPos.y() - m_cropperNormalRect.top();
        switch (m_cropperShape) {
        case CropperShape::DEFAULT:
        case CropperShape::FIXED_RECT:
        case CropperShape::FIXED_ELLIPSE:
            break;
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            setCursor(Qt::SizeFDiagCursor);
            if (disX >= m_cropperMinimumWidth && disY >= m_cropperMinimumHeight) {
                if (disX > disY && m_cropperNormalRect.top() + disX <= m_imageRect.bottom()) {
                    m_cropperNormalRect.setRight(m_currPos.x());
                    m_cropperNormalRect.setBottom(m_cropperNormalRect.top() + disX);
                    emit sigCroppedImageChanged();
                } else if (disX <= disY && m_cropperNormalRect.left() + disY <= m_imageRect.right()) {
                    m_cropperNormalRect.setBottom(m_currPos.y());
                    m_cropperNormalRect.setRight(m_cropperNormalRect.left() + disY);
                    emit sigCroppedImageChanged();
                }
            }
            break;
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            setCursor(Qt::SizeFDiagCursor);
            if (disX >= m_cropperMinimumWidth) {
                m_cropperNormalRect.setRight(m_currPos.x());
                emit sigCroppedImageChanged();
            }
            if (disY >= m_cropperMinimumHeight) {
                m_cropperNormalRect.setBottom(m_currPos.y());
                emit sigCroppedImageChanged();
            }
            break;
        }
        break;
    }
    case RECT_RIGHT: {
        disX = m_currPos.x() - m_cropperNormalRect.left();
        switch (m_cropperShape) {
        case CropperShape::DEFAULT:
        case CropperShape::FIXED_RECT:
        case CropperShape::FIXED_ELLIPSE:
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            break;
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            if (disX >= m_cropperMinimumWidth) {
                m_cropperNormalRect.setRight(m_currPos.x());
                emit sigCroppedImageChanged();
            }
            break;
        }
        break;
    }
    case RECT_BOTTOM: {
        disY = m_currPos.y() - m_cropperNormalRect.top();
        switch (m_cropperShape) {
        case CropperShape::DEFAULT:
        case CropperShape::FIXED_RECT:
        case CropperShape::FIXED_ELLIPSE:
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            break;
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            if (disY >= m_cropperMinimumHeight) {
                m_cropperNormalRect.setBottom(m_cropperNormalRect.bottom() + yOffset);
                emit sigCroppedImageChanged();
            }
            break;
        }
        break;
    }
    case RECT_BOTTOM_LEFT: {
        disX = m_cropperNormalRect.right() - m_currPos.x();
        disY = m_currPos.y() - m_cropperNormalRect.top();
        switch (m_cropperShape) {
        case CropperShape::DEFAULT:
            break;
        case CropperShape::FIXED_RECT:
        case CropperShape::FIXED_ELLIPSE:
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            if (disX >= m_cropperMinimumWidth) {
                m_cropperNormalRect.setLeft(m_currPos.x());
                emit sigCroppedImageChanged();
            }
            if (disY >= m_cropperMinimumHeight) {
                m_cropperNormalRect.setBottom(m_currPos.y());
                emit sigCroppedImageChanged();
            }
            break;
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            if (disX >= m_cropperMinimumWidth && disY >= m_cropperMinimumHeight) {
                if (disX > disY && m_cropperNormalRect.top() + disX <= m_imageRect.bottom()) {
                    m_cropperNormalRect.setLeft(m_currPos.x());
                    m_cropperNormalRect.setBottom(m_cropperNormalRect.top() + disX);
                    emit sigCroppedImageChanged();
                } else if (disX <= disY && m_cropperNormalRect.right() - disY >= m_imageRect.left()) {
                    m_cropperNormalRect.setBottom(m_currPos.y());
                    m_cropperNormalRect.setLeft(m_cropperNormalRect.right() - disY);
                    emit sigCroppedImageChanged();
                }
            }
            break;
        }
        break;
    }
    case RECT_LEFT: {
        disX = m_cropperNormalRect.right() - m_currPos.x();
        switch (m_cropperShape) {
        case CropperShape::DEFAULT:
        case CropperShape::FIXED_RECT:
        case CropperShape::FIXED_ELLIPSE:
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            break;
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            if (disX >= m_cropperMinimumHeight) {
                m_cropperNormalRect.setLeft(m_cropperNormalRect.left() + xOffset);
                emit sigCroppedImageChanged();
            }
            break;
        }
        break;
    }
    case RECT_TOP_LEFT: {
        disX = m_cropperNormalRect.right() - m_currPos.x();
        disY = m_cropperNormalRect.bottom() - m_currPos.y();
        switch (m_cropperShape) {
        case CropperShape::DEFAULT:
        case CropperShape::FIXED_RECT:
        case CropperShape::FIXED_ELLIPSE:
            break;
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            if (disX >= m_cropperMinimumWidth) {
                m_cropperNormalRect.setLeft(m_currPos.x());
                emit sigCroppedImageChanged();
            }
            if (disY >= m_cropperMinimumHeight) {
                m_cropperNormalRect.setTop(m_currPos.y());
                emit sigCroppedImageChanged();
            }
            break;
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            if (disX >= m_cropperMinimumWidth && disY >= m_cropperMinimumHeight) {
                if (disX > disY && m_cropperNormalRect.bottom() - disX >= m_imageRect.top()) {
                    m_cropperNormalRect.setLeft(m_currPos.x());
                    m_cropperNormalRect.setTop(m_cropperNormalRect.bottom() - disX);
                    emit sigCroppedImageChanged();
                } else if (disX <= disY && m_cropperNormalRect.right() - disY >= m_imageRect.left()) {
                    m_cropperNormalRect.setTop(m_currPos.y());
                    m_cropperNormalRect.setLeft(m_cropperNormalRect.right() - disY);
                    emit sigCroppedImageChanged();
                }
            }
            break;
        }
        break;
    }
    case RECT_TOP: {
        disY = m_cropperNormalRect.bottom() - m_currPos.y();
        switch (m_cropperShape) {
        case CropperShape::DEFAULT:
        case CropperShape::FIXED_RECT:
        case CropperShape::FIXED_ELLIPSE:
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            break;
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            if (disY >= m_cropperMinimumHeight) {
                m_cropperNormalRect.setTop(m_cropperNormalRect.top() + yOffset);
                emit sigCroppedImageChanged();
            }
            break;
        }
        break;
    }
    case RECT_TOP_RIGHT: {
        disX = m_currPos.x() - m_cropperNormalRect.left();
        disY = m_cropperNormalRect.bottom() - m_currPos.y();
        switch (m_cropperShape) {
        case CropperShape::DEFAULT:
        case CropperShape::FIXED_RECT:
        case CropperShape::FIXED_ELLIPSE:
            break;
        case CropperShape::RECT:
        case CropperShape::ELLIPSE:
            if (disX >= m_cropperMinimumWidth) {
                m_cropperNormalRect.setRight(m_currPos.x());
                emit sigCroppedImageChanged();
            }
            if (disY >= m_cropperMinimumHeight) {
                m_cropperNormalRect.setTop(m_currPos.y());
                emit sigCroppedImageChanged();
            }
            break;
        case CropperShape::SQUARE:
        case CropperShape::CIRCLE:
            if (disX >= m_cropperMinimumWidth && disY >= m_cropperMinimumHeight) {
                if (disX < disY && m_cropperNormalRect.left() + disY <= m_imageRect.right()) {
                    m_cropperNormalRect.setTop(m_currPos.y());
                    m_cropperNormalRect.setRight(m_cropperNormalRect.left() + disY);
                    emit sigCroppedImageChanged();
                } else if (disX >= disY && m_cropperNormalRect.bottom() - disX >= m_imageRect.top()) {
                    m_cropperNormalRect.setRight(m_currPos.x());
                    m_cropperNormalRect.setTop(m_cropperNormalRect.bottom() - disX);
                    emit sigCroppedImageChanged();
                }
            }
            break;
        }
        break;
    }
    case RECT_INSIDE: {
        // Make sure the cropperRect is entirely inside the imageRecct
        if (xOffset > 0) {
            if (m_cropperNormalRect.right() + xOffset > m_imageRect.right())
                xOffset = 0;
        } else if (xOffset < 0) {
            if (m_cropperNormalRect.left() + xOffset < m_imageRect.left())
                xOffset = 0;
        }
        if (yOffset > 0) {
            if (m_cropperNormalRect.bottom() + yOffset > m_imageRect.bottom())
                yOffset = 0;
        } else if (yOffset < 0) {
            if (m_cropperNormalRect.top() + yOffset < m_imageRect.top())
                yOffset = 0;
        }
        m_cropperNormalRect.moveTo(m_cropperNormalRect.left() + xOffset, m_cropperNormalRect.top() + yOffset);
        emit sigCroppedImageChanged();
    }
    break;
    }

    repaint();
}

void ClippingLabel::mouseReleaseEvent(QMouseEvent *)
{
    m_isLButtonPressed = false;
    m_isCursorPosCalculated = false;
    setCursor(Qt::ArrowCursor);
}

