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
