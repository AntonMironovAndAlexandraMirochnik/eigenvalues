#include "controller/main_controller.h"

int main(int argc, char** argv)
{
    qsrand(QTime::currentTime().msec());
    QApplication *application;
    MainController *controller;

    //initialization of application
    {
        application = new QApplication(argc, argv);
        application->setApplicationName("Eigenvalues");
        application->setApplicationVersion(APPLICATION_VERSION);
        application->setWindowIcon(QIcon(":/images/logo"));
    }

    //initialization of main controller
    {
        controller = new MainController(application);
    }

    while (QThreadPool::globalInstance()->maxThreadCount() < QThread::idealThreadCount())
    {
        QThreadPool::globalInstance()->reserveThread();
    }

    controller->window()->show();

    return application->exec();
}
