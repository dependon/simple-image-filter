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
#ifndef CLIPPINGWIDGET_H
#define CLIPPINGWIDGET_H

#include <QObject>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QSlider>

#include "application.h"
#include "clippinglabel.h"
#include "control/statusbarwidget.h"
#include "control/pushbutton.h"
#include "control/toolbutton.h"
#include "control/openimagebutton.h"
#include "control/combox.h"
#include "control/lineedit.h"
#include "control/label.h"

class ClippingWidget : public QWidget
{
    Q_OBJECT
public:
    ClippingWidget(QWidget *parent = nullptr);
    ~ClippingWidget();
    void setupLayout();
    void init();
    void setChooseCurrentImage(QPixmap pix);

public slots:
    void onOutputShapeChanged(int idx);
    void onCropperShapeChanged(int idx);
    void onEnableOpacityChanged(int state);
    void onShowDragSquareChanged(int state);
    void onDragSquareEdgeChanged(QString edge);
    void onOpacityChanged(int val);
    void onFixedWidthChanged(QString width);
    void onFixedHeightChanged(QString height);
    void onMinWidthChanged(QString width);
    void onMinHeightChanged(QString height);
    void onShowRectBorder(int state);
    void onChooseRectBorderColor();
    void onChooseDragSquareColor();
    void onChooseOriginalImage();
    void onUpdatePreview();
    void onSaveCroppedImage();

private:
    void setLabelColor(QLabel *label, QColor color);

private:
    ClippingLabel *m_imgCropperLabel = nullptr;
    QHBoxLayout *m_mainLayout = nullptr;
    Label *m_labelPreviewImage = nullptr;
    Label *m_labelRectBorderColor = nullptr;
    Label *m_labelDragSquareColor = nullptr;
    Combox *m_comboOutputShape = nullptr;
    Combox *m_comboCropperShape = nullptr;
    LineEdit *m_editOriginalImagePath = nullptr;
    LineEdit *m_editCropperFixedWidth = nullptr;
    LineEdit *m_editCropperFixedHeight = nullptr;
    LineEdit *m_editCropperMinWidth = nullptr;
    LineEdit *m_editCropperMinHeight = nullptr;
    QLineEdit *m_editDragSquareEdge = nullptr;
    QCheckBox *m_checkShowDragSquare = nullptr;
    QCheckBox *m_checkEnableOpacity = nullptr;
    QCheckBox *m_checkShowRectBorder = nullptr;
    QSlider *m_sliderOpacity = nullptr;
    openImageButton *m_btnChooseOriginalImagePath = nullptr;
    openImageButton *m_btnChooseRectBorderCorlor = nullptr;
    openImageButton *m_btnChooseDragSquareColor = nullptr;
    openImageButton *m_btnSavePreview = nullptr;
    PushButton *m_btnQuit = nullptr;
};

#endif // CLIPPINGWIDGET_H
