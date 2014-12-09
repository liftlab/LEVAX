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
        ui->humanDirLabel->setText(filename);
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
