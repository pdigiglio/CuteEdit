#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
    mainWindow.loadNew();
    mainWindow.show();

    //QLabel label("Ciao");
    //label.show();

    return a.exec();
}
