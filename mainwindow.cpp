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
    ui->totalFloorSpinBox->setMaximum(MAX_FLOORS);
    ui->defaultFloorSpinBox->setMaximum(bh.getNoOfFloor());

    /* Set default value */
    prevTotalHuman=0;
    prevTotalVisitor=0;
    loadBuildingByFile=false;
    loadHumanByFile=false;

    /* Set placeholder text */
    ui->inputSummaryBox->setPlaceholderText("Building Summary");
    ui->inputSummaryBox_2->setPlaceholderText("Resident Summary");
    ui->inputSummaryBox_3->setPlaceholderText("Visitor Summary\n\n-Optional-");

    /* Create default lift */
    ui->liftCombo->addItem("Lift 1");

    /* Button listener */
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(onActionHelp()));
    connect(ui->actionBuilding_Model, SIGNAL(triggered()), this, SLOT(uploadBuildingXML()));
    connect(ui->actionHuman_Model, SIGNAL(triggered()), this, SLOT(uploadHumanXML()));
    connect(ui->exportBuildingModel, SIGNAL(triggered()), this, SLOT(exportBuildingXML()));
    connect(ui->exportHumanModel, SIGNAL(triggered()), this, SLOT(exportHumanXML()));
    connect(ui->actionPrint_Simulated_Data, SIGNAL(triggered()), this, SLOT(printSimulatedData()));
    connect(ui->actionPrint_Simulation_Result, SIGNAL(triggered()), this, SLOT(printSimulatedResult()));

}

/* Destructor */
MainWindow::~MainWindow()
{
    delete ui;
}

/* humanXMLUploadBtn dialog function */
void MainWindow::uploadHumanXML()
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
            /* Clear all text box */
            ui->inputSummaryBox_2->clear();
            ui->inputSummaryBox_3->clear();

            /* Reset values */
            ui->humanTotalSpinBox->setValue(0);
            ui->humanAvgSpinBox->setValue(0);
            ui->humanVisitorSpinBox->setValue(0);
            prevTotalHuman=0;
            prevTotalVisitor=0;

            /* Delete all objects */
            shh.resetAll();
        }

        /* Validate data */
        bool verify = validateHumanData(filename);

        /* Check if human data is invalid */
        if(!verify)
        {
            loadHumanByFile = false;

            /* remove all data */
            shh.resetAll();

            prevTotalHuman=0;
            prevTotalVisitor=0;

        }
        else
        {
            /* Set value */
            ui->humanTotalSpinBox->setValue(shh.getNumberOfSimulatedHumanObject());
            ui->humanVisitorSpinBox->setValue(shh.getNumberOfVisitorObj());
            ui->humanAvgSpinBox->setValue(shh.getNumberOfSimulatedHumanObject()/bh.getNoOfFloor());

            loadHumanByFile=true;

            /* Allow run simulation immediately, do not need to apply settings */
            if(loadHumanByFile && loadBuildingByFile)
            {
                ui->runBtn->setDisabled(false);
                ui->applySettingBtn->setDisabled(true);

                ui->exportBuildingModel->setDisabled(false);
                ui->exportHumanModel->setDisabled(false);
            }

            /* Display resident */
            if(shh.getNumberOfSimulatedHumanObject() > 0)
            {
                updateHumanSummary(true);
                ui->humanAvgSpinBox->setValue(shh.getNumberOfSimulatedHumanObject()/(bh.getNoOfFloor()-1));

            }
            /* Display visitor */
            if(shh.getNumberOfVisitorObj() > 0)
                updateHumanSummary(false);

            if(!loadBuildingByFile)
            {
                ui->runBtn->setDisabled(false);
                ui->applySettingBtn->setDisabled(true);
                ui->exportBuildingModel->setDisabled(false);
                ui->exportHumanModel->setDisabled(false);
                updateBuildingSummary();
            }
        }
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

        /* Reset all lift before any input */
        lh.resetAll();

        /* Reset all simulated human data before any input */
        shh.resetAll();

        prevTotalHuman=0;
        prevTotalVisitor=0;

        /* Validate data */
        QString summary = validateBuildingData(filename);

        /* Check if building data is invalid */
        if(summary == "false")
        {
            /* Reset all elements */
            loadBuildingByFile=false;
            loadHumanByFile=false;
            ui->humanTotalSpinBox->setValue(0);
            ui->humanAvgSpinBox->setValue(0);
            ui->humanVisitorSpinBox->setValue(0);

            /* reset all building due to error */
            bh.resetAll();

            /* Create default lift */
            lh.createNewLift(lh.getNumberOfLiftsObject()+1, 400, 3, 1);
            ui->liftCombo->addItem("Lift 1");
        }
        else
        {
            /* Remove elements from combobox except index 0*/
            for(int i=ui->liftCombo->count()-1; i > 0; i--)
                ui->liftCombo->removeItem(i);

            /* Display to inputSummaryBox */
            ui->inputSummaryBox->append(summary);

            /* Display changes to spinboxes and combo box */
            ui->totalFloorSpinBox->setValue(bh.getNoOfFloor());
            ui->totalLiftsSpinBox->setValue(lh.getNumberOfLiftsObject());
            ui->metreSpinBox->setValue(bh.getMetrePerFloor());

            ui->maxSpeedSpinBox->setValue(lh.getLiftSpeed(0));
            ui->maxWeightSpinBox->setValue(lh.getLiftWeight(0));
            ui->defaultFloorSpinBox->setValue(lh.getLiftDefaultFloor(0));

            /* Add lift number to combo box */
            for(int i=0;i<lh.getNumberOfLiftsObject();i++)
            {
                QString liftStr = "Lift " + QString::number(i+1);
                ui->liftCombo->addItem(liftStr);
            }

            /* Set elements to enable */
            loadBuildingByFile=true;
            loadHumanByFile=false;
            ui->humanTotalSpinBox->setDisabled(false);
            ui->humanVisitorSpinBox->setDisabled(false);
            ui->humanTotalSpinBox->setValue(0);
            ui->humanAvgSpinBox->setValue(0);
            ui->humanVisitorSpinBox->setValue(0);
        }
    }
}

