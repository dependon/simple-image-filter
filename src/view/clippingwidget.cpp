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
#include "clippingwidget.h"
#include <QFormLayout>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#ifdef USE_DTK
#include <DWidgetUtil>
#include <DTitlebar>
#include <DColorDialog>
#endif

ClippingWidget::ClippingWidget(QWidget *parent) :
    QWidget(parent)
{

    setupLayout();
    init();
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowTitle(tr("scale image"));
}

ClippingWidget::~ClippingWidget()
{

}


void ClippingWidget::setupLayout()
{
    m_imgCropperLabel = new ClippingLabel(600, 500, this);
    m_imgCropperLabel->setFrameStyle(1);

    m_comboOutputShape = new Combox(this);
    m_comboCropperShape = new Combox(this);

    m_labelPreviewImage = new Label(this);

    m_editOriginalImagePath = new LineEdit(this);
    m_btnChooseOriginalImagePath = new openImageButton(this);
    QHBoxLayout *hOriginalImagePathLayout = new QHBoxLayout();
    hOriginalImagePathLayout->addWidget(m_editOriginalImagePath);
    hOriginalImagePathLayout->addWidget(m_btnChooseOriginalImagePath);

    m_editCropperFixedWidth = new LineEdit(this);
    m_editCropperFixedHeight = new LineEdit(this);
    QHBoxLayout *hCropperFixedSizeLayout = new QHBoxLayout();
    hCropperFixedSizeLayout->addWidget(m_editCropperFixedWidth);
    hCropperFixedSizeLayout->addWidget(m_editCropperFixedHeight);

    m_editCropperMinWidth = new LineEdit(this);
    m_editCropperMinWidth->setText("8");
    m_editCropperMinHeight = new LineEdit(this);
    m_editCropperMinHeight->setText("8");
    QHBoxLayout *hCropperMinSizeLayout = new QHBoxLayout();
    hCropperMinSizeLayout->addWidget(m_editCropperMinWidth);
    hCropperMinSizeLayout->addWidget(m_editCropperMinHeight);

    m_checkEnableOpacity = new QCheckBox(this);
    m_sliderOpacity = new QSlider(Qt::Horizontal, this);

    m_checkShowDragSquare = new QCheckBox(this);
    m_editDragSquareEdge = new QLineEdit("8", this);
    m_checkShowRectBorder = new QCheckBox(this);

    m_labelRectBorderColor = new Label(this);

    m_btnChooseRectBorderCorlor = new openImageButton(this);
    QHBoxLayout *hRectBorderColorLayout = new QHBoxLayout();
    hRectBorderColorLayout->addWidget(m_labelRectBorderColor);
    hRectBorderColorLayout->addWidget(m_btnChooseRectBorderCorlor);

    m_labelDragSquareColor = new Label(this);
    m_btnChooseDragSquareColor = new openImageButton(this);
    QHBoxLayout *hDragSquareColorLayout = new QHBoxLayout();
    hDragSquareColorLayout->addWidget(m_labelDragSquareColor);
    hDragSquareColorLayout->addWidget(m_btnChooseDragSquareColor);

    QFormLayout *formLayout1 = new QFormLayout();
    formLayout1->addRow(new QLabel(tr("Preview:")), m_labelPreviewImage);
    formLayout1->addRow(new QLabel(tr("OriginalImage:"), this), hOriginalImagePathLayout);
    formLayout1->addRow(new QLabel(tr("OutputShape:"), this), m_comboOutputShape);
    formLayout1->addRow(new QLabel(tr("CropperShape:"), this), m_comboCropperShape);
    formLayout1->addRow(new QLabel(tr("FixedSize:"), this), hCropperFixedSizeLayout);
    formLayout1->addRow(new QLabel(tr("MinimumSize:"), this), hCropperMinSizeLayout);

    QFormLayout *formLayout2 = new QFormLayout();
    formLayout2->addRow(new QLabel(tr("EnableOpacity:"), this), m_checkEnableOpacity);
    formLayout2->addRow(new QLabel(tr("Opacity:"), this), m_sliderOpacity);

    QFormLayout *formLayout3 = new QFormLayout();
    formLayout3->addRow(new QLabel(tr("ShowDragSquare:"), this), m_checkShowDragSquare);
    formLayout3->addRow(new QLabel(tr("DragSquareEdge:"), this), m_editDragSquareEdge);
    formLayout3->addRow(new QLabel(tr("DragSquareColor:"), this), hDragSquareColorLayout);

    QFormLayout *formLayout4 = new QFormLayout();
    formLayout4->addRow(new QLabel(tr("ShowRectBorder:"), this), m_checkShowRectBorder);
    formLayout4->addRow(new QLabel(tr("RectBorderColor:"), this), hRectBorderColorLayout);

    m_btnSavePreview = new openImageButton(tr("Save"), this);
    m_btnQuit = new PushButton(tr("Quit"), this);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(m_btnSavePreview);
    btnLayout->addStretch();
    btnLayout->addWidget(m_btnQuit);
    btnLayout->addStretch();

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addLayout(formLayout1);
    vLayout->addStretch();
    vLayout->addLayout(formLayout2);
    vLayout->addStretch();
    vLayout->addLayout(formLayout3);
    vLayout->addStretch();
    vLayout->addLayout(formLayout4);
    vLayout->addStretch();
    vLayout->addLayout(btnLayout);

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->addWidget(m_imgCropperLabel);
    m_mainLayout->addLayout(vLayout);
    this->setLayout(m_mainLayout);
}

