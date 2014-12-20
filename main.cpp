/*-----------------------------------------------------------------------------
 * File      - main.cpp
 * Author    - LEVAX
 * Purpose   - Main application. Display splash and GUI
 *---------------------------------------------------------------------------*/

#include "mainwindow.h"
#include <ctime>
#include <QApplication>

/* main program */
int main(int argc, char *argv[])
{
    /* Random Seed */
    srand(time(NULL));

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