/* Export Human XML */
void MainWindow::exportHumanXML()
{
    /* Export only if text area has data */
    if(!ui->inputSummaryBox_2->document()->isEmpty() || !ui->inputSummaryBox_3->document()->isEmpty())
    {
        TiXmlDocument doc;
        TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
        doc.LinkEndChild( decl );

        /* Comments */
        TiXmlComment* comment = new TiXmlComment(" Simulated humans data ");
        doc.LinkEndChild( comment );

        /* Save Human */
        TiXmlElement* element = new TiXmlElement( "human" );
        doc.LinkEndChild( element );

        /* Save Resident */
        for(long i=0; i<shh.getNumberOfSimulatedHumanObject(); i++)
        {
            TiXmlElement* element2 = new TiXmlElement( "person" );

            element2->SetAttribute("weight", shh.getWeight(i,true));
            element2->SetAttribute("resident", shh.getResident(i,true));
            element->LinkEndChild( element2 );

            for(long j=0;j<shh.getNoOfTimesTravel(i,true);j++)
            {
                TiXmlElement* element3 = new TiXmlElement( "floor" );
                element3->SetAttribute("time", shh.getTravelTime(i,j,true));
                element3->SetAttribute("travelFloor", shh.getTravelFloor(i,j,true));
                element2->LinkEndChild( element3 );
            }

        }

        /* Save Visitor */
        for(long i=0; i<shh.getNumberOfVisitorObj(); i++)
        {
            TiXmlElement* element2 = new TiXmlElement( "person" );

            element2->SetAttribute("weight", shh.getWeight(i,false));
            element2->SetAttribute("resident", shh.getResident(i,false));
            element->LinkEndChild( element2 );

            for(long j=0;j<shh.getNoOfTimesTravel(i,false);j++)
            {
                TiXmlElement* element3 = new TiXmlElement( "floor" );
                element3->SetAttribute("time", shh.getTravelTime(i,j,false));
                element3->SetAttribute("travelFloor", shh.getTravelFloor(i,j,false));
                element2->LinkEndChild( element3 );
            }
        }

        /* Display QFileDialog prompt */
        QString fileName = QFileDialog::getSaveFileName(
                    this,
                    tr("Save Human .XML"), "SimulatedHumans.xml",
                    tr("XML Data File (*.xml)"));


        /* Save and display success */
        if(!fileName.isEmpty())
        {
            bool success = doc.SaveFile((fileName.toStdString()).c_str());
            doc.Clear();
            if(success)
                QMessageBox::information(this,tr("Success"),"File saved to " +fileName);
            else
                QMessageBox::information(this,tr("Failure"),"File failed to save for unknown reason");
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),"No data to be exported!");
    }
}

/* Export Building XML */
void MainWindow::exportBuildingXML()
{
    /* Export only if text area has data */
    if(!ui->inputSummaryBox->document()->isEmpty())
    {
        TiXmlDocument doc;
        TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
        doc.LinkEndChild( decl );

        /* Comments */
        TiXmlComment* comment = new TiXmlComment(" Building/Lift Data ");
        doc.LinkEndChild( comment );

        /* Save Building */
        TiXmlElement* element = new TiXmlElement( "building" );

        element->SetAttribute("floors", bh.getNoOfFloor());
        element->SetAttribute("metrePerFloor", bh.getMetrePerFloor());
        doc.LinkEndChild( element );

        /* Save Lift */
        for(long i=0; i<bh.getNoOfLifts(); i++)
        {
            TiXmlElement* element2 = new TiXmlElement( "lift" );

            element2->SetAttribute("maxWeight", lh.getLiftWeight(i));
            element2->SetAttribute("speed", lh.getLiftSpeed(i));
            element2->SetAttribute("defaultFloor", lh.getLiftDefaultFloor(i));

            element->LinkEndChild( element2 );
        }

        /* Display QFileDialog prompt */
        QString fileName = QFileDialog::getSaveFileName(
                    this,
                    tr("Save Building .XML"), "BuildingModel.xml",
                    tr("XML Data File (*.xml)"));


        /* Save and display success */
        if(!fileName.isEmpty())
        {
            bool success = doc.SaveFile((fileName.toStdString()).c_str());
            doc.Clear();
            if(success)
                QMessageBox::information(this,tr("Success"),"File saved to " +fileName);
            else
                QMessageBox::information(this,tr("Failure"),"File failed to save for unknown reason");
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),"No data to be exported!");
    }
}

/* Handle help button press */
void MainWindow::onActionHelp()
{
    /* Help message to describe XML data structure */
    QString message = "<b>E.g. of Simulated Human XML</b><br>"
                       "&lt;human&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person weight=\"60\" resident=\"4\"&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor time=\"21968\" travelFloor=\"1\"/&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor time=\"51414\" travelFloor=\"4\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;/person&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person weight=\"78\" resident=\"27\"&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor time=\"22307\" travelFloor=\"1\"/&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor time=\"52965\" travelFloor=\"27\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;/person&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person weight=\"69\" resident=\"0\"&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor time=\"65946\" travelFloor=\"27\"/&gt;<br>"
                                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;floor time=\"71932\" travelFloor=\"1\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;/person&gt;<br>"
                        "&lt;/human&gt;<br><br>"
                        "-Person 1<br>"
                        "Weighs 60kg, living at 4th floor.<br>"
                        "He travels to 1st floor at 21968s (06:06:08AM) and back to 4th floor at 51414s (14:16:54PM)<br>"
                        "-Person 2<br>"
                        "Weighs 78kg, living at 27th floor.<br>"
                        "He travels to 1st floor at 22307s (06:11:47AM) and back to 27th floor at 52965s (14:42:45PM)<br>"
                        "-Person 3<br>"
                        "Weighs 69kg, not a resident here.(resident=0)<br>"
                        "He travels to 27th floor at 65946s (18:19:06PM) and back to 1st floor at 71932s (19:58:52PM)<br>"
                        "<b>E.g. of Building XML</b><br>"
                        "&lt;building floors=\"28\" metrePerFloor=\"3\"&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;lift maxWeight=\"400\" speed=\"3\" defaultFloor=\"1\"/&gt;<br>"
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;lift maxWeight=\"540\" speed=\"4\" defaultFloor=\"15\"/&gt;<br>"
                        "&lt;/building&gt;<br><br>"
                        "Building is 28 stories high<br>"
                        "Each level is 3 metres apart. It has two lift<br>"
                        "-Lift 1<br>"
                        "Maximum weight of 400kg and travels 3 metre per second with default floor at 1<br>"
                        "-Lift 2<br>"
                        "Maximum weight of 540kg and travels 4 metre per second with default floor at 15<br>";

    /* Pop-up message box */
    QMessageBox::information(this,tr("Help"),message);
}

