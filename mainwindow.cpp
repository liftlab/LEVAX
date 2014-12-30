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

    /* Set Spinbox Max Values*/
    ui->humanTotalSpinBox->setMaximum(MAX_PEOPLE);
    ui->humanVisitorSpinBox->setMaximum(MAX_VISITOR);

    /* Set default value */
    prevTotalHuman=0;
    prevTotalVisitor=0;
    loadBuildingByFile=false;
    loadHumanByFile=true;
    updateChanges=false;

    /* Set placeholder text */
    QString phText = "\nFile->Import XML\nor\nGenerate with the spinboxes on the right";
    ui->inputSummaryBox->setPlaceholderText("Building Summary\n"+phText);
    ui->inputSummaryBox_2->setPlaceholderText("Resident Summary\n"+phText);
    ui->inputSummaryBox_3->setPlaceholderText("Visitor Summary\n"+phText);

    /* Create default lift */
    ui->liftCombo->addItem("Lift 1");

    /* Help menu listener */
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(onActionHelp()));
    connect(ui->actionBuilding_Model, SIGNAL(triggered()), this, SLOT(uploadBuildingXML()));
    connect(ui->actionHuman_Model, SIGNAL(triggered()), this, SLOT(uploadHumanXML()));

    /* Debug menu listener */
    connect(ui->actionCheckObj, SIGNAL(triggered()), this, SLOT(onActionCheckObj()));
}

/* Destructor */
MainWindow::~MainWindow()
{
    delete ui;
}

/* humanXMLUploadBtn dialog function */
void MainWindow::uploadHumanXML()
{
    if(loadBuildingByFile || !ui->inputSummaryBox->document()->isEmpty())
    {
        /* Prompt dialog at current folder, accepting .xml file only */
        QFileDialog dlg(NULL, tr("Open Simulated Human Data"));
        dlg.setNameFilter(tr("XML Data File (*.xml)"));
        QString filename;

        /* Check for Open button pressed */
        if(dlg.exec())
        {
            /* Get directory */
            filename = dlg.selectedFiles().at(0);

            /* Reset all previously loaded simulated human data */
            if((shh.getNumberOfSimulatedHumanObject() > 0 && loadHumanByFile) || (shh.getNumberOfVisitorObj() > 0 && loadHumanByFile)
               || (ui->humanTotalSpinBox->value() > 0 || ui->humanVisitorSpinBox->value() > 0))
            {

                ui->inputSummaryBox_2->clear();
                ui->inputSummaryBox_3->clear();

                ui->humanTotalSpinBox->setValue(0);
                ui->humanAvgSpinBox->setValue(0);
                ui->humanVisitorSpinBox->setValue(0);

                shh.resetAll();
            }

            /* Validate data */
            bool verify = validateHumanData(filename);
qDebug() << verify;
            /* Check if human data is invalid */
            if(!verify)
            {
                loadHumanByFile = false;

                /* remove all data */
                shh.resetAll();

                /* Display error */
                QMessageBox::critical(this,tr("Error!"),"The XML file selected has errors in them<br>See Help to learn more about XML structures");
            }
            else
            {
                /* enable update, set value, disable update */
                updateChanges = true;
                ui->humanTotalSpinBox->setValue(shh.getNumberOfSimulatedHumanObject());
                ui->humanVisitorSpinBox->setValue(shh.getNumberOfVisitorObj());
                ui->humanAvgSpinBox->setValue(shh.getNumberOfSimulatedHumanObject()/bh.getNoOfFloor());
                updateChanges = false;
                loadHumanByFile=true;

                /* Display resident */
                if(shh.getNumberOfSimulatedHumanObject() > 0)
                    updatePeopleSummary(true);
                /* Display visitor */
                if(shh.getNumberOfVisitorObj() > 0)
                    updatePeopleSummary(false);

            }
        }
    }
    else
    {
        /* Display warning message to load building XML first */
        QMessageBox::information(this,tr("Notice"), "Please load building XML first");
    }
}