void ClippingWidget::init()
{
    m_imgCropperLabel->setRectCropper();
    m_editCropperFixedWidth->setEnabled(false);
    m_editCropperFixedHeight->setEnabled(false);

    m_labelPreviewImage->setFixedSize(96, 96);
    m_labelPreviewImage->setAlignment(Qt::AlignCenter);
    m_labelPreviewImage->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    connect(m_imgCropperLabel, &ClippingLabel::sigCroppedImageChanged,
            this, &ClippingWidget::onUpdatePreview);

    m_btnChooseOriginalImagePath->setIcon(QIcon("res/select-file.ico"));
    m_btnChooseOriginalImagePath->setFixedWidth(30);
    connect(m_btnChooseOriginalImagePath, &QPushButton::clicked,
            this, &ClippingWidget::onChooseOriginalImage);

    m_comboOutputShape->addItem(tr("Rect/Square"));
    m_comboOutputShape->addItem(tr("Ellipse/Circle"));
    connect(m_comboOutputShape, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onOutputShapeChanged(int)));

    m_comboCropperShape->addItem(tr("Rect"));
    m_comboCropperShape->addItem(tr("Square"));
    m_comboCropperShape->addItem(tr("FixedRect"));
    m_comboCropperShape->addItem(tr("Ellipse"));
    m_comboCropperShape->addItem(tr("Circle"));
    m_comboCropperShape->addItem(tr("FixedEllipse"));
    connect(m_comboCropperShape, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onCropperShapeChanged(int)));

    connect(m_editCropperFixedWidth, &LineEdit::textChanged,
            this, &ClippingWidget::onFixedWidthChanged);
    connect(m_editCropperFixedHeight, &LineEdit::textChanged,
            this, &ClippingWidget::onFixedHeightChanged);
    connect(m_editCropperMinWidth, &LineEdit::textChanged,
            this, &ClippingWidget::onMinWidthChanged);
    connect(m_editCropperMinHeight, &LineEdit::textChanged,
            this, &ClippingWidget::onMinHeightChanged);

    m_checkEnableOpacity->setCheckState(Qt::Checked);
    m_imgCropperLabel->setEnableOpacity(true);
    connect(m_checkEnableOpacity, &QCheckBox::stateChanged,
            this, &ClippingWidget::onEnableOpacityChanged);

    m_checkShowDragSquare->setCheckState(Qt::Checked);
    m_imgCropperLabel->setShowDragSquare(true);
    connect(m_checkShowDragSquare, &QCheckBox::stateChanged,
            this, &ClippingWidget::onShowDragSquareChanged);
    connect(m_editDragSquareEdge, &QLineEdit::textChanged,
            this, &ClippingWidget::onDragSquareEdgeChanged);

    m_sliderOpacity->setRange(0, 100);
    m_sliderOpacity->setValue(60);
    connect(m_sliderOpacity, &QSlider::valueChanged,
            this, &ClippingWidget::onOpacityChanged);

    m_checkShowRectBorder->setCheckState(Qt::Checked);
    connect(m_checkShowRectBorder, &QCheckBox::stateChanged,
            this, &ClippingWidget::onShowRectBorder);

    setLabelColor(m_labelRectBorderColor, Qt::white);
    m_btnChooseRectBorderCorlor->setIcon(QIcon("res/color-palette.ico"));
    m_btnChooseRectBorderCorlor->setFixedWidth(40);
    connect(m_btnChooseRectBorderCorlor, &QPushButton::clicked,
            this, &ClippingWidget::onChooseRectBorderColor);

    setLabelColor(m_labelDragSquareColor, Qt::white);
    m_btnChooseDragSquareColor->setIcon(QIcon("res/color-palette.ico"));
    m_btnChooseDragSquareColor->setFixedWidth(40);
    connect(m_btnChooseDragSquareColor, &QPushButton::clicked,
            this, &ClippingWidget::onChooseDragSquareColor);

    connect(m_btnSavePreview, &QPushButton::clicked,
            this, &ClippingWidget::onSaveCroppedImage);
    connect(m_btnQuit, &QPushButton::clicked,
    this, [ = ] {
        this->close();
        QWidget *widget = static_cast<QWidget *>(parent());
        if (widget)
        {
            widget->close();
        }
    });

    m_imgCropperLabel->update();
}


