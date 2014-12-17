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
void SimulatedHumanHandler::createSimulatedHuman(int personID, int residentOf)
{
    /* Create new object */
    SimulatedHuman *sh = new SimulatedHuman(personID, residentOf);
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
void SimulatedHumanHandler::setPersonID(int idx, int personID)
{
    simulatedHumanObj[idx-1]->setPersonID(personID);
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
    return simulatedHumanObj[idx-1]->getPersonID();
}

/* Return resident floor */
int SimulatedHumanHandler::getResident(int idx)
{
    return simulatedHumanObj[idx-1]->getResident();
}

/* Return weight */
int SimulatedHumanHandler::getWeight(int idx)
{
    return simulatedHumanObj[idx-1]->getWeight();
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
