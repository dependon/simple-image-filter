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
#ifndef APPLICATION_H
#define APPLICATION_H
#include "mainwidget.h"
#include <QCoreApplication>
enum MenuItemId {
    IdNormal,
    IdBEEP,
    IdWarn,
    IdCool,
    IdGrayScale,
    IdInverseColor,
    Idold,
    IdlightContrast,
    IdLaplaceSharpen,
    IdSobel,
    IdHorizontal,
    IdVertical,
    IdContourExtraction,
    IdBinaryzation,
    IdMetal,
    IdTransparency
};
enum isChange {
    Change,
    UnChange,
};

#ifdef USE_DTK
#include <DApplication>
DWIDGET_USE_NAMESPACE
typedef  DApplication AppS;
#else
#include <QApplication>
typedef QApplication AppS;
#endif

#define App (static_cast<Application*>(QCoreApplication::instance()))
class Application : public AppS
{
    Q_OBJECT
public:
    Application(int &argc, char **argv);

signals:
    //设置栈窗口(0为打开的初始界面,1为图像显示界面)
    void setStackWidget(const int &index);
    //图形处理信号，用于线程处理完毕发送给UI显示
//    void sigFilterImage(QImage *);
    void sigFilterImage(QImage, isChange is = Change);
    //鼠标移动信号，由于在view里面鼠标移动后，事件没有渗透到主窗口，特此写了一个信号
    void sigMouseMove();

    //重置亮度和对比度
    void sigResetLightContrast();
private:

};

#endif // APPLICATION_H
