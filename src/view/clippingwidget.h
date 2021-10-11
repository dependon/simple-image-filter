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
//    bool m_draging;//is Drag
//    QPoint m_startPostion;//Mouse position before dragging
//    QPoint m_framPostion;//The original position of the form
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

#endif // TESTClippingLabel_H