/* buildingXMLUploadBtn dialog function */
void MainWindow::uploadBuildingXML()
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
        ui->inputSummaryBox_2->clear();
        ui->inputSummaryBox_3->clear();

        /* Reset all lift data before any input */
        lh.resetAll();
        /* Reset all simulated human data before any input */
        shh.resetAll();

        /* Validate data */
        QString summary = validateBuildingData(filename);

        /* Check if building data is invalid */
        if(summary == "false")
        {
            /* Reset all elements */
            loadBuildingByFile=false;
            loadHumanByFile=false;
            ui->humanTotalSpinBox->setDisabled(true);
            ui->humanAvgSpinBox->setDisabled(true);
            ui->humanVisitorSpinBox->setDisabled(true);
            ui->humanTotalSpinBox->setValue(0);
            ui->humanAvgSpinBox->setValue(0);
            ui->humanVisitorSpinBox->setValue(0);

            /* reset all building due to error */
            bh.resetAll();

            /* Display error */
            QMessageBox::critical(this,tr("Error!"),"The XML file selected has errors in them<br>See Help to learn more about XML structures");
        }
        else
        {
            /* Remove elements from combobox except index 0*/
            for(int i=ui->liftCombo->count()-1; i > 0; i--)
                ui->liftCombo->removeItem(i);

            /* Display to inputSummaryBox */
            ui->inputSummaryBox->append(summary);

            /* enable update, set value, disable update */
            updateChanges = true;
            /* Display changes to spinboxes and combo box */
            ui->totalFloorSpinBox->setValue(bh.getNoOfFloor());
            ui->totalLiftsSpinBox->setValue(lh.getNumberOfLiftsObject());
            ui->metreSpinBox->setValue(bh.getMetrePerFloor());

            for(int i=0;i<lh.getNumberOfLiftsObject();i++)
            {
                QString liftStr = "Lift " + QString::number(i+1);
                ui->liftCombo->addItem(liftStr);
            }
            updateChanges = false;

            /* Set elements to enable */
            loadBuildingByFile=true;
            ui->humanTotalSpinBox->setDisabled(false);
            ui->humanVisitorSpinBox->setDisabled(false);
            loadHumanByFile=false;
            ui->humanTotalSpinBox->setValue(0);
            ui->humanAvgSpinBox->setValue(0);
            ui->humanVisitorSpinBox->setValue(0);
        }
    }
}

/* Handle total human spin box value changed */
void MainWindow::on_humanTotalSpinBox_valueChanged(int arg1)
{
    if(!updateChanges)
    {
        /* Set average floor value */
        int avgPerFloor = arg1/bh.getNoOfFloor();
        ui->humanAvgSpinBox->setValue(avgPerFloor);

        bool update = false;

        /* Only update when value is more than or equal to 0 */
        if(arg1 >= 0)
        {
            /* Undo and clear memory if previous data is loaded */
            if(loadHumanByFile)
            {
                ui->inputSummaryBox_2->clear();
                ui->inputSummaryBox_3->clear();

                /* Free all simulatedHuman object memory (if any) */
                shh.resetAll();

                /* enable update, set value, disable update */
                updateChanges = true;
                ui->humanVisitorSpinBox->setValue(0);
                ui->humanAvgSpinBox->setValue(0);
                updateChanges = false;

                /* Set to default */
                loadHumanByFile=false;
            }

            /* Handle addition of simulated human (resident) */
            if(arg1 > shh.getNumberOfSimulatedHumanObject())
            {
                /* Add remainding object */
                for(int i=shh.getNumberOfSimulatedHumanObject();i<arg1;i++)
                {
                    /* Create first person */
                    int randResidentOf = rand() % bh.getNoOfFloor() + 2;
                    shh.createSimulatedHuman(randResidentOf);
                    shh.populateSimulatedHuman(bh.getNoOfFloor(), true);

                    update = true;
                }

                loadHumanByFile=false;
            }
            else if(arg1 < shh.getNumberOfSimulatedHumanObject()) /* Handle deletion of simulated human (resident) */
            {
                /* Delete object from the back until arg1 */
                for(int i=shh.getNumberOfSimulatedHumanObject();i>arg1;i--)
                {
                    ui->inputSummaryBox_2->undo();

                    /* true = remove resident */
                    shh.removeLastData(true);
                    ui->inputSummaryBox_2->verticalScrollBar()->setValue(ui->inputSummaryBox_2->verticalScrollBar()->maximum());
                }

                update = false;
            }
            else if(arg1 == shh.getNumberOfSimulatedHumanObject()) /* CPU Management */
            {
                update = false;
            }

            if(update)
            {
                updatePeopleSummary(true);
                ui->inputSummaryBox_2->verticalScrollBar()->setValue(ui->inputSummaryBox_2->verticalScrollBar()->maximum());

                update = false;
            }

            prevTotalHuman = arg1;
        }
    }
}

