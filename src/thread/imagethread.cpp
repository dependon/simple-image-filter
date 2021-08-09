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
    case MenuItemId::IdTransparency:
        m_iimg = QImageAPI::transparencyImg(m_info.transparency, m_iimg);
        App->sigFilterImage(m_iimg);
        break;
    default:
        break;

    }
}
