/*-----------------------------------------------------------------------------
 * File      - mainwindow.cpp
 * Author    - LEVAX
 * Purpose   - Handles GUI interaction and element data retrieval
 *---------------------------------------------------------------------------*/

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
    if(ui->buildingDirLabel->text() != "...")
    {
        /* Prompt dialog at current folder, accepting .xml file only */
        QFileDialog dlg(NULL, tr("Open Simulated Human Data"));
        dlg.setNameFilter(tr("XML Data File (*.xml)"));
        QString filename;

        /* Retrieve previous filename */
        QString prevFilename = ui->humanDirLabel->text();

        /* Check for Open button pressed */
        if(dlg.exec())
        {

            /* Get directory */
            filename = dlg.selectedFiles().at(0);

            /* Validate data */
            QString summary = validateHumanData(filename);

            /* Check if human data is invalid */
            if(summary == "false")
            {
                /* Undo if previous data is loaded */
                if(prevFilename != "...")
                    ui->inputSummaryBox->undo();

                ui->humanDirLabel->setText("...");

                /* Display error */
                QMessageBox::critical(this,tr("Error!"),"The XML file selected has errors in them<br>See Help to learn more about XML structures");
            }
            else
            {
                /* Undo if previous data is loaded */
                if(prevFilename != "...")
                    ui->inputSummaryBox->undo();

                /* Header */
                QString header = "<b><font size=\"4\">Simulated Human Data</font></b><br>"
                                 "====================<br>";
                header += summary;

                /* Display to inputSummaryBox */
                ui->inputSummaryBox->append(header);

                /* Enable simulated human elements */
                ui->humanDirLabel->setDisabled(false);
                ui->humanDirLabel->setText(filename);
                ui->humanTotalSpinBox->setValue(0);
                ui->humanAvgSpinBox->setValue(0);
                ui->humanVisitorSpinBox->setValue(0);
            }
        }
    }
    else
    {
        /* Display warning message to load building XML first */
        QMessageBox::information(this,tr("Notice"),"Please load building XML first");
    }
}

/* buildingXMLUploadBtn dialog function */
void MainWindow::on_buildingXMLUploadBtn_clicked()
{
    /* Prompt dialog at current folder, accepting .xml file only */
    QFileDialog dlg(NULL, tr("Open Building Model"));
    dlg.setNameFilter(tr("XML Data File (*.xml)"));
    QString filename;

    /* Check for Open button pressed */
    if(dlg.exec())
    {
        /* Get directory */
        filename = dlg.selectedFiles().at(0);

        /* Clear inputSummaryBox */
        ui->inputSummaryBox->clear();

        /* Validate data */
        QString summary = validateBuildingData(filename);

        /* Check if building data is invalid */
        if(summary == "false")
        {
            /* Reset all elements */
            ui->buildingDirLabel->setText("...");
            ui->humanDirLabel->setText("...");
            ui->humanDirLabel->setDisabled(true);
            ui->humanTotalSpinBox->setDisabled(true);
            ui->humanAvgSpinBox->setDisabled(true);
            ui->humanVisitorSpinBox->setDisabled(true);
            ui->humanTotalSpinBox->setValue(0);
            ui->humanAvgSpinBox->setValue(0);
            ui->humanVisitorSpinBox->setValue(0);

            /* reset all building data due to error */
            bh.resetAll();

            /* Display error */
            QMessageBox::critical(this,tr("Error!"),"The XML file selected has errors in them<br>See Help to learn more about XML structures");
        }
        else
        {
            /* header */
            QString header = "<b><font size=\"4\">Building Data</font></b><br>"
                              "====================<br>";
            header += summary;

            /* Display to inputSummaryBox */
            ui->inputSummaryBox->append(header);

            /* Set elements to enable */
            ui->buildingDirLabel->setText(filename);
            ui->humanDirLabel->setDisabled(false);
            ui->humanTotalSpinBox->setDisabled(false);
            ui->humanAvgSpinBox->setDisabled(false);
            ui->humanVisitorSpinBox->setDisabled(false);
            ui->humanDirLabel->setText("...");
            ui->humanTotalSpinBox->setValue(0);
            ui->humanAvgSpinBox->setValue(0);
            ui->humanVisitorSpinBox->setValue(0);
        }
    }
}