/*****************************************************************************
 *
 *    slots
 *
*****************************************************************************/

void ClippingWidget::onChooseOriginalImage()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a picture"), "",
                                                    ".png");
    if (filename.isNull())
        return;

    QPixmap pixmap;
    if (!pixmap.load(filename)) {
        QMessageBox::critical(this, tr("Error"), tr("Load image failed"), QMessageBox::Ok);
        return;
    }

    m_editOriginalImagePath->setText(filename);
    m_imgCropperLabel->setOriginalImage(pixmap);
    m_imgCropperLabel->update();
    onUpdatePreview();
    m_labelPreviewImage->setFrameStyle(0);
}

void ClippingWidget::setChooseCurrentImage(QPixmap pix)
{
    m_imgCropperLabel->setOriginalImage(pix);
    m_imgCropperLabel->update();
    onUpdatePreview();
    m_labelPreviewImage->setFrameStyle(0);
}

void ClippingWidget::onOutputShapeChanged(int idx)
{
    // Output: Rectangular
    if (idx == 0)
        m_imgCropperLabel->setOutputShape(OutputShape::RECT);
    else
        m_imgCropperLabel->setOutputShape(OutputShape::ELLIPSE);
    onUpdatePreview();
}

void ClippingWidget::onCropperShapeChanged(int idx)
{
    switch (CropperShape(idx + 1)) {
    case CropperShape::RECT: {
        m_imgCropperLabel->setRectCropper();
        m_editCropperFixedWidth->setEnabled(false);
        m_editCropperFixedHeight->setEnabled(false);
        m_editCropperMinWidth->setEnabled(true);
        m_editCropperMinHeight->setEnabled(true);
        m_checkShowDragSquare->setEnabled(true);
        m_editDragSquareEdge->setEnabled(true);
        m_btnChooseDragSquareColor->setEnabled(true);
        break;
    }
    case CropperShape::SQUARE: {
        m_imgCropperLabel->setSquareCropper();
        m_editCropperFixedWidth->setEnabled(false);
        m_editCropperFixedHeight->setEnabled(false);
        m_editCropperMinWidth->setEnabled(true);
        m_editCropperMinHeight->setEnabled(true);
        m_checkShowDragSquare->setEnabled(true);
        m_editDragSquareEdge->setEnabled(true);
        m_btnChooseDragSquareColor->setEnabled(true);
        break;
    }
    case CropperShape::FIXED_RECT: {
        m_imgCropperLabel->setFixedRectCropper(QSize(64, 64));
        m_editCropperFixedWidth->setEnabled(true);
        m_editCropperFixedHeight->setEnabled(true);
        m_editCropperMinWidth->setEnabled(false);
        m_editCropperMinHeight->setEnabled(false);
        m_editCropperFixedWidth->setText("64");
        m_editCropperFixedHeight->setText("64");
        m_checkShowDragSquare->setEnabled(false);
        m_editDragSquareEdge->setEnabled(false);
        m_btnChooseDragSquareColor->setEnabled(false);
        break;
    }
    case CropperShape::ELLIPSE: {
        m_imgCropperLabel->setEllipseCropper();
        m_editCropperFixedWidth->setEnabled(false);
        m_editCropperFixedHeight->setEnabled(false);
        m_editCropperMinWidth->setEnabled(true);
        m_editCropperMinHeight->setEnabled(true);
        m_checkShowDragSquare->setEnabled(true);
        m_editDragSquareEdge->setEnabled(true);
        m_btnChooseDragSquareColor->setEnabled(true);
        break;
    }
    case CropperShape::CIRCLE: {
        m_imgCropperLabel->setCircleCropper();
        m_editCropperFixedWidth->setEnabled(false);
        m_editCropperFixedHeight->setEnabled(false);
        m_editCropperMinWidth->setEnabled(true);
        m_editCropperMinHeight->setEnabled(true);
        m_checkShowDragSquare->setEnabled(true);
        m_editDragSquareEdge->setEnabled(true);
        m_btnChooseDragSquareColor->setEnabled(true);
        break;
    }
    case CropperShape::FIXED_ELLIPSE:
        m_imgCropperLabel->setFixedEllipseCropper(QSize(64, 64));
        m_editCropperFixedWidth->setEnabled(true);
        m_editCropperFixedHeight->setEnabled(true);
        m_editCropperMinWidth->setEnabled(false);
        m_editCropperMinHeight->setEnabled(false);
        m_editCropperFixedWidth->setText("64");
        m_editCropperFixedHeight->setText("64");
        m_checkShowDragSquare->setEnabled(false);
        m_editDragSquareEdge->setEnabled(false);
        m_btnChooseDragSquareColor->setEnabled(false);
        break;
    case CropperShape::DEFAULT:
        break;
    }

    m_imgCropperLabel->update();
    onUpdatePreview();
}

