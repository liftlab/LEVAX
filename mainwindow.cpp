#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Constructor */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Help menu listener */
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(onActionHelp()));
}

/* Destructor */
MainWindow::~MainWindow()
{
    delete ui;
}

/* humanXMLUploadBtn dialog function */
void MainWindow::on_humanXMLUploadBtn_clicked()
{
    /* Prompt dialog at current folder, accepting .xml file only */
    QString filename=QFileDialog::getOpenFileName(
                this,
                tr("Open Simulated Human Data"),
                "./",
                "XML Data File (*.xml);;"

                );

    /* Set directory to label */
    if(!filename.isEmpty())
    {
        ui->humanDirLabel->setDisabled(false);
        ui->humanDirLabel->setText(filename);
        ui->humanTotalSpinBox->setValue(0);
        ui->humanAvgSpinBox->setValue(0);
        ui->humanVisitorSpinBox->setValue(0);
    }
    else
    {
        ui->humanDirLabel->setText("...");
    }
}

/* buildingXMLUploadBtn dialog function */
void MainWindow::on_buildingXMLUploadBtn_clicked()
{
    /* Prompt dialog at current folder, accepting .xml file only */
    QString filename=QFileDialog::getOpenFileName(
                this,
                tr("Open Building Model"),
                "./",
                "XML Data File (*.xml);;"

                );

    /* Set directory to label */
    if(!filename.isEmpty())
        ui->buildingDirLabel->setText(filename);
    else
        ui->buildingDirLabel->setText("...");
}

/* Handle total human spin box value changed */
void MainWindow::on_humanTotalSpinBox_valueChanged(int arg1)
{
    /* reset and disable human directory label */
    if(arg1 != 0)
    {
        ui->humanDirLabel->setText("...");
        ui->humanDirLabel->setDisabled(true);
    }
}

/* Handle average human spin box value changed */
void MainWindow::on_humanAvgSpinBox_valueChanged(int arg1)
{
    /* reset and disable human directory label */
    if(arg1 != 0)
    {
        ui->humanDirLabel->setText("...");
        ui->humanDirLabel->setDisabled(true);
    }
}

/* Handle human visitor spin box value changed */
void MainWindow::on_humanVisitorSpinBox_valueChanged(int arg1)
{
    /* reset and disable human directory label */
    if(arg1 != 0)
    {
        ui->humanDirLabel->setText("...");
        ui->humanDirLabel->setDisabled(true);
    }
}

/* Handle reset button press */
void MainWindow::on_resetBtn_clicked()
{
    /* reset all data to default */
    ui->inputSummaryBox->clear();
    ui->outputBox->clear();
    ui->buildingDirLabel->setText("...");
    ui->humanDirLabel->setText("...");
    ui->humanDirLabel->setDisabled(false);
    ui->algoCombo->setCurrentIndex(0);
    ui->humanTotalSpinBox->setValue(0);
    ui->humanAvgSpinBox->setValue(0);
    ui->humanVisitorSpinBox->setValue(0);
    ui->saveResultBtn->setDisabled(true);
}

/* Handle help button press */
void MainWindow::onActionHelp()
{
    /* Help message */
    QString message = "<b>E.g. of Simulated Human XML</b><br>"
                       "&lt;human&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person id=\"1\" weight=\"60\" resident=\"4\"&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor travel=\"4\"/&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor travel=\"20\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;/person&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person id=\"2\" weight=\"78\" resident=\"27\"&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor travel=\"27\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;/person&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person id=\"3\" weight=\"69\" resident=\"-1\"&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor travel=\"27\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;/person&gt;<br>"
                        "&lt;/human&gt;<br><br>"
                        "-Person 1<br>"
                        "Weighs 60kg, living at 4th floor.<br>"
                        "He usually travels to 4th floor and 20th floor.<br>"
                        "-Person 2<br>"
                        "Weighs 78kg, living at 27th floor.<br>"
                        "He usually travels to 27th floor.<br>"
                        "-Person 3<br>"
                        "Weighs 69kg, not a resident here.<br>"
                        "He usually travels to 27th floor.<br><br>"
                        "<b>E.g. of Building XML</b><br>"
                        "&lt;building floors=\"28\" metrePerFloor=\"3\" householdPerFloor=\"4\"&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;lift id=\"1\" maxWeight=\"400\" speed=\"1\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;lift id=\"2\" maxWeight=\"340\" speed=\"1\"/&gt;<br>"
                        "&lt;/building&gt;<br><br>"
                        "Building is 28 stories high with 4 household per floor<br>"
                        "Each level is 3metres apart. It has two lift<br>"
                        "-Lift 1<br>"
                        "Maximum weight of 400kg and travels 1 floor per sec<br>"
                        "-Lift 2<br>"
                        "Maximum weight of 340kg and travels 1 floor per sec<br>";

    /* Pop-up message box */
    QMessageBox::information(this,tr("Help"),message);
}

/* Handle run button press */
void MainWindow::on_runBtn_clicked()
{
    /* Input manually */
    if(!ui->humanDirLabel->isEnabled()
       && (ui->humanAvgSpinBox->value() == 0 || ui->humanTotalSpinBox->value() == 0 || ui->buildingDirLabel->text() == "..."))
    {

        QMessageBox::warning(this,tr("Form incomplete"),"Please fill in all necessary fields");

    } else if(ui->humanDirLabel->isEnabled() /* Input by files */
              && (ui->humanDirLabel->text() == "..." || ui->buildingDirLabel->text() == "..."))
    {

        QMessageBox::warning(this,tr("Form incomplete"),"Please fill in all necessary fields");

    }
    else /* All fields are set */
    {
        /* Output message */
        QString message = "Simulation completed in 1.3s<br>"
                        ".<br>"
                        "..<br>"
                        "...<br>"
                        "....<br>"
                        ".....<br>"
                        "......<br>"
                        ".......<br>"
                        "........<br>"
                        ".........<br>"
                        "..........<br>"
                        "...........<br>";

        /* Set output message */
        ui->outputBox->setText(message);
        ui->saveResultBtn->setDisabled(false);
    }
}

/* Handle save result button clicked */
void MainWindow::on_saveResultBtn_clicked()
{
    /* Initialize current date time */
    QDateTime local(QDateTime::currentDateTime());

    /* Display QFileDialog prompt */
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save File"), "Result_"+local.toString("dMMM_HHmmap")+".txt",
                tr("Text Documents (*.txt);;"));


    /* Save and display success */
    if(!fileName.isEmpty())
        QMessageBox::information(this,tr("Help"),"File saved to "+fileName);
}
