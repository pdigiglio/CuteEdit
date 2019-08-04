#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("CuteEdit");
    QCoreApplication::setOrganizationName("FreeSoft");
    QCoreApplication::setOrganizationDomain("freesoft.com");

    MainWindow mainWindow;
    mainWindow.loadNew();
    mainWindow.show();

    //QLabel label("Ciao");
    //label.show();

    return a.exec();
}
