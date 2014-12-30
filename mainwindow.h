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

#include "tinyXML/tinyxml.h"        /* TinyXML XML parser*/
#include "BuildingHandler.h"        /* Building Handler */
#include "LiftHandler.h"            /* Lift Handler */
#include "SimulatedHumanHandler.h"  /* SimulatedHuman Handler */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void uploadHumanXML();                              /* humanXMLUploadBtn dialog function */
    void uploadBuildingXML();                           /* buildingXMLUploadBtn dialog function */
    void on_humanTotalSpinBox_valueChanged(int args1);  /* Handle total human spin box value changed */
    void on_humanVisitorSpinBox_valueChanged(int arg1); /* Handle human visitor spin box value changed */
    void on_resetBtn_clicked();                         /* Handle reset button press */
    void onActionHelp();                                /* Handle help button press */
    void on_runBtn_clicked();                           /* Handle run button press */
    void on_saveResultBtn_clicked();                    /* Handle save result button clicked */
    void on_liftCombo_currentIndexChanged(int index);   /* Handle lift combo box selection */
    void on_totalFloorSpinBox_valueChanged(int arg1);   /* Handle total floor spin box value changed */
    void on_metreSpinBox_valueChanged(int arg1);        /* Handle metre per floor spin box value changed */
    void on_totalLiftsSpinBox_valueChanged(int arg1);   /* Handle total number of lift spin box value changed */
    void on_maxWeightSpinBox_valueChanged(int arg1);    /* Handle lift max weight spinbox value changed */
    void on_maxSpeedDoubleSpinBox_valueChanged(double arg1); /* Handle lift max speed spin box value changed */

    void exportBuildingXML();                           /* Export building XML */
    void exportHumanXML();                              /* Export human XML */

    /* FOR DEBUGGING PURPOSES ONLY */
    void onActionCheckObj();                            /* Check current created object */

private:
    Ui::MainWindow *ui;
    BuildingHandler bh;                                 /* Building handler object */
    LiftHandler lh;                                     /* Lift handler object */
    SimulatedHumanHandler shh;                          /* SimulatedHuman handler object */

    int prevTotalHuman;                                 /* Total number of human generated previously */
    int prevTotalVisitor;                               /* Total number of visitor generated previously */
    bool loadBuildingByFile;                            /* Data is loaded by XML file */
    bool loadHumanByFile;                               /* Data is loaded by XML file */

    bool updateChanges;

    QString validateBuildingData(const QString &arg1);  /* validate building XML */
    bool validateHumanData(const QString &arg1);        /* validate human XML */
    void updatePeopleSummary(bool);                     /* Update summary box */
    void updateBuildingSummary();                       /* Display building summary */
};

#endif // MAINWINDOW_H
