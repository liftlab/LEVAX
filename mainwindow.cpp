#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Constructor */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
