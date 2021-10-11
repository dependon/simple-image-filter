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
#include "imagethread.h"
#include <QThread>


ImageRunnable::ImageRunnable()
{
    setAutoDelete(true);
}

ImageRunnable::~ImageRunnable()
{

}

void ImageRunnable::setData(QImage &img, ImageFilterInfo info)
{
    m_info = info;
    m_iimg = img;
}

void ImageRunnable::run()
{
    switch (m_info.id) {
    case MenuItemId::IdNormal:
        break;
    case MenuItemId::Idold:
        m_iimg = QImageAPI::oldImage(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdBEEP:
        m_iimg = QImageAPI::QImageD_RunBEEPSHorizontalVertical(m_iimg, m_info.spatialDecay, m_info.photometricStandardDeviation);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdCool:
        m_iimg = QImageAPI::coolImage(m_iimg, m_info.coolImageDecay);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdWarn:
        m_iimg = QImageAPI::warnImage(m_iimg, m_info.warnImageDecay);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdGrayScale:
        m_iimg = QImageAPI::GrayScaleImage(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdInverseColor:
        m_iimg = QImageAPI::InverseColorImage(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdlightContrast:
        m_iimg = QImageAPI::lightContrastImage(m_iimg, m_info.lightDecay, m_info.ContrastDecay);
        App->sigFilterImage(m_iimg, UnChange);
        break;
    case MenuItemId::IdLaplaceSharpen:
        m_iimg = QImageAPI::LaplaceSharpen(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdSobel:
        m_iimg = QImageAPI::SobelEdge(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdHorizontal:
        m_iimg = QImageAPI::Horizontal(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdVertical:
        m_iimg = QImageAPI::Vertical(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdMetal:
        m_iimg = QImageAPI::Metal(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdContourExtraction:
        m_iimg = QImageAPI::ContourExtraction(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdBinaryzation:
        m_iimg = QImageAPI::Binaryzation(m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdTransparency:
        m_iimg = QImageAPI::transparencyImg(m_info.transparency, m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    default:
        break;

    }
}
