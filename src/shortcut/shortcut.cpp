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
#include "shortcut.h"

namespace {

const QString VIEW_GROUP = "SHORTCUTVIEW";
const QString ALBUM_GROUP = "SHORTCUTALBUM";


}  // namespace

Shortcut::Shortcut(QObject *parent)
    : QObject(parent)
{
    ShortcutGroup group1;
    ShortcutGroup group2;
    ShortcutGroup group3;

    group1.groupName = QObject::tr("Simple Image Filter");

    group3.groupName = QObject::tr("Settings");
    //整理代码结构，解决显示界面出现和快捷键不一致问题，是由于配置文件导致，但配置文件无效，改为不使用配置文件
    group1.groupItems << ShortcutItem(QObject::tr("Open"),  "Ctrl+O")
                      << ShortcutItem(QObject::tr("Reset"), "Ctrl+E")
                      << ShortcutItem(QObject::tr("FitImage"), "Ctrl+R")
                      << ShortcutItem(QObject::tr("FitWindow"), "Ctrl+T")
                      << ShortcutItem(QObject::tr("Rotate-90"), "Ctrl+Left")
                      << ShortcutItem(QObject::tr("Rotate+90"), "Ctrl+Right")
                      << ShortcutItem(QObject::tr("Save"), "Ctrl+S")
                      << ShortcutItem(QObject::tr("ScaleImage"), "Ctrl+Y")
                      << ShortcutItem(QObject::tr("Zoom in"), "Ctrl+ '+'")
                      << ShortcutItem(QObject::tr("Zoom out"), "Ctrl+ '-'")
                      << ShortcutItem(QObject::tr("Copy"), "Ctrl+C")
                      << ShortcutItem(QObject::tr("Withdrawal step"), "Ctrl+Z");

    group3.groupItems << ShortcutItem(QObject::tr("Help"), "F2")
                      << ShortcutItem(QObject::tr("Display shortcuts"), "Ctrl + Shift + ?");

#ifndef LITE_DIV
    m_shortcutGroups << group1 << group2 << group3;
#else
    m_shortcutGroups << group1 << group3;
#endif

    // convert to json object
    QJsonArray jsonGroups;
    for (auto scg : m_shortcutGroups) {
        QJsonObject jsonGroup;
        jsonGroup.insert("groupName", scg.groupName);
        QJsonArray jsonGroupItems;
        for (auto sci : scg.groupItems) {
            QJsonObject jsonItem;
            jsonItem.insert("name", sci.name);
            jsonItem.insert("value", sci.value);
            jsonGroupItems.append(jsonItem);
        }
        jsonGroup.insert("groupItems", jsonGroupItems);
        jsonGroups.append(jsonGroup);
    }
    m_shortcutObj.insert("shortcut", jsonGroups);
}
QString Shortcut::toStr()
{
    QJsonDocument doc(m_shortcutObj);
    return doc.toJson().data();
}
