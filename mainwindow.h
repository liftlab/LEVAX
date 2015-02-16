/*-----------------------------------------------------------------------------
 * File      - mainwindow.h
 * Author    - LEVAX
 * Purpose   - Declare function and objects for GUI actions.
 *---------------------------------------------------------------------------*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>              /* GUI Window */
#include <QFileDialog>              /* File dialog */
#include <QMessageBox>              /* Help pop-up message box */
#include <QDateTime>                /* Date time */
#include <QTimer>                   /* Splash timer */
#include <QSplashScreen>            /* Splash screen */
#include <QDebug>                   /* Debug, similar to cout. Usage qDebut() << "Hello World" */
#include <QScrollBar>               /* Scroll bar */
#include <QtPrintSupport>           /* Printer support */
#include <QTextStream>              /* Write to file */
#include <QFile>                    /* Write to file */

#include "tinyXML/tinyxml.h"        /* TinyXML XML parser*/
#include "BuildingHandler.h"        /* Building Handler */
#include "LiftHandler.h"            /* Lift Handler */
#include "SimulatedHumanHandler.h"  /* SimulatedHuman Handler */
#include "Algorithm.h"

namespace Ui {
class MainWindow;
}

#define MAX_FLOORS 50               /* Define maximum floor */
#define MAX_LIFTS 4                 /* Define maximum no of lifts */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    /* FILE MENU */
    void uploadHumanXML();                              /* humanXMLUploadBtn dialog function */
    void uploadBuildingXML();                           /* buildingXMLUploadBtn dialog function */
    void exportHumanXML();                              /* Export human XML */
    void exportBuildingXML();                           /* Export building XML */
    void onActionHelp();                                /* Handle help button press */
    void printSimulatedData();                          /* Print simulated data */
    void printSimulatedResult();                        /* Print simulated result */

    /* HUMAN MODEL */
    void on_humanTotalSpinBox_valueChanged(int args1);  /* Handle total human spin box value changed */
    void on_humanVisitorSpinBox_valueChanged(int arg1); /* Handle total visitor spin box value changed */

    /* BUILDING MODEL */
    void on_totalFloorSpinBox_valueChanged(int arg1);   /* Handle total number of floor spinbox value changed */
    void on_metreSpinBox_valueChanged(int arg1);        /* Handle metre per floor spinbox value changed */
    void on_liftCombo_currentIndexChanged(int index);   /* Handle lift combo box selection */
    void on_totalLiftsSpinBox_valueChanged(int arg1);   /* Handle total number of lift spin box value changed */
    void on_maxWeightSpinBox_valueChanged(int arg1);    /* Handle lift max weight spinbox value changed */
    void on_maxSpeedSpinBox_valueChanged(int arg1);     /* Handle lift max speed spin box value changed */
    void on_defaultFloorSpinBox_valueChanged(int arg1); /* Handle lift default floor changes */

    /* BUTTONS */
    void on_resetBtn_clicked();                         /* Handle reset button press */
    void on_runBtn_clicked();                           /* Handle run button press */
    void on_saveResultBtn_clicked();                    /* Handle save result button clicked */
    void on_applySettingBtn_clicked();                  /* Handle apply button clicked, create model */

private:
    Ui::MainWindow *ui;
    BuildingHandler bh;                                 /* Building handler object */
    LiftHandler lh;                                     /* Lift handler object */
    SimulatedHumanHandler shh;                          /* SimulatedHuman handler object */

    int prevTotalHuman;                                 /* Total number of human generated previously */
    int prevTotalVisitor;                               /* Total number of visitor generated previously */
    bool loadBuildingByFile;                            /* Data is loaded by XML file */
    bool loadHumanByFile;                               /* Data is loaded by XML file */

    QString validateBuildingData(const QString &arg1);  /* validate building XML */
    bool validateHumanData(const QString &arg1);        /* validate human XML */
    void updateHumanSummary(bool);                      /* Update summary box */
    void updateBuildingSummary();                       /* Display building summary */
    QString generateSimulationData();                   /* Combine simulated humans and simulated building model */
    QString populateResult(pair<QString, pair<pair<double, vector<int> >, pair<int, int> > >, /* Populate result message */
                           QString);


};

#endif // MAINWINDOW_H
