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
#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H

#ifdef USE_DTK
#include <DFloatingWidget>
DWIDGET_USE_NAMESPACE
typedef  DFloatingWidget FloatWidget;
#else
#include <QWidget>
typedef QWidget FloatWidget;
#endif
#include <QDragEnterEvent>
class StatusBarWidget : public FloatWidget
{
    Q_OBJECT
public:
    explicit StatusBarWidget(QWidget *parent = nullptr);

signals:

public slots:

};

#endif // STATUSBARWIDGET_H
