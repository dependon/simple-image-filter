#include "application.h"
#include <QDebug>
Application::Application(int &argc, char **argv) :
    AppS(argc, argv)
{
    this->setApplicationName(tr("simple-image-filter"));
    this->setApplicationDisplayName(tr("simple-image-filter"));
    this->setApplicationDescription(
        QObject::tr(
            "<a href='https://github.com/dependon/simple-image-filter/'>github/simple-image-filter</a><br/>"
            "<span style='font-size:12pt;'>mail: liuminghang0821@gmail.com</span><br/><br/>"
            "Published under GPL V3"
        )
    );
    setApplicationVersion(DApplication::buildVersion("1.0.0"));
}
