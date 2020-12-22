#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class Menu;
class QSlider;
class PushButton;
class StatusBarWidget;
class QGraphicsPixmapItem;
namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    //初始化函数
    bool Init();
    //初始化状态栏btn函数
    void initBtn();
    //初始化信号槽
    void initConnect();
    //初始化右键菜单
    void initMenu();
    //反初始化
    bool Uninit();
    //通过路径打开图片
    void openImage(const QString& path);

    ~MainWidget();
protected:
    //窗口显示事件
    void showEvent(QShowEvent *event) override;
    //窗口大小改变事件
    void resizeEvent(QResizeEvent *event) override;
    //事件钩子
    bool eventFilter(QObject *obj, QEvent *event) override;
    //右键菜单事件
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    //打开文管选择图片文件
    void openImageFile();
    //设置栈窗口(0为打开的初始界面,1为图像显示界面)
    void setCurrentWidget(const int& index);
    //在窗口1打开文管选择文件
    void on_openPicture_clicked();

private:
    Ui::MainWidget *ui;

    StatusBarWidget *m_statusbarWidget{nullptr};    //栈窗体

    PushButton *m_openBtn{nullptr};    //打开按钮(下方状态栏)
    PushButton *m_fitImageBtn{nullptr};    //适应图片大小按钮(下方状态栏)
    PushButton *m_fitWindowBtn{nullptr};    //适应窗口大小按钮(下方状态栏)
    PushButton *m_rotateRight{nullptr};    //向右旋转按钮按钮(下方状态栏)
    PushButton *m_rotateLeft{nullptr};   //向左旋转按钮按钮(下方状态栏)
    PushButton *m_saveBtn{nullptr};    //保存图片按钮(下方状态栏)
    PushButton *m_resetBtn{nullptr};    //还原原来的图片状态按钮(下方状态栏)

    Menu *m_leftMenu{nullptr};  //右键菜单

    QAction *m_openAction{nullptr};  //打开按钮(右键菜单)
    QAction *m_fitImageAction{nullptr};//适应图片大小按钮(右键菜单)
    QAction *m_fitWindowAction{nullptr};    //适应窗口大小按钮(右键菜单)
    QAction *m_rotateRightAction{nullptr};   //向右旋转按钮按钮(右键菜单)
    QAction *m_rotateLeftAction{nullptr};    //向左旋转按钮按钮(右键菜单)
    QAction *m_saveBAction{nullptr};    //保存图片按钮(右键菜单)
    QAction *m_resetAction{nullptr};    //还原原来的图片状态按钮(右键菜单)

    QAction *m_oldFilter{nullptr};    //老照片滤镜(右键菜单)
    QAction *m_beepFilter{nullptr};    //磨皮滤镜(右键菜单)
    QAction *m_warnFilter{nullptr};    //暖色滤镜(右键菜单)
    QAction *m_coolFilter{nullptr};    //冷色滤镜(右键菜单)
    QAction *m_grayScaleFilter{nullptr};    //黑白滤镜(右键菜单)
    QAction *m_lightContrastImage{nullptr};    //对比度和亮度(右键菜单)
    QAction *m_inverseColorFilter{nullptr};    //反色滤镜(右键菜单)

    QSlider *m_lightSlider {nullptr};


};

#endif // MAINWIDGET_H
