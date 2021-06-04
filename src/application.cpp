#include "application.h"
#include <QDebug>
Application::Application(int &argc, char **argv) :
    AppS(argc, argv)
{
    this->setApplicationName(tr("simple-image-filter"));
    this->setApplicationDisplayName(tr("simple-image-filter"));

}