/* Handle total human spin box value changed */
void MainWindow::on_humanTotalSpinBox_valueChanged(int arg1)
{
    /* Set average floor value */
    int avgPerFloor = arg1/(ui->totalFloorSpinBox->value()-1);
    ui->humanAvgSpinBox->setValue(avgPerFloor);

    /* Enable apply settings button and disable run button */
    ui->applySettingBtn->setDisabled(false);
    ui->runBtn->setDisabled(true);

    /* Disable Export XML Feature */
    ui->exportBuildingModel->setDisabled(true);
    ui->exportHumanModel->setDisabled(true);
}

/* Handle total visitor spin box value changed */
void MainWindow::on_humanVisitorSpinBox_valueChanged(int arg1)
{
    /* Enable apply settings button and disable run button */
    ui->applySettingBtn->setDisabled(false);
    ui->runBtn->setDisabled(true);

    /* Disable Export XML Feature */
    ui->exportBuildingModel->setDisabled(true);
    ui->exportHumanModel->setDisabled(true);
}

/* Handle total number of floor spinbox value changed */
void MainWindow::on_totalFloorSpinBox_valueChanged(int arg1)
{
    /* Set new number of floor */
    bh.setNoOfFloors(ui->totalFloorSpinBox->value());

    ui->defaultFloorSpinBox->setMaximum(arg1);

    /* Set average floor value */
    int avgPerFloor = ui->humanTotalSpinBox->value()/(arg1-1);
    ui->humanAvgSpinBox->setValue(avgPerFloor);

    /* Enable apply settings button and disable run button */
    ui->applySettingBtn->setDisabled(false);
    ui->runBtn->setDisabled(true);

    /* Disable Export XML Feature */
    ui->exportBuildingModel->setDisabled(true);
    ui->exportHumanModel->setDisabled(true);
}

/* Handle metre per floor spinbox value changed */
void MainWindow::on_metreSpinBox_valueChanged(int arg1)
{
    /* Update new metre per floor */
    bh.setMetrePerFloor(ui->metreSpinBox->value());

    /* Enable apply settings button and disable run button */
    ui->applySettingBtn->setDisabled(false);
    ui->runBtn->setDisabled(true);

    /* Disable Export XML Feature */
    ui->exportBuildingModel->setDisabled(true);
    ui->exportHumanModel->setDisabled(true);
}

/* Handle lift combo box selection */
void MainWindow::on_liftCombo_currentIndexChanged(int index)
{
    /* Set spinbox to current lift values and enable it */
    if(index != 0)
    {
        /* Set values */
        ui->maxWeightSpinBox->setValue(lh.getLiftWeight(index-1));
        ui->maxSpeedSpinBox->setValue(lh.getLiftSpeed(index-1));
        ui->defaultFloorSpinBox->setValue(lh.getLiftDefaultFloor(index-1));

        ui->defaultFloorSpinBox->setMaximum(bh.getNoOfFloor());


        /* Enable */
        ui->maxWeightSpinBox->setDisabled(false);
        ui->maxSpeedSpinBox->setDisabled(false);
        ui->defaultFloorSpinBox->setDisabled(false);
    }
    else
    {
        /* Disable spinbox for max weight and max speed */
        ui->maxWeightSpinBox->setDisabled(true);
        ui->maxSpeedSpinBox->setDisabled(true);
        ui->defaultFloorSpinBox->setDisabled(true);
    }
}

/* Handle total number of lift spin box value changed */
void MainWindow::on_totalLiftsSpinBox_valueChanged(int arg1)
{
    /* Handle addition of simulated human (visitor) */
    if(arg1 > lh.getNumberOfLiftsObject())
    {
        /* Remove elements from combobox except index 0 */
        for(int i=ui->liftCombo->count()-1; i > 0; i--)
            ui->liftCombo->removeItem(i);

        /* Set new number of lifts */
        bh.setNoOfLifts(arg1);

        /* Add new lift object */
        for(int i=lh.getNumberOfLiftsObject();i<arg1;i++)
            lh.createNewLift(lh.getNumberOfLiftsObject()+1, 400, 3, 1);

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

        /* Set new number of lifts */
        bh.setNoOfLifts(arg1);

        /* Delete object from the back until arg1 */
        for(int i=lh.getNumberOfLiftsObject();i>arg1;i--)
            lh.removeLastLift();

        /* Add to combo box */
        for(int i=0;i<arg1;i++)
        {
            QString liftStr = "Lift " + QString::number(i+1);
            ui->liftCombo->addItem(liftStr);
        }
    }

    /* Enable apply settings button and disable run button */
    ui->applySettingBtn->setDisabled(false);
    ui->runBtn->setDisabled(true);

    /* Disable spinbox for max weight and max speed */
    ui->exportBuildingModel->setDisabled(true);
    ui->exportHumanModel->setDisabled(true);
}

/* Handle lift max weight spinbox value changed */
void MainWindow::on_maxWeightSpinBox_valueChanged(int arg1)
{
    if(arg1 > 0)
    {
        /* Set weight only if current index is not 0 */
        if(ui->liftCombo->currentIndex() != 0)
            lh.setLiftWeight(ui->liftCombo->currentIndex()-1, arg1);

        /* Enable apply settings button and disable run button */
        ui->applySettingBtn->setDisabled(false);
        ui->runBtn->setDisabled(true);

        /* Disable spinbox for max weight and max speed */
        ui->exportBuildingModel->setDisabled(true);
        ui->exportHumanModel->setDisabled(true);
    }
}

