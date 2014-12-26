/*-----------------------------------------------------------------------------
 * File      - SimulatedHumanHandler.cpp
 * Author    - LEVAX
 * Purpose   - Handles SimulatedHuman object
 *---------------------------------------------------------------------------*/

#include "SimulatedHumanHandler.h"

/* Constructor */
SimulatedHumanHandler::SimulatedHumanHandler()
{
    /* Reserve memory */
    simulatedHumanObj.reserve(MAX_PEOPLE);
    nonResSimulatedHumanObj.reserve(MAX_VISITOR);
}

/* Destructor */
SimulatedHumanHandler::~SimulatedHumanHandler()
{
    resetAll();
}

/* Create simulatedHuman */
void SimulatedHumanHandler::createSimulatedHuman(int residentOf)
{
    /* Create new object */
    SimulatedHuman *sh = new SimulatedHuman(residentOf);

    if(residentOf != 0)
    {
        sh->setPersonID(simulatedHumanObj.size()+1);
        simulatedHumanObj.push_back(sh);
    }
    else
    {
        sh->setPersonID(nonResSimulatedHumanObj.size()+1);
        nonResSimulatedHumanObj.push_back(sh);
    }
}

/* Free and reset all simulatedHuman object in simulatedHumanObj vector */
void SimulatedHumanHandler::resetAll()
{
    if(simulatedHumanObj.size() != 0)
    {
        qDebug() << "Freeing memory for resident";
        for(std::size_t i=0;i<simulatedHumanObj.size();i++)
        {
            delete simulatedHumanObj[i];
            qDebug() << "Memory cleared for simulatedHuman resident" << i+1;
        }
        simulatedHumanObj.clear();
    }

    if(nonResSimulatedHumanObj.size() != 0)
    {
        qDebug() << "Freeing memory for visitor";
        for(std::size_t i=0;i<nonResSimulatedHumanObj.size();i++)
        {
            delete nonResSimulatedHumanObj[i];
            qDebug() << "Memory cleared for simulatedHuman visitor" << i+1;
        }
        nonResSimulatedHumanObj.clear();
    }
}

/* Return number of simulatedHuman objects */
int SimulatedHumanHandler::getNumberOfSimulatedHumanObject()
{
    return simulatedHumanObj.size();
}

/* Set person id */
void SimulatedHumanHandler::setPersonID(int idx, int personID, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx]->setPersonID(personID);
    else
        nonResSimulatedHumanObj[idx]->setPersonID(personID);
}

/* Set resident floor */
void SimulatedHumanHandler::setResident(int idx, int residentOf, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx]->setResident(residentOf);
    else
        nonResSimulatedHumanObj[idx]->setResident(0);
}

/* Set weight */
void SimulatedHumanHandler::setWeight(int idx, int weight, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx-1]->setWeight(weight);
    else
        nonResSimulatedHumanObj[idx-1]->setWeight(weight);
}

/* Add floor travelling */
void SimulatedHumanHandler::addFloorTravelling(int idx, int floor, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx-1]->addFloorTravelling(floor);
    else
       nonResSimulatedHumanObj[idx-1]->addFloorTravelling(floor);
}

/* Return person id */
int SimulatedHumanHandler::getPersonID(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getPersonID();
    else
        return nonResSimulatedHumanObj[idx]->getPersonID();
}

/* Return resident floor */
int SimulatedHumanHandler::getResident(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getResident();
    else
        return nonResSimulatedHumanObj[idx]->getResident();
}

/* Return weight */
int SimulatedHumanHandler::getWeight(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getWeight();
    else
        return nonResSimulatedHumanObj[idx]->getWeight();
}

/* Return floor travelling at index */
int SimulatedHumanHandler::getFloorTravelling(int idxOfPerson, int idxOfFloorTravelling, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idxOfPerson]->getFloorTravelling(idxOfFloorTravelling);
    else
        return nonResSimulatedHumanObj[idxOfPerson]->getFloorTravelling(idxOfFloorTravelling);
}

