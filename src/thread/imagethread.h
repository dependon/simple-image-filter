/*
 *图像数据处理线程，通过创建线程池，对线程进行调用，数据通过全局信号发回图像显示界面
*/
#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H

#include "imageapi.h"
#include "application.h"

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QRunnable>

struct ImageFilterInfo {
    MenuItemId id;
    double spatialDecay{0.02};
    double photometricStandardDeviation{10};
    int warnImageDecay{30};
    int coolImageDecay{30};
    int lightDecay{100};
    int ContrastDecay{100};
    int transparency{255};
};

class ImageRunnable : public QObject, public QRunnable
{

    Q_OBJECT
public :
    ImageRunnable();
    ~ImageRunnable();
    //设置数据
    void setData(QImage &img, ImageFilterInfo info);
protected:
    virtual void run();
private:
    ImageFilterInfo m_info;
    QImage *m_img {nullptr};
    QImage m_iimg{nullptr};
};
#endif // IMAGETHREAD_H