/* Handle lift max speed spin box value changed */
void MainWindow::on_maxSpeedSpinBox_valueChanged(int arg1)
{
    if(arg1 > 0)
    {
        /* Set speed only if current index is not 0 */
        if(ui->liftCombo->currentIndex() != 0)
            lh.setLiftSpeed(ui->liftCombo->currentIndex()-1, arg1);

        /* Enable apply settings button and disable run button */
        ui->applySettingBtn->setDisabled(false);
        ui->runBtn->setDisabled(true);

        /* Disable spinbox for max weight and max speed */
        ui->exportBuildingModel->setDisabled(true);
        ui->exportHumanModel->setDisabled(true);
    }
}

/* Handle reset button press */
void MainWindow::on_resetBtn_clicked()
{
    /* Reset variable */
    loadHumanByFile=false;
    loadBuildingByFile=false;
    prevTotalHuman=0;
    prevTotalVisitor=0;

    /* reset GUI values */
    ui->liftCombo->setCurrentIndex(0);
    ui->algoCombo->setCurrentIndex(0);
    ui->humanTotalSpinBox->setValue(0);
    ui->humanAvgSpinBox->setValue(0);
    ui->humanVisitorSpinBox->setValue(0);
    ui->humanTotalSpinBox->setDisabled(false);
    ui->humanVisitorSpinBox->setDisabled(false);
    ui->humanAvgSpinBox->setDisabled(true);
    ui->totalFloorSpinBox->setValue(2);
    ui->totalLiftsSpinBox->setValue(1);
    ui->metreSpinBox->setValue(3);
    ui->maxWeightSpinBox->setValue(400);
    ui->maxSpeedSpinBox->setValue(3);
    ui->exportBuildingModel->setDisabled(true);
    ui->exportHumanModel->setDisabled(true);
    ui->tabWidget->setCurrentWidget(ui->tab);
    ui->defaultFloorSpinBox->setValue(1);

    /* Remove elements from combobox except index 0 */
    for(int i=ui->liftCombo->count()-1; i > 0; i--)
        ui->liftCombo->removeItem(i);

    /* Disable box */
    ui->saveResultBtn->setDisabled(true);
    ui->humanAvgSpinBox->setDisabled(true);

    /* Reset building/lift data */
    bh.resetAll();
    lh.resetAll();
    shh.resetAll();

    /* Create default lift */
    lh.createNewLift(lh.getNumberOfLiftsObject()+1, 400, 3, 1);
    ui->liftCombo->addItem("Lift 1");

    /* clear text box */
    ui->inputSummaryBox->clear();
    ui->inputSummaryBox_2->clear();
    ui->inputSummaryBox_3->clear();
    ui->outputBox->clear();
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
        if((shh.getNumberOfSimulatedHumanObject()/(bh.getNoOfFloor()-1)) == 0)
            QMessageBox::warning(this,tr("Warning"),"Average people per floor cannot be 0");
        else
        {
            Algorithm algo;
            pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > result;

            if(ui->algoCombo->currentIndex() == 1)
            {
                // returns simulation data, elapsed time, average human wait time
                result = algo.nearestCar(&bh, &lh, &shh);

                QString message = populateResult(result, "Nearest Car");

                /* Set output message */
                ui->outputBox->setText(message);

                ui->saveResultBtn->setDisabled(false);
                ui->tabWidget->setCurrentWidget(ui->tab_2);

                QString content = "Simulation for "+ ui->algoCombo->currentText() + " completed!";
                QMessageBox::information(this,tr("Completed!"), content);
            }
            else if(ui->algoCombo->currentIndex() == 2)
            {               
                result = algo.fixedSectoringCommonSectorSystem(&bh, &lh, &shh);

                QString message = populateResult(result, "Fixed Sectoring Common Sector System (FSO)");

                /* Set output message */
                ui->outputBox->setText(message);

                ui->saveResultBtn->setDisabled(false);
                ui->tabWidget->setCurrentWidget(ui->tab_2);

                QString content = "Simulation for "+ ui->algoCombo->currentText() + " completed!";
                QMessageBox::information(this,tr("Completed!"), content);

            }
            else if(ui->algoCombo->currentIndex() == 3)
            {
                result = algo.fixedSectoringPriorityTimedSystem(&bh, &lh, &shh);

                QString message = populateResult(result, "Fixed Sectoring Priority Timed System (FS4)");

                /* Set output message */
                ui->outputBox->setText(message);

                ui->saveResultBtn->setDisabled(false);
                ui->tabWidget->setCurrentWidget(ui->tab_2);

                QString content = "Simulation for "+ ui->algoCombo->currentText() + " completed!";
                QMessageBox::information(this,tr("Completed!"), content);
            }
            else if(ui->algoCombo->currentIndex() == 4)
            {
                result = algo.dynamicSectoringSystem(&bh, &lh, &shh);

                QString message = populateResult(result, "Dynamic Sectoring System (DS)");

                /* Set output message */
                ui->outputBox->setText(message);

                ui->saveResultBtn->setDisabled(false);
                ui->tabWidget->setCurrentWidget(ui->tab_2);

                QString content = "Simulation for "+ ui->algoCombo->currentText() + " completed!";
                QMessageBox::information(this,tr("Completed!"), content);
            }
            else
            {
                QString message = "";

                /* Execute nearest car algorithm */
                result = algo.nearestCar(&bh, &lh, &shh);
                message = populateResult(result, "Nearest Car");

                int averageWaitTime1 = result.second.second.first;
                int averageTravelTime1 = result.second.second.second;

                /* Execute Fixed Sectoring Common Sector System (FSO) algorithm */
                result = algo.fixedSectoringCommonSectorSystem(&bh, &lh, &shh);
                message += populateResult(result, "Fixed Sectoring Common Sector System (FSO)");

                int averageWaitTime2 = result.second.second.first;
                int averageTravelTime2 = result.second.second.second;

                /* Execute Fixed Sectoring Priority Timed System (FS4) algorithm */
                result = algo.fixedSectoringPriorityTimedSystem(&bh, &lh, &shh);
                message += populateResult(result, "Fixed Sectoring Priority Timed System (FS4)");

                int averageWaitTime3 = result.second.second.first;
                int averageTravelTime3 = result.second.second.second;

                /* Execute Dynamic Sectoring System (DS) algorithm */
                result = algo.dynamicSectoringSystem(&bh, &lh, &shh);
                message += populateResult(result, "Dynamic Sectoring System (DS)");

                int averageWaitTime4 = result.second.second.first;
                int averageTravelTime4 = result.second.second.second;

                map<QString, int> points;
                points["Nearest Car"] = 0;
                points["Fixed Sectoring Common Sector System (FSO)"] = 0;
                points["Fixed Sectoring Priority Timed System (FS4)"] = 0;
                points["Dynamic Sectoring System (DS)"] = 0;

                /* Populate points for best average wait time */
                if(averageWaitTime1 < averageWaitTime2)
                    points["Nearest Car"]++;
                if(averageWaitTime1 < averageWaitTime3)
                    points["Nearest Car"]++;
                if(averageWaitTime1 < averageWaitTime4)
                    points["Nearest Car"]++;

                if(averageWaitTime2 < averageWaitTime1)
                    points["Fixed Sectoring Common Sector System (FSO)"]++;
                if(averageWaitTime2 < averageWaitTime3)
                    points["Fixed Sectoring Common Sector System (FSO)"]++;
                if(averageWaitTime2 < averageWaitTime4)
                    points["Fixed Sectoring Common Sector System (FSO)"]++;

                if(averageWaitTime3 < averageWaitTime1)
                    points["Fixed Sectoring Priority Timed System (FS4)"]++;
                if(averageWaitTime3 < averageWaitTime2)
                    points["Fixed Sectoring Priority Timed System (FS4)"]++;
                if(averageWaitTime3 < averageWaitTime4)
                    points["Fixed Sectoring Priority Timed System (FS4)"]++;

                if(averageWaitTime4 < averageWaitTime1)
                    points["Dynamic Sectoring System (DS)"]++;
                if(averageWaitTime4 < averageWaitTime2)
                    points["Dynamic Sectoring System (DS)"]++;
                if(averageWaitTime4 < averageWaitTime3)
                    points["Dynamic Sectoring System (DS)"]++;

                /* Populate points for best average travel time */
                if(averageTravelTime1 < averageTravelTime2)
                    points["Nearest Car"]++;
                if(averageTravelTime1 < averageTravelTime3)
                    points["Nearest Car"]++;
                if(averageTravelTime1 < averageTravelTime4)
                    points["Nearest Car"]++;

                if(averageTravelTime2 < averageTravelTime1)
                    points["Fixed Sectoring Common Sector System (FSO)"]++;
                if(averageTravelTime2 < averageTravelTime3)
                    points["Fixed Sectoring Common Sector System (FSO)"]++;
                if(averageTravelTime2 < averageTravelTime4)
                    points["Fixed Sectoring Common Sector System (FSO)"]++;

                if(averageTravelTime3 < averageTravelTime1)
                    points["Fixed Sectoring Priority Timed System (FS4)"]++;
                if(averageTravelTime3 < averageTravelTime2)
                    points["Fixed Sectoring Priority Timed System (FS4)"]++;
                if(averageTravelTime3 < averageTravelTime4)
                    points["Fixed Sectoring Priority Timed System (FS4)"]++;

                if(averageTravelTime4 < averageTravelTime1)
                    points["Dynamic Sectoring System (DS)"]++;
                if(averageTravelTime4 < averageTravelTime2)
                    points["Dynamic Sectoring System (DS)"]++;
                if(averageTravelTime4 < averageTravelTime3)
                    points["Dynamic Sectoring System (DS)"]++;

                /* Find algorithm with highest points */
                map<QString, int>::const_iterator it = points.begin();
                int highest = it->second;
                QString bestAlgo = it->first;
                for(map<QString, int>::const_iterator it = points.begin(); it != points.end(); ++it)
                {
                    if(it->second > highest)
                    {
                        highest = it->second;
                        bestAlgo = it->first;
                    }
                }

                message = "The algorithm that scored the most points in terms of"
                        " average waiting time and average travel time is <b>"
                        + bestAlgo + "</b><br><br>"
                        + message
                        + "<br><br>";

                /* Set output message */
                ui->outputBox->setText(message);

                ui->saveResultBtn->setDisabled(false);
                ui->tabWidget->setCurrentWidget(ui->tab_2);

                QString content = "Simulation for "+ ui->algoCombo->currentText() + " completed!";
                QMessageBox::information(this,tr("Completed!"), content);
            }

            if((loadBuildingByFile && loadHumanByFile) || (loadHumanByFile && !loadBuildingByFile))
            {
                /* Disable apply settings button and enable run button */
                ui->runBtn->setDisabled(false);
                ui->applySettingBtn->setDisabled(true);
            }
            else
            {
                /* Enable apply settings button and disable run button */
                ui->runBtn->setDisabled(true);
                ui->applySettingBtn->setDisabled(false);
            }
            /* Enable Export XML Feature */
            ui->exportBuildingModel->setDisabled(false);
            ui->exportHumanModel->setDisabled(false);
        }       
    }  
}

