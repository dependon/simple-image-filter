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
#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#ifdef USE_DTK
#include <DPushButton>
DWIDGET_USE_NAMESPACE
typedef  DPushButton PushBtn;
#else
#include <QPushButton>
typedef QPushButton PushBtn;
#endif

class PushButton: public PushBtn
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = nullptr);

    explicit PushButton(const QString &text, QWidget *parent = nullptr);
};

#endif // ICONBUTTON_H
