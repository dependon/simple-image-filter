#include "imagethread.h"
#include <QThread>


ImageRunnable::ImageRunnable()
{
    setAutoDelete(true);
}

ImageRunnable::~ImageRunnable()
{

}

//void ImageRunnable::setData(QImage *img, ImageFilterInfo info)
//{
//    m_info=info;
//    m_img=img;
//}
void ImageRunnable::setData(QImage &img, ImageFilterInfo info)
{
    m_info=info;
    m_iimg=img;
}

void ImageRunnable::run()
{
    switch (m_info.id) {
    case MenuItemId::IdNormal:
        break;
    case MenuItemId::Idold:
        QImageAPI::oldImage(&m_iimg,&m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdBEEP:
        QImageAPI::QImageD_RunBEEPSHorizontalVertical(&m_iimg,&m_iimg,m_info.spatialDecay,m_info.photometricStandardDeviation);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdCool:
        QImageAPI::coolImage(&m_iimg,&m_iimg,m_info.coolImageDecay);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdWarn:
        QImageAPI::warnImage(&m_iimg,&m_iimg,m_info.warnImageDecay);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdGrayScale:
        QImageAPI::GrayScaleImage(&m_iimg,&m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdInverseColor:
        QImageAPI::InverseColorImage(&m_iimg,&m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    case MenuItemId::IdlightContrast:
        QImageAPI::lightContrastImage(&m_iimg,&m_iimg,m_info.lightDecay,m_info.ContrastDecay);
        App->sigFilterImage(m_iimg,UnChange);
        break;
    default:
        break;

    }
}