/* Populate result message */
QString MainWindow::populateResult(pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > result, QString algoName)
{
    /* Output message */
    QString message = "<b>" + algoName + " simulation completed in "
                    + QString::number(result.second.first.first, 'f', 2)
                    + " seconds</b><br><br>";

    message += "Average passenger wait time: <b>" + QString::number(result.second.second.first) + "s</b><br>";
    message += "Average passenger travel time: <b>" + QString::number(result.second.second.second) + "s</b><br>";

    for(int i=0;i<lh.getNumberOfLiftsObject();i++)
    {
        message += "Lift " + QString::number(i+1) + " idle time: <b>" + QDateTime::fromTime_t(result.second.first.second[i]).toUTC().toString("h") + "hr ";
        message += QDateTime::fromTime_t(result.second.first.second[i]).toUTC().toString("m") + "min ";
        message += QDateTime::fromTime_t(result.second.first.second[i]).toUTC().toString("s") + "sec</b><br>";
    }

    message += "<br>";
    message += result.first;

    return message;
}

/* Handle save result button clicked */
void MainWindow::on_saveResultBtn_clicked()
{
    /* Initialize current date time */
    QDateTime local(QDateTime::currentDateTime());

    /* Display QFileDialog prompt */
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save File"), "Result_"+local.toString("dMMM_HH-mm-ssap")+".txt",
                tr("Text Documents (*.txt);;"));

    /* Save and display success */
    if(!fileName.isEmpty())
    {
        QFile file;
        file.setFileName(fileName);
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream stream(&file);
        stream << ui->outputBox->toPlainText();
        file.close();

        QMessageBox::information(this,tr("Help"),"File saved to "+fileName);
    }
}