/* Handle total human spin box value changed */
void MainWindow::on_humanTotalSpinBox_valueChanged(int arg1)
{
    /* Get previous human file name previous human label enabled status */
    bool prevHumanLabelStatus = ui->humanDirLabel->isEnabled();
    QString prevFilename = ui->humanDirLabel->text();

    /* reset and disable human directory label */
    if(arg1 != 0)
    {
        /* Undo if previous data is loaded */
        if(prevHumanLabelStatus == true && prevFilename != "...")
            ui->inputSummaryBox->undo();

        ui->humanDirLabel->setText("...");
        ui->humanDirLabel->setDisabled(true);
    }
}

/* Handle average human spin box value changed */
void MainWindow::on_humanAvgSpinBox_valueChanged(int arg1)
{
    /* Get previous human file name previous human label enabled status */
    bool prevHumanLabelStatus = ui->humanDirLabel->isEnabled();
    QString prevFilename = ui->humanDirLabel->text();

    /* reset and disable human directory label */
    if(arg1 != 0)
    {
        /* Undo if previous data is loaded */
        if(prevHumanLabelStatus == true && prevFilename != "...")
            ui->inputSummaryBox->undo();

        ui->humanDirLabel->setText("...");
        ui->humanDirLabel->setDisabled(true);
    }
}

/* Handle human visitor spin box value changed */
void MainWindow::on_humanVisitorSpinBox_valueChanged(int arg1)
{
    /* Get previous human file name previous human label enabled status */
    bool prevHumanLabelStatus = ui->humanDirLabel->isEnabled();
    QString prevFilename = ui->humanDirLabel->text();

    /* reset and disable human directory label */
    if(arg1 != 0)
    {
        /* Undo if previous data is loaded */
        if(prevHumanLabelStatus == true && prevFilename != "...")
            ui->inputSummaryBox->undo();

        ui->humanDirLabel->setText("...");
        ui->humanDirLabel->setDisabled(true);
    }
}

/* Handle reset button press */
void MainWindow::on_resetBtn_clicked()
{
    /* reset all data to default */
    ui->humanDirLabel->setDisabled(true);
    ui->inputSummaryBox->clear();
    ui->outputBox->clear();
    ui->buildingDirLabel->setText("...");
    ui->humanDirLabel->setText("...");
    ui->algoCombo->setCurrentIndex(0);
    ui->humanTotalSpinBox->setValue(0);
    ui->humanAvgSpinBox->setValue(0);
    ui->humanVisitorSpinBox->setValue(0);
    ui->saveResultBtn->setDisabled(true);
    ui->humanTotalSpinBox->setDisabled(true);
    ui->humanAvgSpinBox->setDisabled(true);
    ui->humanVisitorSpinBox->setDisabled(true);

    /* reset building data */
    bh.resetAll();
}