/* Populate simulatedHuman with weight, etc */
void SimulatedHumanHandler::populateSimulatedHuman(int totalNoOfFloors, bool isResident)
{
    /* Add own residence floor if its a residence*/
    if(isResident)
        simulatedHumanObj.back()->addFloorTravelling(simulatedHumanObj.back()->getResident());

    /* Generate 1-100
     * 1-15     = Child,
     * 21-30    = Heavier Adult,
     * 36-50    = Thin Adult,
     * 50-100   = Average Adult
     */
    int randHumanType = rand() % 100 + 1;

    /* Set weight base on type*/
    if(randHumanType >= 1 && randHumanType <= 15)
    {
        /* Generate weight from 10 to 35 kg */
        int max = 35;
        int min = 15;

        if(isResident)
        {
            simulatedHumanObj.back()->setWeight(rand() % (max - min) + min);

            /* Generate 1-100
             * 1-90     = Travel only to their residence,
             * 90-100    = Travel to other floor to find friends
             */
            int travellingType = rand() % 100 + 1;
            if(travellingType >= 90 && travellingType <= 100)
            {
                /* Generate number of friends the object visiting */
                int noOfFriends = rand() % TRAVEL_MAX + 1;
                for(int i=0;i<noOfFriends;i++)
                    simulatedHumanObj.back()->addFloorTravelling(rand() % totalNoOfFloors + 2);
            }
        }
        else
        {
            nonResSimulatedHumanObj.back()->setWeight(rand() % (max - min) + min);
            nonResSimulatedHumanObj.back()->addFloorTravelling(rand() % totalNoOfFloors + 2);
        }
    }
    else if(randHumanType >= 21 && randHumanType <= 30)
    {
        /* Generate weight from 80 to 150 kg */
        int max = 150;
        int min = 80;

        if(isResident)
        {
            simulatedHumanObj.back()->setWeight(rand() % (max - min) + min);

            /* Generate 1-100
             * 1-95     = Travel only to their residence,
             * 95-100    = Travel to other floor to find friends
             */
            int travellingType = rand() % 100 + 1;
            if(travellingType >= 95 && travellingType <= 100)
            {
                /* Generate number of friends the object visiting */
                int noOfFriends = rand() % TRAVEL_MAX + 1;
                for(int i=0;i<noOfFriends;i++)
                    simulatedHumanObj.back()->addFloorTravelling(rand() % totalNoOfFloors + 2);
            }
        }
        else
        {
                nonResSimulatedHumanObj.back()->setWeight(rand() % (max - min) + min);
                nonResSimulatedHumanObj.back()->addFloorTravelling(rand() % totalNoOfFloors + 2);
        }
    }
    else if(randHumanType >= 36 && randHumanType <= 50)
    {
        /* Generate weight from 35 to 55 kg */
        int max = 55;
        int min = 35;

        if(isResident)
        {
            simulatedHumanObj.back()->setWeight(rand() % (max - min) + min);

            /* Generate 1-100
             * 1-90     = Travel only to their residence,
             * 90-100    = Travel to other floor to find friends
             */
            int travellingType = rand() % 100 + 1;
            if(travellingType >= 90 && travellingType <= 100)
            {
                /* Generate number of friends the object visiting */
                int noOfFriends = rand() % TRAVEL_MAX + 1;
                for(int i=0;i<noOfFriends;i++)
                    simulatedHumanObj.back()->addFloorTravelling(rand() % totalNoOfFloors + 2);
            }
        }
        else
        {
                nonResSimulatedHumanObj.back()->setWeight(rand() % (max - min) + min);
                nonResSimulatedHumanObj.back()->addFloorTravelling(rand() % totalNoOfFloors + 2);
        }
    }
    else
    {
        /* Generate weight from 55 to 79 kg */
        int max = 79;
        int min = 55;

        if(isResident)
        {
            simulatedHumanObj.back()->setWeight(rand() % (max - min) + min);

            /* Generate 1-100
             * 1-85     = Travel only to their residence,
             * 85-100    = Travel to other floor to find friends
             */
            int travellingType = rand() % 100 + 1;
            if(travellingType >= 85 && travellingType <= 100)
            {
                /* Generate number of friends the object visiting */
                int noOfFriends = rand() % TRAVEL_MAX + 1;
                for(int i=0;i<noOfFriends;i++)
                    simulatedHumanObj.back()->addFloorTravelling(rand() % totalNoOfFloors + 2);
            }
        }
        else
        {
                nonResSimulatedHumanObj.back()->setWeight(rand() % (max - min) + min);
                nonResSimulatedHumanObj.back()->addFloorTravelling(rand() % totalNoOfFloors + 2);
        }
    }
}

/* Remove last simulatedHuman from vector. Bool flag true=resident false=visitor */
void SimulatedHumanHandler::removeLastData(bool isResident)
{
    if(isResident)
    {
        /* Free memory of last data */
        delete simulatedHumanObj[simulatedHumanObj.size()-1];

        /* Remove from vector */
        simulatedHumanObj.erase(simulatedHumanObj.end() - 1);
    }
    else /* Delete visitor */
    {
        /* Free memory of last data */
        delete nonResSimulatedHumanObj[nonResSimulatedHumanObj.size()-1];

        /* Remove from vector */
        nonResSimulatedHumanObj.erase(nonResSimulatedHumanObj.end() - 1);
    }
}

/* Get vector size of floorTravelling */
int SimulatedHumanHandler::getFloorTravellingSize(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getFloorTravellingSize();
    else
        return nonResSimulatedHumanObj[idx]->getFloorTravellingSize();
}

/* Return number of resident objects */
int SimulatedHumanHandler::getNumberOfVisitorObj()
{
    return nonResSimulatedHumanObj.size();
}

/* Print all simulatedHuman object data for debugging */
void SimulatedHumanHandler::getAllPersonData()
{
    for(std::size_t i=0;i<simulatedHumanObj.size();i++)
    {
        std::size_t numberOfFloorTravelling = simulatedHumanObj[i]->getFloorTravellingSize();

        qDebug() << "PersonID" << simulatedHumanObj[i]->getPersonID();
        qDebug() << "Weight" << simulatedHumanObj[i]->getWeight();
        qDebug() << "Resident of" << simulatedHumanObj[i]->getResident();

        QDebug debug = qDebug();
        qDebug() << "Travelling: ";
        for(std::size_t j=0; j<numberOfFloorTravelling;j++)
            debug << getFloorTravelling(i, j, true);
    }

    for(std::size_t i=0;i<nonResSimulatedHumanObj.size();i++)
    {
        std::size_t numberOfFloorTravelling = nonResSimulatedHumanObj[i]->getFloorTravellingSize();

        qDebug() << "VisitorID" << nonResSimulatedHumanObj[i]->getPersonID();
        qDebug() << "Weight" << nonResSimulatedHumanObj[i]->getWeight();
        qDebug() << "Not a resident";

        QDebug debug = qDebug();
        qDebug() << "Travelling: ";
        for(std::size_t j=0; j<numberOfFloorTravelling;j++)
            debug << getFloorTravelling(i, j, false);
    }
}