/* Handle apply button clicked, create model */
void MainWindow::on_applySettingBtn_clicked()
{
    if(ui->humanTotalSpinBox->value() == 0)
    {
        QMessageBox::information(this,tr("Error"),"Unable to apply settings!<br>Resident must be more than 0");
    }
    else if(ui->humanAvgSpinBox->value() == 0)
    {
        QMessageBox::information(this,tr("Error"),"Unable to apply settings!<br>Average people per level must be more than 0");
    }
    else
    {
        /* Display building summary */
        updateBuildingSummary();

        /* Only update when value is more than or equal to 0 */
        if(ui->humanTotalSpinBox->value() >= 0)
        {
            /* Handle addition of simulated human (resident) */
            if(ui->humanTotalSpinBox->value() > shh.getNumberOfSimulatedHumanObject())
            {
                /* Add remainding object */
                for(int i=shh.getNumberOfSimulatedHumanObject();i<ui->humanTotalSpinBox->value();i++)
                {
                    int randResidentOf = 2 + (rand() % (bh.getNoOfFloor() - 2 + 1));
                    shh.createSimulatedHuman(randResidentOf);
                    shh.populateSimulatedHuman(i, bh.getNoOfFloor(), true);
                }
            }
            else if(ui->humanTotalSpinBox->value() < shh.getNumberOfSimulatedHumanObject()) /* Handle deletion of simulated human (resident) */
            {
                /* Delete object from the back until ui->humanTotalSpinBox->value() */
                for(int i=shh.getNumberOfSimulatedHumanObject();i>ui->humanTotalSpinBox->value();i--)
                {
                    ui->inputSummaryBox_2->undo();

                    /* true = remove resident */
                    shh.removeLastData(true);
                }
            }

            /* Update floors to all resident evenly only if file is not loaded */
            int avgPerFloor = ui->humanTotalSpinBox->value()/(bh.getNoOfFloor()-1);
            shh.updateFloorsEvenly(avgPerFloor, bh.getNoOfFloor(), ui->humanTotalSpinBox->value());


            /* Update human */
            updateHumanSummary(true);

            prevTotalHuman = ui->humanTotalSpinBox->value();
        }

        /* Only update when value is more than or equal to 0 */
        if(ui->humanVisitorSpinBox->value() >= 0)
        {
            /* Handle addition of simulated human (visitor) */
            if(ui->humanVisitorSpinBox->value() > shh.getNumberOfVisitorObj())
            {
                /* Add remainding object */
                for(int i=shh.getNumberOfVisitorObj();i<ui->humanVisitorSpinBox->value();i++)
                {
                    shh.createSimulatedHuman(0);
                }
            }
            else if(ui->humanVisitorSpinBox->value() < shh.getNumberOfVisitorObj()) /* Handle deletion of simulated human (visitor) */
            {
                /* Delete object from the back until ui->humanVisitorSpinBox->value() */
                for(int i=shh.getNumberOfVisitorObj();i>ui->humanVisitorSpinBox->value();i--)
                {
                    ui->inputSummaryBox_3->undo();

                    /* false = remove visitor */
                    shh.removeLastData(false);
                }
            }

            /* Update all visitor object with new visit floor */
            for(int i=0;i<shh.getNumberOfVisitorObj();i++)
                shh.populateSimulatedHuman(i, bh.getNoOfFloor(), false);

            /* Update human */
            updateHumanSummary(false);

            prevTotalVisitor = ui->humanVisitorSpinBox->value();
        }

        /* Scroll to top */
        ui->inputSummaryBox->verticalScrollBar()->setValue(ui->inputSummaryBox->verticalScrollBar()->minimum());
        ui->inputSummaryBox_2->verticalScrollBar()->setValue(ui->inputSummaryBox_2->verticalScrollBar()->minimum());
        ui->inputSummaryBox_3->verticalScrollBar()->setValue(ui->inputSummaryBox_3->verticalScrollBar()->minimum());

        /* Disable apply settings button and enable run button */
        ui->applySettingBtn->setDisabled(true);
        ui->runBtn->setDisabled(false);

        /* Clear simulation result, if any */
        ui->outputBox->clear();

        /* Switch Tab */
        ui->tabWidget->setCurrentWidget(ui->tab);

        /* Enable Export XML Feature */
        ui->exportBuildingModel->setDisabled(false);
        ui->exportHumanModel->setDisabled(false);
    }
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
                {
                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"floors\" or \"metrePerFloor\" is either empty or not a number");
                    return "false";
                }

                /* Floors must be at least 2 level */
                pRoot->QueryIntAttribute("floors", &attr);
                if(attr < 2 || attr > MAX_FLOORS)
                {
                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"floors\" must be in the range of 2 to " + QString::number(MAX_FLOORS));
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
                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"metrePerFloor\" must be 3 or more");
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
                            if(pParm->Attribute("maxWeight") == NULL || pParm->Attribute("speed") == NULL || pParm->Attribute("defaultFloor") == NULL)
                            {
                                QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nElement \"maxWeight\", \"speed\" or \"defaultFloor\" missing");
                                return "false";
                            }
                            else
                            {
                                /* checks for maxWeight, speed is a number/empty data */
                                int attr2;
                                if(TIXML_SUCCESS != pParm->QueryIntAttribute("maxWeight", &attr2) || TIXML_SUCCESS != pParm->QueryIntAttribute("speed", &attr2) || TIXML_SUCCESS != pParm->QueryIntAttribute("defaultFloor", &attr2))
                                {
                                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"maxWeight\", \"speed\" or \"defaultFloor\" is either \nempty or not a number");
                                    return "false";
                                }
                                int maxWeight, speed, defaultFloor = 0;

                                /* maxWeight must be at least 400kg */
                                pParm->QueryIntAttribute("maxWeight", &attr2);
                                if(attr2 < 400)
                                {
                                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"maxWeight\" must be more than or equal to 400kg");
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

                                /* speed must be in the range of 3 to 6 */
                                pParm->QueryIntAttribute("speed", &speed);
                                if(speed < 3 || speed > 6)
                                {
                                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"speed\" must range from 3 to 6");
                                    return "false";
                                }
                                else
                                {
                                    /* Append data to QString */
                                    summary += "Maximum speed ";
                                    summary += pParm->Attribute("speed");
                                    summary += " metre/second<br>";
                                }

                                /* defaultFloor must be at least 1 or more and not more than the total no of floor */
                                pParm->QueryIntAttribute("defaultFloor", &attr2);
                                if(attr2 < 1 || attr2 >= bh.getNoOfFloor()+1)
                                {
                                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"defaultFloor\" must be from 1 to " + QString::number(bh.getNoOfFloor()));
                                    return "false";
                                }
                                else
                                {
                                    /* Append data to QString */
                                    summary += "Default floor ";
                                    summary += pParm->Attribute("defaultFloor");
                                    summary += "<br>";

                                    defaultFloor = attr2;
                                }

                                /* Create new lift object */
                                lh.createNewLift(liftCount, maxWeight, speed, defaultFloor);

                                /* If lift is at the highest level, set direction to DOWN (-1) */
                                if(defaultFloor == bh.getNoOfFloor())
                                    lh.setLiftDirection(liftCount-1, -1);

                                pParm = pParm->NextSiblingElement();
                            }
                        }
                        else
                        {
                            QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nSibling element must be named <lift>");
                            return "false";
                        }
                    }

                    /* set number of lifts */
                    if(liftCount > MAX_LIFTS)
                    {
                        QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nThere can only be a maximum of " + QString::number(MAX_LIFTS) + " lifts");
                        return "false";
                    }
                    else
                        bh.setNoOfLifts(liftCount);
                }
                else
                {
                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nSibling element <lift> missing");
                    return "false";
                }
            }
            else
            {
                QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nElement \"floors\" or \"metrePerFloor\" missing");
                return "false";
            }
        }
        else
        {
            QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nNo building data found");
            return "false";
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nError in XML structure\nSee Help to learn more about XML structures");
        return "false";
    }
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
                int residentCount = 0;
                int visitorCount = 0;
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
                            {
                                QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"weight\" or \"resident\" is either empty or not a number");
                                return false;
                            }
                            /* person must have a resident value of more than 0 but not 1 and less than building limit */
                            pParm->QueryIntAttribute("resident", &attr);
                            if(attr < 0 || attr >= bh.getNoOfFloor()+1 || attr == 1)
                            {
                                QMessageBox::critical(this,tr("Inconsistent data!"), "Load XML Fail!\nA resident has an invalid floor!\ni.e. Negative or more than the current building model");
                                return false;
                            }
                            else
                            {
                                /* Check if is resident */
                                if(strcmp(pParm->Attribute("resident"), "0") == 0)
                                {
                                    visitorCount++;
                                    isResident = false;
                                }
                                else
                                {
                                    residentCount++;
                                    isResident = true;
                                }
                                /* Create a simulatedHuman object with resident value */
                                shh.createSimulatedHuman(attr);
                            }

                            /* person must have a weight of more than 0 and 150 or lower */
                            pParm->QueryIntAttribute("weight", &attr);
                            if(attr > 150 || attr < 1)
                            {
                                QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"weight\" has to be 1 to 150");
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
                                        /* check if time and travel exist */
                                        if(pParm2->Attribute("time") != NULL && pParm2->Attribute("travelFloor") != NULL)
                                        {
                                            /* checks if travel is a number/empty data */
                                            int travel;
                                            if(TIXML_SUCCESS != pParm2->QueryIntAttribute("travelFloor", &travel))
                                            {
                                                QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"travelFloor\" is either empty or not a number");
                                                return false;
                                            }

                                            int time;
                                            if(TIXML_SUCCESS != pParm2->QueryIntAttribute("time", &time))
                                            {
                                                QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nAttribute \"time\" is either empty or not a number");
                                                return false;
                                            }

                                            /* checks if person is travelling at least to level 1 or more
                                             * but lesser than the total limit in building */
                                            if(travel < 1 || travel >= bh.getNoOfFloor()+1)
                                            {
                                                QMessageBox::critical(this,tr("Inconsistent data!"), "Load XML Fail!\nThe data has an invalid travel floor!\ni.e. More than the current building model");
                                                return false;
                                            }

                                            if(time < 0 || time > 86399)
                                            {
                                                QMessageBox::critical(this,tr("Load XML Fail!!"), "Load XML Fail!\nTime has to be from 0 to 86399 seconds");
                                                return false;
                                            }

                                            if(isResident)
                                            {
                                                shh.addTravelTime(time, travel, shh.getNumberOfSimulatedHumanObject()-1, isResident);
                                            }
                                            else
                                            {
                                                shh.addTravelTime(time, travel, shh.getNumberOfVisitorObj()-1, isResident);
                                            }

                                            pParm2 = pParm2->NextSiblingElement();
                                        }
                                        else
                                        {
                                            QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nElement \"time\" or \"travel\" missing");
                                            return false;
                                        }
                                    }
                                    else
                                    {
                                        QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nSibling Element must be named <floor>");
                                        return false;
                                    }

                                }
                                pParm = pParm->NextSiblingElement();
                            }
                            else
                            {
                                QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nSibling Element <floor> missing");
                                return false;
                            }
                        }
                        else
                        {
                            QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nElement \"weight\" or \"resident\" missing");
                            return false;
                        }
                    }
                    else
                    {
                        QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nElement must be named \"Person\"");
                        return false;
                    }
                }
                if(visitorCount > MAX_VISITOR)
                {
                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nThere can only be a maximum of " + QString::number(MAX_VISITOR) + " visitors");
                    return false;
                }
                if(residentCount > MAX_PEOPLE)
                {
                    QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nThere can only be a maximum of " + QString::number(MAX_PEOPLE) + " residents");
                    return false;
                }
            }
            else
            {
                QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nElement <person> not found");
                return false;
            }
        }
        else
        {
            QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nNo simulated human data found");
            return false;
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Load XML Fail!"), "Load XML Fail!\nError in XML structure\nSee Help to learn more about XML structures");
        return false;
    }

    return true;
}

