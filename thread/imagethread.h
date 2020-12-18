#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H

#include "imageapi.h"
#include "application.h"

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QRunnable>


enum MenuItemId {
    IdNormal,
    IdBEEP,
    IdWarn,
    IdCool,
    IdGrayScale,
    IdInverseColor,
    Idold,
    IdlightContrast,

};
struct ImageFilterInfo{
    MenuItemId id;
    double spatialDecay{0.02};
    double photometricStandardDeviation{10};
    int warnImageDecay{30};
    int coolImageDecay{30};
    int lightDecay{100};
    int ContrastDecay{100};
};

class ImageRunnable :public QObject, public QRunnable
{

    Q_OBJECT
public :
    ImageRunnable();
    ~ImageRunnable();
    void setData(QImage *img,ImageFilterInfo info);
protected:
    virtual void run();
private:
    ImageFilterInfo m_info;
    QImage *m_img {nullptr};
};
#endif // IMAGETHREAD_H
