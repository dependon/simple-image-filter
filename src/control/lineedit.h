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
#ifndef LINEEDIT_H
#define LINEEDIT_H

#ifdef USE_DTK
#include <DLineEdit>
DWIDGET_USE_NAMESPACE
typedef  DLineEdit MyLineEdit;
#else
#include <QLineEdit>
typedef QLineEdit MyLineEdit;
#endif

class LineEdit: public MyLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget *parent = nullptr);

};

#endif // LINEEDIT_H