/* Update resident/visitor summary */
void MainWindow::updateHumanSummary(bool isResident)
{
    /* Header */
    QString header;

    if(isResident)
    {
        /* Append header */
        if(shh.getNumberOfSimulatedHumanObject() >= 1)
        {
            ui->inputSummaryBox_2->clear();

            /* Create text dynamically */
            for(int i=0;i<shh.getNumberOfSimulatedHumanObject();i++)
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
                header += "<br>No Of Times Travel ";
                header += QString::number(shh.getNoOfTimesTravel(i, true));
                header += "<br>";

                int travelTime = 0;

                for(int m=0;m<shh.getNoOfTimesTravel(i, true);m++)
                {
                    travelTime = shh.getTravelTime(i, m, true);

                    header += "Travel Time ";

                    header += QDateTime::fromTime_t(travelTime).toUTC().toString("h:mm:ss ap");
                    header += " to floor ";
                    header += QString::number(shh.getTravelFloor(i, m, true));


                    header += "<br>";
                }
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
            ui->inputSummaryBox_3->clear();

            /* Create text dynamically */
            for(int i=0;i<shh.getNumberOfVisitorObj();i++)
            {
                if(i==0)
                {
                    header = "<b><font size=\"4\">Simulated Visitor Data</font></b><br>"
                             "====================<br>";
                }

                /* Append data to QString */
                header += "<b>Visitor number ";
                header += QString::number(shh.getPersonID(i, false));
                header += "</b><br>";

                /* Append data to QString */
                header += "Weighs ";
                header += QString::number(shh.getWeight(i, false));
                header += "kg<br>";

                /* Append data to QString */
                header += "<br>No Of Times Travel ";
                header += QString::number(shh.getNoOfTimesTravel(i, false));
                header += "<br>";

                int travelTime = 0;

                for(int m=0;m<shh.getNoOfTimesTravel(i, false);m++)
                {
                    travelTime = shh.getTravelTime(i, m, false);

                    header += "Travel Time ";

                    header += QDateTime::fromTime_t(travelTime).toUTC().toString("h:mm:ss ap");
                    header += " to floor ";
                    header += QString::number(shh.getTravelFloor(i, m, false));

                    header += "<br>";
                }
                ui->inputSummaryBox_3->append(header);

                header.clear();
            }
        }
    }

    /* Set text focus to top */
    ui->inputSummaryBox_2->verticalScrollBar()->setValue(ui->inputSummaryBox_2->verticalScrollBar()->minimum());
    ui->inputSummaryBox_3->verticalScrollBar()->setValue(ui->inputSummaryBox_3->verticalScrollBar()->minimum());

}

