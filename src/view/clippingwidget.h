#ifndef TESTClippingLabel_H
#define TESTClippingLabel_H

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
private:
    bool m_draging;//是否拖动
    QPoint m_startPostion;//拖动前鼠标位置
    QPoint m_framPostion;//窗体的原始位置
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
    void setLabelColor(QLabel *label, QColor color)
    {
        QPixmap pixmap(QSize(80, 25));
        pixmap.fill(color);
        label->setPixmap(pixmap);
    }

private:
    ClippingLabel *imgCropperLabel;
    QHBoxLayout *mainLayout;

    Label *labelPreviewImage;

    Combox *comboOutputShape;
    Combox *comboCropperShape;

    LineEdit *editOriginalImagePath;
    openImageButton *btnChooseOriginalImagePath;

    LineEdit *editCropperFixedWidth;
    LineEdit *editCropperFixedHeight;
    LineEdit *editCropperMinWidth;
    LineEdit *editCropperMinHeight;

    QCheckBox *checkShowDragSquare;
    QCheckBox *checkEnableOpacity;
    QSlider *sliderOpacity;
    QLineEdit *editDragSquareEdge;

    QCheckBox *checkShowRectBorder;
    Label *labelRectBorderColor;
    openImageButton *btnChooseRectBorderCorlor;

    Label *labelDragSquareColor;
    openImageButton *btnChooseDragSquareColor;

    openImageButton *btnSavePreview;
    PushButton *btnQuit;


};

#endif // TESTClippingLabel_H
