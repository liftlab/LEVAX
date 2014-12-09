#include "mainwindow.h"
#include <QApplication>

/* main program */
int main(int argc, char *argv[])
{
    /* initialize QApplication and display MainWindow GUI */
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Set icon
    a.setWindowIcon(QIcon(":/icons/Images/icon.ico"));

    return a.exec();
}