/* Handle help button press */
void MainWindow::onActionHelp()
{
    /* Help message to describe XML data structure */
    QString message = "<b>E.g. of Simulated Human XML</b><br>"
                       "&lt;human&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person weight=\"60\" resident=\"4\"&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor travel=\"4\"/&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor travel=\"20\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;/person&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person weight=\"78\" resident=\"27\"&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor travel=\"27\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;/person&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person weight=\"69\" resident=\"0\"&gt;<br>"
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
                        "Weighs 69kg, not a resident here.(resident=0)<br>"
                        "He usually travels to 27th floor.<br><br>"
                        "<b>E.g. of Building XML</b><br>"
                        "&lt;building floors=\"28\" metrePerFloor=\"3\" householdPerFloor=\"4\"&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;lift maxWeight=\"400\" speed=\"1\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;lift maxWeight=\"340\" speed=\"1\"/&gt;<br>"
                        "&lt;/building&gt;<br><br>"
                        "Building is 28 stories high with 4 household per floor<br>"
                        "Each level is 3 metres apart. It has two lift<br>"
                        "-Lift 1<br>"
                        "Maximum weight of 400kg and travels 1 second per floor<br>"
                        "-Lift 2<br>"
                        "Maximum weight of 340kg and travels 1 second per floor<br>";

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

/* validate building XML */
QString MainWindow::validateBuildingData(const QString &arg1)
{
    /* Stores file data */
    QString summary;

    /* load XML */
    TiXmlDocument doc(arg1.toStdString().c_str());

    /* Check if file can be open */
    if(doc.LoadFile())
    {
        TiXmlElement *pRoot, *pParm;

        /* checks if building exist */
        pRoot = doc.FirstChildElement("building");
        if(pRoot)
        {
            /* check if floors, metrePerFloor, householdPerFloor exist */
            if(pRoot->Attribute("floors") != NULL && pRoot->Attribute("metrePerFloor") != NULL && pRoot->Attribute("householdPerFloor") != NULL)
            {
                /* checks for floors, metrePerFloor, householdPerFloor is a number/empty data */
                int attr;
                if(TIXML_SUCCESS != pRoot->QueryIntAttribute("floors", &attr) || TIXML_SUCCESS != pRoot->QueryIntAttribute("metrePerFloor", &attr) || TIXML_SUCCESS != pRoot->QueryIntAttribute("householdPerFloor", &attr))
                    return "false";

                /* Floors must be at least 2 level */
                pRoot->QueryIntAttribute("floors", &attr);
                if(attr < 2)
                {
                    return "false";
                }
                else
                {
                    /* Append data to QString */
                    summary += "Total number of floors ";
                    summary += pRoot->Attribute("floors");
                    summary += "<br>";

                    /* set total number of floors */
                    bh.setNoOfFloors(attr);
                }

                /* metrePerFloor must be at least 3m*/
                pRoot->QueryIntAttribute("metrePerFloor", &attr);
                if(attr < 3)
                {
                    return "false";
                }
                else
                {
                    /* Append data to QString */
                    summary += "Number of metres per floor ";
                    summary += pRoot->Attribute("metrePerFloor");
                    summary += "<br>";

                    /* set metre per floor */
                    bh.setMetrePerFloor(attr);
                }

                /* householdPerFloor must be at least 1 or more*/
                pRoot->QueryIntAttribute("householdPerFloor", &attr);
                if(attr < 1)
                {
                    return "false";
                }
                else
                {
                    /* Append data to QString */
                    summary += "Number of household per floor ";
                    summary += pRoot->Attribute("householdPerFloor");
                    summary += "<br>";

                    /* set number of household per floor */
                    bh.setHouseholdPerFloor(attr);
                }

                /* check if first element of building exist */
                pParm = pRoot->FirstChildElement();
                if(pParm)
                {
                    int liftCount = 0;
                    while(pParm)
                    {
                        if(strcmp(pParm->Value(),"lift") == 0)
                        {
                            liftCount++;

                            /* check if maxWeight, speed exist */
                            if(pParm->Attribute("maxWeight") == NULL || pParm->Attribute("speed") == NULL)
                                return "false";
                            else
                            {
                                /* checks for maxWeight, speed is a number/empty data */
                                int attr2;
                                if(TIXML_SUCCESS != pParm->QueryIntAttribute("maxWeight", &attr2) || TIXML_SUCCESS != pParm->QueryIntAttribute("speed", &attr2))
                                    return "false";

                                /* maxWeight must be at least 200kg */
                                pParm->QueryIntAttribute("maxWeight", &attr2);
                                if(attr2 < 200)
                                {
                                    return "false";
                                }
                                else
                                {
                                    /* Append data to QString */
                                    summary += "<b>Lift no ";
                                    summary += QString::number(liftCount);
                                    summary += "</b><br>";
                                    summary += "Maximum weight ";
                                    summary += pParm->Attribute("maxWeight");
                                    summary += "kg<br>";
                                }

                                /* speed must be at least 1 or more*/
                                pParm->QueryIntAttribute("speed", &attr2);
                                if(attr2 < 1)
                                {
                                    return "false";
                                }
                                else
                                {
                                    /* Append data to QString */
                                    summary += "Maximum speed ";
                                    summary += pParm->Attribute("speed");
                                    summary += "sec/floor<br>";
                                }

                                pParm = pParm->NextSiblingElement();
                            }
                        }
                        else
                        {
                            return "false";
                        }
                    }
                    /* set number of lifts */
                    bh.setNoOfLifts(liftCount);
                }
                else
                {
                    return "false";
                }
            }
            else
            {
                return "false";
            }
        }
        else
        {
            return "false";
        }
    }
    else
    {
        return "false";
    }

    return summary;
}

/* validate human XML */
QString MainWindow::validateHumanData(const QString &arg1)
{
    /* Stores file data */
    QString summary;

    /* Load XML */
    TiXmlDocument doc(arg1.toStdString().c_str());

    /* Check if file can be open */
    if(doc.LoadFile())
    {
        TiXmlElement *pRoot, *pParm, *pParm2;

        /* checks if human exist */
        pRoot = doc.FirstChildElement("human");
        if(pRoot)
        {
            /* Check if first element of human exist */
            pParm = pRoot->FirstChildElement();
            if(pParm)
            {
                int personCount = 0;
                while(pParm)
                {
                    /* is the element named "person" */
                    if(strcmp(pParm->Value(),"person") == 0)
                    {
                        personCount++;

                        /* checks if weight, resident exist */
                        if(pParm->Attribute("weight") != NULL && pParm->Attribute("resident") != NULL)
                        {
                            /* checks for weight, resident is a number/empty data */
                            int attr;
                            if(TIXML_SUCCESS != pParm->QueryIntAttribute("weight", &attr) || TIXML_SUCCESS != pParm->QueryIntAttribute("resident", &attr))
                                return "false";

                            /* person must have a resident value of more than 0 and less than building limit */
                            pParm->QueryIntAttribute("resident", &attr);
                            if((attr < 0) || (attr > bh.getNoOfFloor()))
                            {
                                return "false";
                            }
                            else
                            {
                                /* Append data to QString */
                                summary += "<b>Person number ";
                                summary += QString::number(personCount);
                                if(strcmp(pParm->Attribute("resident"),"0") == 0)
                                {
                                    summary += "</b><br>Is not a resident<br>";
                                }
                                else
                                {
                                    summary += "</b><br>Is a resident staying at level ";
                                    summary += pParm->Attribute("resident");
                                    summary += "<br>";
                                }
                            }

                            /* person must have a weight of more than 0 and 150 or lower */
                            pParm->QueryIntAttribute("weight", &attr);
                            if(attr > 150 || attr < 1)
                            {
                                return "false";
                            }
                            else
                            {
                                /* Append data to QString */
                                summary += "Weighs ";
                                summary += pParm->Attribute("weight");
                                summary += "kg<br>";
                            }

                            /* checks if first element of person exist */
                            pParm2 = pParm->FirstChildElement();
                            if(pParm2)
                            {
                                /* Append data to QString */
                                summary += "Will travel to floor ";

                                while(pParm2)
                                {
                                     /* is the element named "floor" */
                                    if(strcmp(pParm2->Value(),"floor") == 0)
                                    {
                                        /* check if travel exist */
                                        if(pParm2->Attribute("travel") != NULL)
                                        {
                                            /* checks if travel is a number/empty data */
                                            int attr2;
                                            if(TIXML_SUCCESS != pParm2->QueryIntAttribute("travel", &attr2))
                                                return "false";

                                            /* checks if person is travelling at least to level 2 or more
                                             * but lesser than the total limit in building */
                                            if(attr2 < 2 || attr2 > bh.getNoOfFloor())
                                            {
                                                return "false";
                                            }
                                            else
                                            {
                                                /* Append data to QString */
                                                summary += pParm2->Attribute("travel");
                                                summary += ", ";
                                            }

                                            pParm2 = pParm2->NextSiblingElement();
                                        }
                                        else
                                        {
                                            return "false";
                                        }
                                    }
                                    else
                                    {
                                        return "false";
                                    }
                                }
                                pParm = pParm->NextSiblingElement();

                                /* remove last comma */
                                summary.remove(summary.length()-2,1);

                                /* Append data to QString */
                                summary += "<br>";
                            }
                            else
                            {
                                return "false";
                            }
                        }
                        else
                        {
                            return "false";
                        }
                    }
                    else
                    {
                        return "false";
                    }
                }
            }
            else
            {
                return "false";
            }
        }
        else
        {
            return "false";
        }
    }
    else
    {
        return "false";
    }

    return summary;
}

