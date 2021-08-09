#ifndef TESTIMAGECROPPERLABEL_H
#define TESTIMAGECROPPERLABEL_H

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
#include "imagecropperlabel.h"
#include "control/statusbarwidget.h"
#include "control/pushbutton.h"
#include "control/toolbutton.h"
#include "control/openimagebutton.h"
#include "control/combox.h"
#include "control/lineedit.h"
#include "control/label.h"

class ImageCropperDemo : public QWidget
{
    Q_OBJECT
public:
    ImageCropperDemo(QWidget *parent = nullptr);
    ~ImageCropperDemo();
    void setupLayout();
    void init();
    void setChooseCurrentImage(QPixmap pix);
protected:
//    void mousePressEvent(QMouseEvent *event);//按下
//    void mouseMoveEvent(QMouseEvent *event);//移动
//    void mouseReleaseEvent(QMouseEvent *event);//抬起
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
    ImageCropperLabel *imgCropperLabel;
    QHBoxLayout *mainLayout;

    Label *labelPreviewImage;

    combox *comboOutputShape;
    combox *comboCropperShape;

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

#endif // TESTIMAGECROPPERLABEL_H
