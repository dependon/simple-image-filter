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
#ifndef TRANSPARENCYWIDGET_H
#define TRANSPARENCYWIDGET_H

#include <QWidget>

namespace Ui {
class TransparencyWidget;
}

class TransparencyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransparencyWidget(QWidget *parent = nullptr);
    ~TransparencyWidget();
signals:
    void transparencyChange(const int &index);
private slots:
    void on_okBtn_clicked();

private:
    Ui::TransparencyWidget *ui;
};

#endif // TRANSPARENCYWIDGET_H
