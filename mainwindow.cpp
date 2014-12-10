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
        QString filename=QFileDialog::getOpenFileName(
                    this,
                    tr("Open Simulated Human Data"),
                    "./",
                    "XML Data File (*.xml)"

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
    else
    {
        QMessageBox::information(this,tr("Notice"),"Please load building XML first");
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
                "XML Data File (*.xml)"

                );

    /* Set directory to label */
    if(!filename.isEmpty())
    {
        ui->buildingDirLabel->setText(filename);
        ui->humanDirLabel->setDisabled(false);
        ui->humanTotalSpinBox->setDisabled(false);
        ui->humanAvgSpinBox->setDisabled(false);
        ui->humanVisitorSpinBox->setDisabled(false);
    }
    else
    {
        ui->humanDirLabel->setDisabled(true);
        ui->buildingDirLabel->setText("...");
        ui->humanDirLabel->setText("...");
        ui->inputSummaryBox->clear();
        ui->humanTotalSpinBox->setDisabled(true);
        ui->humanAvgSpinBox->setDisabled(true);
        ui->humanVisitorSpinBox->setDisabled(true);
        ui->humanTotalSpinBox->setValue(0);
        ui->humanAvgSpinBox->setValue(0);
        ui->humanVisitorSpinBox->setValue(0);
    }
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
                            "&nbsp;&nbsp;&nbsp;&nbsp;&lt;person id=\"3\" weight=\"69\" resident=\"0\"&gt;<br>"
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

/* Detect humanDirLabel text change */
void MainWindow::on_humanDirLabel_textChanged(const QString &arg1)
{
    if(arg1 != "...")
    {
        update_inputSummaryBox(arg1,ui->buildingDirLabel->text());
    }
}

/* Detect buildingDirLabel text change */
void MainWindow::on_buildingDirLabel_textChanged(const QString &arg1)
{
    if(arg1 != "...")
    {
        update_inputSummaryBox(ui->humanDirLabel->text(), arg1);
    }
}

/* update inputSummaryBox with new data */
void MainWindow::update_inputSummaryBox(const QString &arg1, const QString &arg2)
{
    /* clear */
    ui->inputSummaryBox->clear();

    /* header seperator */
    QString header1 = "Building Data<br>"
                      "====================<br>";

    QString header2 = "Simulated Human Data<br>"
                      "====================<br>";

    /* Output of building */
    ui->inputSummaryBox->append(header1);
    if(arg2 == "...")
    {
        ui->inputSummaryBox->append("No data uploaded<br>");
    }
    else
    {
        /* Display error if invalid */
        if(!validateBuildingData(arg2))
        {
            ui->buildingDirLabel->setText("...");
            ui->inputSummaryBox->append("No data uploaded<br>");
            QMessageBox::critical(this,tr("Error!"),"The XML file selected has errors in them<br>See Help to learn more about XML structures");
        }
        else
        {
            ui->inputSummaryBox->append("Data display here<br>");
        }
    }

    /* Output of simulated humans */
    ui->inputSummaryBox->append(header2);
    if(arg1 == "...")
    {
        ui->inputSummaryBox->append("No data uploaded<br>");
    }
    else
    {
        /* Display error if invalid */
        if(!validateHumanData(arg1))
        {
            ui->humanDirLabel->setText("...");
            ui->inputSummaryBox->append("No data uploaded<br>");
            QMessageBox::critical(this,tr("Error!"),"The XML file selected has errors in them<br>See Help to learn more about XML structures");
        }
        else
        {
            ui->inputSummaryBox->append("Data display here<br>");
        }
    }
}

/* validate building XML */
bool MainWindow::validateBuildingData(const QString &arg1)
{
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
                    return false;

                /* Floors must be at least 2 level*/
                pRoot->QueryIntAttribute("floors", &attr);
                if(attr < 2)
                    return false;
                else
                    totalNoOfFloors = attr;

                /* metrePerFloor must be at least 3m*/
                pRoot->QueryIntAttribute("metrePerFloor", &attr);
                if(attr < 3)
                    return false;

                /* householdPerFloor must be at least 1 or more*/
                pRoot->QueryIntAttribute("householdPerFloor", &attr);
                if(attr < 1)
                    return false;

                /* check if first element of building exist */
                pParm = pRoot->FirstChildElement();
                if(pParm)
                {
                    while(pParm)
                    {
                        if(strcmp(pParm->Value(),"lift") == 0)
                        {
                            /* check if id, maxWeight, speed exist */
                            if(pParm->Attribute("id") == NULL || pParm->Attribute("maxWeight") == NULL || pParm->Attribute("speed") == NULL)
                                return false;
                            else
                            {
                                /* checks for id, maxWeight, speed is a number/empty data */
                                int attr2;
                                if(TIXML_SUCCESS != pParm->QueryIntAttribute("id", &attr2) || TIXML_SUCCESS != pParm->QueryIntAttribute("maxWeight", &attr2) || TIXML_SUCCESS != pParm->QueryIntAttribute("speed", &attr2))
                                    return false;

                                /* maxWeight must be at least 200kg */
                                pParm->QueryIntAttribute("maxWeight", &attr2);
                                if(attr2 < 200)
                                    return false;

                                /* speed must be at least 1 or more*/
                                pParm->QueryIntAttribute("speed", &attr2);
                                if(attr2 < 1)
                                    return false;

                                pParm = pParm->NextSiblingElement();
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
    }
    else
    {
        return false;
    }

    return true;
}

/* validate human XML */
bool MainWindow::validateHumanData(const QString &arg1)
{
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
                        /* checks if id, weight, resident exist */
                        if(pParm->Attribute("id") != NULL && pParm->Attribute("weight") != NULL && pParm->Attribute("resident") != NULL)
                        {
                            /* checks for id, weight, resident is a number/empty data */
                            int attr;
                            if(TIXML_SUCCESS != pParm->QueryIntAttribute("id", &attr) || TIXML_SUCCESS != pParm->QueryIntAttribute("weight", &attr) || TIXML_SUCCESS != pParm->QueryIntAttribute("resident", &attr))
                                return false;

                            /* person must have a resident value of more than 0 and less than building limit */
                            pParm->QueryIntAttribute("resident", &attr);
                            if(attr < 0 || attr > totalNoOfFloors)
                                return false;

                            /* person must have a weight of more than 0 and 150 or lower */
                            pParm->QueryIntAttribute("weight", &attr);
                            if(attr > 150 || attr < 1)
                                return false;

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
                                            if(attr2 < 2 || attr2 > totalNoOfFloors)
                                                return false;

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

    return true;
}
