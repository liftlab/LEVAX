#include "mainwindow.h"
#include <QApplication>

/* main program */
int main(int argc, char *argv[])
{
    /* Initialize QApplication */
    QApplication a(argc, argv);

    /* Display Splash scren */
    QSplashScreen *splash=new QSplashScreen;
    splash->setPixmap(QPixmap(":/Images/Images/splash.jpg"));
    splash->show();

    /* Initialize GUI Window */
    MainWindow w;

    /* Close splash in 2.5seconds and open GUI Window */
    QTimer::singleShot(2500,splash,SLOT(close()));
    QTimer::singleShot(2500,&w,SLOT(show()));

    // Set icon
    a.setWindowIcon(QIcon(":/icons/Images/icon.ico"));

    return a.exec();
}
