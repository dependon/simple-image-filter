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
#include "transparencywidget.h"
#include "ui_transparencywidget.h"

TransparencyWidget::TransparencyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransparencyWidget)
{
    ui->setupUi(this);
}

TransparencyWidget::~TransparencyWidget()
{
    delete ui;
}

void TransparencyWidget::on_okBtn_clicked()
{
    emit transparencyChange(ui->spinBox->value());
    QWidget *widget = static_cast<QWidget *>(parent());
    if (widget) {
        widget->close();
    }
}
