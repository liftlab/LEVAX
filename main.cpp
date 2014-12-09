#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Set icon
    a.setWindowIcon(QIcon(":/icons/Images/icon.ico"));

    return a.exec();
}
