/*-----------------------------------------------------------------------------
 * File      - SimulatedHumanHandler.cpp
 * Author    - LEVAX
 * Purpose   - Handles SimulatedHuman object
 *---------------------------------------------------------------------------*/

#include "SimulatedHumanHandler.h"

/* Constructor */
SimulatedHumanHandler::SimulatedHumanHandler()
{
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
    sh->setPersonID(simulatedHumanObj.size()+1);
    simulatedHumanObj.push_back(sh);
}

/* Free and reset all simulatedHuman object in simulatedHumanObj vector */
void SimulatedHumanHandler::resetAll()
{
    if(simulatedHumanObj.size() != 0)
    {
        qDebug() << "Freeing memory for simulatedHuman";
        for(std::size_t i=0;i<simulatedHumanObj.size();i++)
        {
            delete simulatedHumanObj[i];
            qDebug() << "Memory cleared for simulatedHuman" << i+1;
        }
        simulatedHumanObj.clear();
    }
}

/* Return number of simulatedHuman objects */
int SimulatedHumanHandler::getNumberOfSimulatedHumanObject()
{
    return simulatedHumanObj.size();
}

/* Set person id */
void SimulatedHumanHandler::setPersonID(int personID)
{
    simulatedHumanObj[personID-1]->setPersonID(personID);
}

/* Set resident floor */
void SimulatedHumanHandler::setResident(int idx, int residentOf)
{
    simulatedHumanObj[idx-1]->setResident(residentOf);
}

/* Set weight */
void SimulatedHumanHandler::setWeight(int idx, int weight)
{
    simulatedHumanObj[idx-1]->setWeight(weight);
}

/* Add floor travelling */
void SimulatedHumanHandler::addFloorTravelling(int idx, int floor)
{
    simulatedHumanObj[idx-1]->addFloorTravelling(floor);
}

/* Return person id */
int SimulatedHumanHandler::getPersonID(int idx)
{
    return simulatedHumanObj[idx]->getPersonID();
}

/* Return resident floor */
int SimulatedHumanHandler::getResident(int idx)
{
    return simulatedHumanObj[idx]->getResident();
}

/* Return weight */
int SimulatedHumanHandler::getWeight(int idx)
{
    return simulatedHumanObj[idx]->getWeight();
}

/* Return floor travelling at index */
int SimulatedHumanHandler::getFloorTravelling(int idxOfPerson, int idxOfFloorTravelling)
{
    return simulatedHumanObj[idxOfPerson]->getFloorTravelling(idxOfFloorTravelling);
}

/* Print all simulatedHuman object data for debugging */
void SimulatedHumanHandler::getAllPersonData()
{

    for(std::size_t i=0;i<simulatedHumanObj.size();i++)
    {
        std::size_t numberOfFloorTravelling = simulatedHumanObj[i]->getFloorTravellingSize();

        qDebug() << "PersonID" << simulatedHumanObj[i]->getPersonID();
        qDebug() << "Weight" << simulatedHumanObj[i]->getWeight();
        if(simulatedHumanObj[i]->getResident() == 0)
            qDebug() << "Not a resident";
        else
            qDebug() << "Resident of" << simulatedHumanObj[i]->getResident();

        QDebug debug = qDebug();
        qDebug() << "Travelling: ";
        for(std::size_t j=0; j<numberOfFloorTravelling;j++)
            debug << getFloorTravelling(i, j);
    }
}

/* Populate simulatedHuman with weight, etc */
void SimulatedHumanHandler::populateSimulatedHuman(int totalNoOfFloors)
{
    /* Add own residence floor */
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
    else if(randHumanType >= 21 && randHumanType <= 30)
    {
        /* Generate weight from 80 to 150 kg */
        int max = 150;
        int min = 80;
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
    else if(randHumanType >= 36 && randHumanType <= 50)
    {
        /* Generate weight from 35 to 55 kg */
        int max = 55;
        int min = 35;
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
        /* Generate weight from 55 to 79 kg */
        int max = 79;
        int min = 55;
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
}

void SimulatedHumanHandler::removeLastData()
{
    /* Free memory of last data */
    delete simulatedHumanObj[simulatedHumanObj.size()-1];

    /* Remove from vector */
    simulatedHumanObj.erase(simulatedHumanObj.end() - 1);
}

int SimulatedHumanHandler::getFloorTravellingSize(int idx)
{
    return simulatedHumanObj[idx]->getFloorTravellingSize();
}