/* Update building summary */
void MainWindow::updateBuildingSummary()
{
    /* Undo if document is not empty */
    if(!ui->inputSummaryBox->document()->isEmpty())
        ui->inputSummaryBox->undo();

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

    /* Loop number of lifts */
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

        /* Append data to QString */
        summary += "Default floor ";
        summary += QString::number(lh.getLiftDefaultFloor(i));
        summary += "<br>";
    }

    /* Append summary */
    ui->inputSummaryBox->append(summary);

    summary.clear();
    header.clear();

    /* Set text focus to top */
    ui->inputSummaryBox->verticalScrollBar()->setValue(ui->inputSummaryBox->verticalScrollBar()->minimum());
}

/* Combine simulated humans and simulated building model */
QString MainWindow::generateSimulationData()
{
    QString consolidatation;

    consolidatation = ui ->inputSummaryBox->toPlainText();

    consolidatation += "\n";

    consolidatation += ui ->inputSummaryBox_2->toPlainText();

    consolidatation += "\n";

    consolidatation += ui ->inputSummaryBox_3->toPlainText();

    return consolidatation;
}

/* Print Simulated data */
void MainWindow::printSimulatedData()
{
    if(!ui->inputSummaryBox_2->document()->isEmpty() || !ui->inputSummaryBox_3->document()->isEmpty())
    {
        /* Check for available printer */
        QList <QPrinterInfo> plist = QPrinterInfo::availablePrinters();

        /* If printer available */
        if(plist.count() != 0)
        {
            QPrinter printer;

            QPrintDialog *dialog = new QPrintDialog(&printer, this);
            dialog ->setWindowTitle("Print Simulated Data");

            if(dialog ->exec() == QDialog::Accepted){
                QTextDocument doc (generateSimulationData());
                doc.print(&printer);
            }
        }
        else
        {
            QMessageBox::critical(this,tr("Print Fail!"),"Printing Fail!<br><br>1) No printer detected.<br>2) Please start the Print Spooler service and try again.");
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),"No data to be printed!");
    }
}

/* Print Simulated result */
void MainWindow::printSimulatedResult()
{
    if(!ui->outputBox->document()->isEmpty())
    {
        QPrinter printer;

        QPrintDialog *dialog = new QPrintDialog(&printer, this);
        dialog->setWindowTitle("Print Simulated Result");

        if(dialog ->exec() == QDialog::Accepted){
            QTextDocument doc (ui->outputBox->toPlainText());
            doc.print(&printer);
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),"No result to be printed!");
    }
}

/* Handle lift default floor spinbox value changed */
void MainWindow::on_defaultFloorSpinBox_valueChanged(int arg1)
{
    if(arg1 > 0)
    {
        if(ui->defaultFloorSpinBox->isEnabled())
        {
            /* Set weight only if current index is not 0 */
            if(ui->liftCombo->currentIndex() != 0)
                lh.setLiftDefaultFloor(ui->liftCombo->currentIndex()-1, arg1);

            /* If lift is at the highest level, set direction to DOWN (-1) */
            if(arg1 == bh.getNoOfFloor())
            {
                lh.setLiftDirection(ui->liftCombo->currentIndex()-1, -1);
            }
            /* Enable apply settings button and disable run button */
            ui->applySettingBtn->setDisabled(false);
            ui->runBtn->setDisabled(true);

            /* Disable spinbox for max weight and max speed */
            ui->exportBuildingModel->setDisabled(true);
            ui->exportHumanModel->setDisabled(true);
        }
    }
}
