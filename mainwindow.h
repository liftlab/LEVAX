#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>  /* File dialog */
#include <QMessageBox>  /* Help pop-up message box */
#include <QDateTime>    /* Date time */
#include <QDebug>       /* Debug, similar to cout. Usage qDebut() << "Hello World" */

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
    void on_humanXMLUploadBtn_clicked();    /* humanXMLUploadBtn dialog function */
    void on_buildingXMLUploadBtn_clicked(); /* buildingXMLUploadBtn dialog function */
    void on_humanTotalSpinBox_valueChanged(int args1);  /* Handle total human spin box value changed */
    void on_humanAvgSpinBox_valueChanged(int arg1);     /* Handle average human spin box value changed */
    void on_humanVisitorSpinBox_valueChanged(int arg1); /* Handle human visitor spin box value changed */
    void on_resetBtn_clicked();             /* Handle reset button press */
    void onActionHelp();                    /* Handle help button press */
    void on_runBtn_clicked();               /* Handle run button press */
    void on_saveResultBtn_clicked();        /* Handle save result button clicked */

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