/* Handle human visitor spin box value changed */
void MainWindow::on_humanVisitorSpinBox_valueChanged(int arg1)
{
    if(!updateChanges)
    {
        bool update = false;

        /* Only update when value is more than or equal to 0 */
        if(arg1 >= 0)
        {
            /* Undo and clear memory if previous data is loaded */
            if(loadHumanByFile)
            {
                ui->inputSummaryBox_2->clear();
                ui->inputSummaryBox_3->clear();

                /* Free all simulatedHuman object memory (if any) */
                shh.resetAll();

                /* Set to default */
                loadHumanByFile=false;
            }

            /* Handle addition of simulated human (visitor) */
            if(arg1 > shh.getNumberOfVisitorObj())
            {
                /* Add remainding object */
                for(int i=shh.getNumberOfVisitorObj();i<arg1;i++)
                {
                    /* Create first person */
                    shh.createSimulatedHuman(0);
                    shh.populateSimulatedHuman(bh.getNoOfFloor(), false);

                    update = true;
                }
            }
            else if(arg1 < shh.getNumberOfVisitorObj()) /* Handle deletion of simulated human (visitor) */
            {
                /* Delete object from the back until arg1 */
                for(int i=shh.getNumberOfVisitorObj();i>arg1;i--)
                {
                    ui->inputSummaryBox_3->undo();

                    /* false = remove visitor */
                    shh.removeLastData(false);
                    ui->inputSummaryBox_3->verticalScrollBar()->setValue(ui->inputSummaryBox_3->verticalScrollBar()->maximum());
                }
                update = false;
            }
            else if(arg1 == shh.getNumberOfVisitorObj()) /* CPU Management */
            {
                update = false;
            }

            if(update)
            {
                updatePeopleSummary(false);
                ui->inputSummaryBox_3->verticalScrollBar()->setValue(ui->inputSummaryBox_3->verticalScrollBar()->maximum());
                update = false;
            }

            prevTotalVisitor = arg1;
        }
    }
}