void ClippingWidget::onEnableOpacityChanged(int state)
{
    if (state == Qt::Checked) {
        m_sliderOpacity->setEnabled(true);
        m_imgCropperLabel->setEnableOpacity(true);
    } else {
        m_sliderOpacity->setEnabled(false);
        m_imgCropperLabel->setEnableOpacity(false);
    }
    m_imgCropperLabel->update();
}

void ClippingWidget::onShowDragSquareChanged(int state)
{
    if (state == Qt::Checked) {
        m_editDragSquareEdge->setEnabled(true);
        m_btnChooseDragSquareColor->setEnabled(true);
        m_imgCropperLabel->setShowDragSquare(true);
    } else {
        m_editDragSquareEdge->setEnabled(false);
        m_btnChooseDragSquareColor->setEnabled(false);
        m_imgCropperLabel->setShowDragSquare(false);
    }
    m_imgCropperLabel->update();
}

void ClippingWidget::onDragSquareEdgeChanged(QString edge)
{
    m_imgCropperLabel->setDragSquareEdge(edge.toInt());
    m_imgCropperLabel->update();
}

void ClippingWidget::onOpacityChanged(int val)
{
    m_imgCropperLabel->setOpacity(val / 100.0);
    m_imgCropperLabel->update();
}

void ClippingWidget::onFixedWidthChanged(QString width)
{
    m_imgCropperLabel->setCropperFixedWidth(width.toInt());
    m_imgCropperLabel->update();
}

void ClippingWidget::onFixedHeightChanged(QString height)
{
    m_imgCropperLabel->setCropperFixedHeight(height.toInt());
    m_imgCropperLabel->update();
}

void ClippingWidget::onMinWidthChanged(QString width)
{
    m_imgCropperLabel->setCropperMinimumWidth(width.toInt());
    m_imgCropperLabel->update();
}

void ClippingWidget::onMinHeightChanged(QString height)
{
    m_imgCropperLabel->setMinimumHeight(height.toInt());
    m_imgCropperLabel->update();
}

void ClippingWidget::onShowRectBorder(int state)
{
    if (state == Qt::Checked) {
        m_btnChooseRectBorderCorlor->setEnabled(true);
        m_imgCropperLabel->setShowRectBorder(true);
    } else {
        m_btnChooseRectBorderCorlor->setEnabled(false);
        m_imgCropperLabel->setShowRectBorder(false);
    }
    m_imgCropperLabel->update();
}

void ClippingWidget::onChooseRectBorderColor()
{
#ifdef USE_DTK
    QColor color = DColorDialog::getColor(m_imgCropperLabel->getBorderPen().color(), this);
#else
    QColor color = QColorDialog::getColor(m_imgCropperLabel->getBorderPen().color(), this);
#endif
    if (color.isValid()) {
        setLabelColor(m_labelRectBorderColor, color);
        QPen pen = m_imgCropperLabel->getBorderPen();
        pen.setColor(color);
        m_imgCropperLabel->setBorderPen(pen);
        m_imgCropperLabel->update();
    }
}

void ClippingWidget::onChooseDragSquareColor()
{
#ifdef USE_DTK
    QColor color = DColorDialog::getColor(Qt::white, this);
#else
    QColor color = QColorDialog::getColor(Qt::white, this);
#endif

    if (color.isValid()) {
        setLabelColor(m_labelDragSquareColor, color);
        m_imgCropperLabel->setDragSquareColor(color);
        m_imgCropperLabel->update();
    }
}

void ClippingWidget::onUpdatePreview()
{
    QPixmap preview = m_imgCropperLabel->getCroppedImage();
    preview = preview.scaled(m_labelPreviewImage->width(), m_labelPreviewImage->height(),
                             Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_labelPreviewImage->setPixmap(preview);
}

void ClippingWidget::onSaveCroppedImage()
{
    App->sigFilterImage(m_imgCropperLabel->getCroppedImage().toImage());
    close();
    QWidget *widget = static_cast<QWidget *>(parent());
    if (widget) {
        widget->close();
    }
}

void ClippingWidget::setLabelColor(QLabel *label, QColor color)
{
    QPixmap pixmap(QSize(80, 25));
    pixmap.fill(color);
    label->setPixmap(pixmap);
}

