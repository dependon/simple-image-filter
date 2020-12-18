#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class Menu;
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
    bool Init();
    void initBtn();
    void initConnect();
    void initMenu();
    bool Uninit();
    void openImage(const QString& path);

    ~MainWidget();
protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void openImageFile();
    void setCurrentWidget(const int& index);
    void on_openPicture_clicked();

private:
    Ui::MainWidget *ui;
    QString m_currentPath;
    QImage *m_currentImage{nullptr};
    QImage *m_FilterImage{nullptr};
    StatusBarWidget *m_statusbarWidget{nullptr};

    PushButton *m_openBtn{nullptr};
    PushButton *m_fitImageBtn{nullptr};
    PushButton *m_fitWindowBtn{nullptr};
    PushButton *m_rotateRight{nullptr};
    PushButton *m_rotateLeft{nullptr};
    PushButton *m_saveBtn{nullptr};
    PushButton *m_resetBtn{nullptr};

    Menu *m_leftMenu{nullptr};

    QAction *m_openAction{nullptr};
    QAction *m_fitImageAction{nullptr};
    QAction *m_fitWindowAction{nullptr};
    QAction *m_rotateRightAction{nullptr};
    QAction *m_rotateLeftAction{nullptr};
    QAction *m_saveBAction{nullptr};
    QAction *m_resetAction{nullptr};

    QAction *m_oldFilter{nullptr};
    QAction *m_beepFilter{nullptr};
    QAction *m_warnFilter{nullptr};
    QAction *m_coolFilter{nullptr};
    QAction *m_grayScaleFilter{nullptr};
    QAction *m_lightContrastImage{nullptr};
    QAction *m_inverseColorFilter{nullptr};


};

#endif // MAINWIDGET_H