/* Handle reset button press */
void MainWindow::on_resetBtn_clicked()
{
    /* reset boolean */
    loadHumanByFile=false;
    loadBuildingByFile=false;
    updateChanges=false;

    /* reset values */
    ui->liftCombo->setCurrentIndex(0);
    ui->algoCombo->setCurrentIndex(0);
    ui->humanTotalSpinBox->setValue(0);
    ui->humanAvgSpinBox->setValue(0);
    ui->humanVisitorSpinBox->setValue(0);
    ui->totalFloorSpinBox->setValue(2);
    ui->totalLiftsSpinBox->setValue(1);
    ui->metreSpinBox->setValue(3);
    ui->maxWeightSpinBox->setValue(200);
    ui->maxSpeedDoubleSpinBox->setValue(1.00);

    /* Remove elements from combobox except index 0 */
    for(int i=ui->liftCombo->count()-1; i > 0; i--)
        ui->liftCombo->removeItem(i);

    /* Disable box */
    ui->saveResultBtn->setDisabled(true);
    ui->humanTotalSpinBox->setDisabled(true);
    ui->humanAvgSpinBox->setDisabled(true);
    ui->humanVisitorSpinBox->setDisabled(true);

    /* reset building/lift data */
    bh.resetAll();
    lh.resetAll();
    shh.resetAll();

    /* Create default lift */
    lh.createNewLift(lh.getNumberOfLiftsObject()+1,200,1.00);
    ui->liftCombo->addItem("Lift 1");

    /* clear text box */
    ui->inputSummaryBox->clear();
    ui->inputSummaryBox_2->clear();
    ui->inputSummaryBox_3->clear();
    ui->outputBox->clear();
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
                        "&lt;building floors=\"28\" metrePerFloor=\"3\"&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;lift maxWeight=\"400\" speed=\"1.25\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;lift maxWeight=\"340\" speed=\"1.50\"/&gt;<br>"
                        "&lt;/building&gt;<br><br>"
                        "Building is 28 stories high<br>"
                        "Each level is 3 metres apart. It has two lift<br>"
                        "-Lift 1<br>"
                        "Maximum weight of 400kg and travels 1.25 metre per second<br>"
                        "-Lift 2<br>"
                        "Maximum weight of 340kg and travels 1.50 metre per second<br>";

    /* Pop-up message box */
    QMessageBox::information(this,tr("Help"),message);
}

/* Handle run button press */
void MainWindow::on_runBtn_clicked()
{
    if(shh.getNumberOfSimulatedHumanObject() < 1)
    {
        QMessageBox::warning(this,tr("Form incomplete"), "Please fill in all necessary fields");
    }
    else
    {
        if((shh.getNumberOfSimulatedHumanObject()/bh.getNoOfFloor()) == 0)
            QMessageBox::warning(this,tr("Warning"),"Average people per floor cannot be 0");
        else
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
            ui->tabWidget->setCurrentWidget(ui->tab_2);

            QString content = "Simulation for "+ ui->algoCombo->currentText() + " completed!";
            QMessageBox::information(this,tr("Completed!"), content);
        }
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
    /* header */
    QString header = "<b><font size=\"4\">Simulated Building Data</font></b><br>"
                          "====================<br>";

    /* Stores file data */
    QString summary;
    summary += header;

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
            if(pRoot->Attribute("floors") != NULL && pRoot->Attribute("metrePerFloor") != NULL)
            {
                /* checks for floors, metrePerFloor, householdPerFloor is a number/empty data */
                int attr;
                if(TIXML_SUCCESS != pRoot->QueryIntAttribute("floors", &attr) || TIXML_SUCCESS != pRoot->QueryIntAttribute("metrePerFloor", &attr))
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

                                int maxWeight, speed = 0;

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

                                    maxWeight = attr2;
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
                                    summary += " metre/second<br>";

                                    speed = attr2;
                                }

                                /* Create new lift object */
                                lh.createNewLift(liftCount, maxWeight, speed);

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

    lh.getAllLiftData();
    return summary;
}

