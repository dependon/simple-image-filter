#include "imagethread.h"
#include <QThread>


ImageRunnable::ImageRunnable()
{
    setAutoDelete(true);
}

ImageRunnable::~ImageRunnable()
{

}

void ImageRunnable::setData(QImage *img, ImageFilterInfo info)
{
    m_info=info;
    m_img=img;
}

void ImageRunnable::run()
{
    switch (m_info.id) {
    case MenuItemId::IdNormal:
        break;
    case MenuItemId::Idold:
        QImageAPI::oldImage(m_img,m_img);
        App->sigFilterImage(m_img);
        break;
    case MenuItemId::IdBEEP:
        QImageAPI::QImageD_RunBEEPSHorizontalVertical(m_img,m_img,m_info.spatialDecay,m_info.photometricStandardDeviation);
        App->sigFilterImage(m_img);
        break;
    case MenuItemId::IdCool:
        QImageAPI::coolImage(m_img,m_img,m_info.coolImageDecay);
        App->sigFilterImage(m_img);
        break;
    case MenuItemId::IdWarn:
        QImageAPI::warnImage(m_img,m_img,m_info.warnImageDecay);
        App->sigFilterImage(m_img);
        break;
    case MenuItemId::IdGrayScale:
        QImageAPI::GrayScaleImage(m_img,m_img);
        App->sigFilterImage(m_img);
        break;
    case MenuItemId::IdInverseColor:
        QImageAPI::InverseColorImage(m_img,m_img);
        App->sigFilterImage(m_img);
        break;
    case MenuItemId::IdlightContrast:
        break;
    default:
        break;

    }
}