/* validate human XML */
bool MainWindow::validateHumanData(const QString &arg1)
{
    /* Resident Status */
    bool isResident = true;

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
                while(pParm)
                {
                    /* is the element named "person" */
                    if(strcmp(pParm->Value(),"person") == 0)
                    {
                        /* checks if weight, resident exist */
                        if(pParm->Attribute("weight") != NULL && pParm->Attribute("resident") != NULL)
                        {
                            /* checks for weight, resident is a number/empty data */
                            int attr;
                            if(TIXML_SUCCESS != pParm->QueryIntAttribute("weight", &attr) || TIXML_SUCCESS != pParm->QueryIntAttribute("resident", &attr))
                                return false;

                            /* person must have a resident value of more than 0 but not 1 and less than building limit */
                            pParm->QueryIntAttribute("resident", &attr);
                            if((attr < 0) || (attr > bh.getNoOfFloor()) || attr == 1)
                            {
                                return false;
                            }
                            else
                            {
                                /* Check if is resident */
                                if(strcmp(pParm->Attribute("resident"),"0") == 0)
                                    isResident = false;
                                else
                                    isResident = true;

                                /* Create a simulatedHuman object with resident value*/
                                shh.createSimulatedHuman(attr);
                            }

                            /* person must have a weight of more than 0 and 150 or lower */
                            pParm->QueryIntAttribute("weight", &attr);
                            if(attr > 150 || attr < 1)
                            {
                                return false;
                            }
                            else
                            {
                                /* Set weight for simulatedHuman */
                                if(isResident)
                                    shh.setWeight(shh.getNumberOfSimulatedHumanObject(), attr, isResident);
                                else
                                    shh.setWeight(shh.getNumberOfVisitorObj(), attr, isResident);
                            }

                            /* checks if first element of person exist */
                            pParm2 = pParm->FirstChildElement();
                            if(pParm2)
                            {
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
                                                return false;

                                            /* checks if person is travelling at least to level 2 or more
                                             * but lesser than the total limit in building */
                                            if(attr2 < 2 || attr2 > bh.getNoOfFloor())
                                            {
                                                return false;
                                            }
                                            else
                                            {
                                                if(isResident)
                                                    shh.addFloorTravelling(shh.getNumberOfSimulatedHumanObject(), attr2, isResident);
                                                else
                                                    shh.addFloorTravelling(shh.getNumberOfVisitorObj(), attr2, isResident);
                                            }

                                            pParm2 = pParm2->NextSiblingElement();
                                        }
                                        else
                                        {
                                            return false;
                                        }
                                    }
                                    else
                                    {
                                        return false;
                                    }
                                }
                                pParm = pParm->NextSiblingElement();
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    //if(shh.getNumberOfSimulatedHumanObject()/bh.getNoOfFloor() == 0)
    //    QMessageBox::warning(this,tr("Warning"),"Average number of resident per floor cannot be 0");

    shh.getAllPersonData();

    return true;
}

/* Update resident/visitor summary */
void MainWindow::updatePeopleSummary(bool isResident)
{
    /* Header */
    QString header;

    if(isResident)
    {
        /* Append header */
        if(shh.getNumberOfSimulatedHumanObject() >= 1)
        {
            /* Create text dynamically */
            for(int i=prevTotalHuman;i<shh.getNumberOfSimulatedHumanObject();i++)
            {
                if(i == 0)
                {
                    header = "<b><font size=\"4\">Simulated Resident Data</font></b><br>"
                            "====================<br>";
                }
                /* Append data to QString */
                header += "<b>Person number ";
                header += QString::number(shh.getPersonID(i, true));

                header += "</b><br>Is a resident staying at level ";
                header +=  QString::number(shh.getResident(i, true));
                header += "<br>";

                /* Append data to QString */
                header += "Weighs ";
                header += QString::number(shh.getWeight(i, true));
                header += "kg<br>";

                /* Append data to QString */
                header += "Will travel to floor ";

                for(int j=0;j<shh.getFloorTravellingSize(i, true);j++)
                {
                    /* Append data to QString */
                    header += QString::number(shh.getFloorTravelling(i, j, true));
                    header += ", ";
                }

                /* remove last comma */
                header.remove(header.length()-2,1);

                ui->inputSummaryBox_2->append(header);

                header.clear();
            }
        }
    }
    else
    {
        /* If there are visitors, update */
        if(shh.getNumberOfVisitorObj() >= 1)
        {
            /* Create text dynamically */
            for(int i=prevTotalVisitor;i<shh.getNumberOfVisitorObj();i++)
            {
                if(i==0)
                {
                    header = "<b><font size=\"4\">Simulated Visitor Data</font></b><br>"
                             "====================<br>";
                }

                /* Append data to QString */
                header += "<b>Visitor number ";
                header += QString::number(shh.getPersonID(i, false));
                header += "</b><br>Is not a resident<br>";

                /* Append data to QString */
                header += "Weighs ";
                header += QString::number(shh.getWeight(i, false));
                header += "kg<br>";

                /* Append data to QString */
                header += "Will travel to floor ";

                for(int j=0;j<shh.getFloorTravellingSize(i, false);j++)
                {
                    /* Append data to QString */
                    header += QString::number(shh.getFloorTravelling(i, j, false));
                    header += ", ";
                }

                /* remove last comma */
                header.remove(header.length()-2,1);

                ui->inputSummaryBox_3->append(header);

                header.clear();
            }
        }
    }
}

/* FOR DEBUGGING PURPOSES ONLY */
/* Check current created object */
void MainWindow::onActionCheckObj()
{
    /* Message */
    QString message = "Number of lift object(s) = "
                    + QString::number(lh.getNumberOfLiftsObject())
                    + "<br>"
                    + "Number of simulatedHuman object(s) = "
                    + QString::number(shh.getNumberOfSimulatedHumanObject())
                    + "<br>"
                    + "Number of non resident object(s) = "
                    + QString::number(shh.getNumberOfVisitorObj());

    /* Pop-up message box */
    QMessageBox::information(this,tr("Number of objects"),message);
}

void MainWindow::on_liftCombo_currentIndexChanged(int index)
{
    if(index != 0)
    {
        ui->maxWeightSpinBox->setDisabled(false);
        ui->maxWeightSpinBox->setValue(lh.getLiftWeight(index-1));
        ui->maxSpeedDoubleSpinBox->setDisabled(false);
        ui->maxSpeedDoubleSpinBox->setValue(lh.getLiftSpeed(index-1));
    }
    else
    {
        ui->maxWeightSpinBox->setDisabled(true);
        ui->maxSpeedDoubleSpinBox->setDisabled(true);
    }
}

void MainWindow::on_totalFloorSpinBox_valueChanged(int arg1)
{
    if(!updateChanges)
    {
        /* Enable human generation */
        ui->humanVisitorSpinBox->setDisabled(false);
        ui->humanTotalSpinBox->setDisabled(false);

        /* Clear summary box */
        ui->inputSummaryBox->clear();

        /* Set new number of floor */
        bh.setNoOfFloors(arg1);

        /* Update summary box */
        updateBuildingSummary();

        /* Scroll to top */
        ui->inputSummaryBox->verticalScrollBar()->setValue(ui->inputSummaryBox->verticalScrollBar()->minimum());

    }
}

void MainWindow::on_metreSpinBox_valueChanged(int arg1)
{
    if(!updateChanges)
    {
        /* Clear summary box */
        ui->inputSummaryBox->clear();

        /* Update new metre per floor */
        bh.setMetrePerFloor(arg1);

        /* Update summary box */
        updateBuildingSummary();

        /* Scroll to top */
        ui->inputSummaryBox->verticalScrollBar()->setValue(ui->inputSummaryBox->verticalScrollBar()->minimum());
    }
}

void MainWindow::on_maxWeightSpinBox_valueChanged(int arg1)
{
    if(!updateChanges)
    {
        if(arg1 > 0)
        {
            /* Clear summary box */
            ui->inputSummaryBox->clear();

            /* Set weight only if current index is not 0 */
            if(ui->liftCombo->currentIndex() != 0)
            {
                /* Set new weight */
                lh.setLiftWeight(ui->liftCombo->currentIndex()-1, arg1);
            }

            /* Update summary box */
            updateBuildingSummary();

            /* Scroll to top */
            ui->inputSummaryBox->verticalScrollBar()->setValue(ui->inputSummaryBox->verticalScrollBar()->maximum());
        }
    }
}

void MainWindow::on_maxSpeedDoubleSpinBox_valueChanged(double arg1)
{
    if(!updateChanges)
    {
        if(arg1 > 0.00)
        {
            /* Clear summary box */
            ui->inputSummaryBox->clear();

            /* Set speed only if current index is not 0 */
            if(ui->liftCombo->currentIndex() != 0)
            {
                /* Set new speed */
                lh.setLiftSpeed(ui->liftCombo->currentIndex()-1, arg1);
            }

            /* Update summary box */
            updateBuildingSummary();

            /* Scroll to top */
            ui->inputSummaryBox->verticalScrollBar()->setValue(ui->inputSummaryBox->verticalScrollBar()->maximum());
        }
    }
}

void MainWindow::on_totalLiftsSpinBox_valueChanged(int arg1)
{
    if(!updateChanges)
    {
        /* Handle addition of simulated human (visitor) */
        if(arg1 > lh.getNumberOfLiftsObject())
        {
            /* Remove elements from combobox except index 0 */
            for(int i=ui->liftCombo->count()-1; i > 0; i--)
                ui->liftCombo->removeItem(i);

            /* Clear summary box */
            ui->inputSummaryBox->clear();

            /* Set new number of lifts */
            bh.setNoOfLifts(arg1);

            /* Add new lift object */
            for(int i=lh.getNumberOfLiftsObject();i<arg1;i++)
            {
                lh.createNewLift(lh.getNumberOfLiftsObject()+1,200,1.00);
            }

            /* Update summary box */
            updateBuildingSummary();

            /* Scroll to top */
            ui->inputSummaryBox->verticalScrollBar()->setValue(ui->inputSummaryBox->verticalScrollBar()->maximum());

            /* Add to combo box */
            for(int i=0;i<arg1;i++)
            {
                QString liftStr = "Lift " + QString::number(i+1);
                ui->liftCombo->addItem(liftStr);
            }
        }
        else if(arg1 < lh.getNumberOfLiftsObject()) /* Handle deletion of simulated human (visitor) */
        {

            /* Remove elements from combobox except index 0 */
            for(int i=ui->liftCombo->count()-1; i > 0; i--)
                ui->liftCombo->removeItem(i);

            /* Clear summary box */
            ui->inputSummaryBox->clear();

            /* Set new number of lifts */
            bh.setNoOfLifts(arg1);

            /* Delete object from the back until arg1 */
            for(int i=lh.getNumberOfLiftsObject();i>arg1;i--)
            {
                lh.removeLastLift();
            }

            /* Update summary box */
            updateBuildingSummary();

            /* Scroll to top */
            ui->inputSummaryBox->verticalScrollBar()->setValue(ui->inputSummaryBox->verticalScrollBar()->maximum());

            /* Add to combo box */
            for(int i=0;i<arg1;i++)
            {
                QString liftStr = "Lift " + QString::number(i+1);
                ui->liftCombo->addItem(liftStr);
            }
        }
    }
}

void MainWindow::updateBuildingSummary()
{
    /* header */
    QString header = "<b><font size=\"4\">Simulated Building Data</font></b><br>"
                          "====================<br>";

    /* Stores file data */
    QString summary;
    summary += header;

    /* Append data to QString */
    summary += "Total number of floors ";
    summary += QString::number(bh.getNoOfFloor());
    summary += "<br>";

    /* Append data to QString */
    summary += "Number of metres per floor ";
    summary += QString::number(bh.getMetrePerFloor());
    summary += "<br>";

    for(int i=0;i<lh.getNumberOfLiftsObject();i++)
    {
        /* Append data to QString */
        summary += "<b>Lift no ";
        summary += QString::number(lh.getLiftID(i));
        summary += "</b><br>";
        summary += "Maximum weight ";
        summary += QString::number(lh.getLiftWeight(i));
        summary += "kg<br>";

        /* Append data to QString */
        summary += "Maximum speed ";
        summary += QString::number(lh.getLiftSpeed(i));
        summary += " metre/second<br>";
    }
    ui->inputSummaryBox->append(summary);
}
