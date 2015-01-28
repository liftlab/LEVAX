/*-----------------------------------------------------------------------------
 * File      - LiftHandler.cpp
 * Author    - LEVAX
 * Purpose   - Handles Lift object
 *---------------------------------------------------------------------------*/

#include "LiftHandler.h"

/* Constructor */
LiftHandler::LiftHandler()
{
    createNewLift(1, 200, 1.00, 1);
}

/* Destructor */
LiftHandler::~LiftHandler()
{
   resetAll();
}

/* Create new lift object and store in vector*/
void LiftHandler::createNewLift(int liftID, int maxWeight, double speedMetrePerSecond, int defaultFloor)
{
    Lift *l = new Lift(liftID, maxWeight, speedMetrePerSecond, defaultFloor);

    liftObj.push_back(l);
}

/* Free and reset all lift object in liftObj vector */
void LiftHandler::resetAll()
{
    if(liftObj.size() != 0)
    {
        qDebug() << "Freeing memory for lift";

        for(std::size_t i=0;i<liftObj.size();i++)
        {
            delete liftObj[i];
            qDebug() << "Memory cleared for lift" << i+1;
        }

        liftObj.clear();
    }
}

/* Print all lift object data for debugging */
void LiftHandler::getAllLiftData()
{
    for(std::size_t i=0;i<liftObj.size();i++)
    {
        qDebug() << "Lift ID" << liftObj[i]->getLiftID();
        qDebug() << "Max Weight" << liftObj[i]->getLiftWeight();
        qDebug() << "Speed(sec/floor)" << liftObj[i]->getLiftSpeed();
    }
}

/* Return number of lift objects */
int LiftHandler::getNumberOfLiftsObject()
{
    return liftObj.size();
}

/* Set lift id */
void LiftHandler::setLiftID(int idx, int liftID)
{
    liftObj[idx]->setLiftID(liftID);
}

/* Set lift weight */
void LiftHandler::setLiftWeight(int idx, int maxWeight)
{
    liftObj[idx]->setLiftWeight(maxWeight);
}

/* Set lift speed */
void LiftHandler::setLiftSpeed(int idx, double speedMetrePerSecond)
{
    liftObj[idx]->setLiftSpeed(speedMetrePerSecond);
}

/* Set lift default floor */
void LiftHandler::setLiftDefaultFloor(int idx, int defaultFloor)
{
    liftObj[idx]->setLiftDefaultFloor(defaultFloor);
}

/* Set lift current floor */
void LiftHandler::setLiftCurrentFloor(int idx, int currentFloor)
{
    liftObj[idx]->setLiftCurrentFloor(currentFloor);
}

/* Set lift current direction */
void LiftHandler::setLiftDirection(int idx, int direction)
{
    liftObj[idx]->setLiftDirection(direction);
}

/* Set lift current weight */
void LiftHandler::setLiftCurrentWeight(int idx, int currentWeight)
{
    liftObj[idx]->setLiftCurrentWeight(currentWeight);
}

/* Set lift FS (Figure of suitability) */
void LiftHandler::setLiftFS(int idx, int FS)
{
    liftObj[idx]->setLiftFS(FS);
}

/* Return lift id */
int LiftHandler::getLiftID(int idx)
{
    return liftObj[idx]->getLiftID();
}

/* Return lift weight */
int LiftHandler::getLiftWeight(int idx)
{
    return liftObj[idx]->getLiftWeight();
}

/* Return lift speed */
double LiftHandler::getLiftSpeed(int idx)
{
    return liftObj[idx]->getLiftSpeed();
}

/* Return lift default floor */
int LiftHandler::getLiftDefaultFloor(int idx)
{
    return liftObj[idx]->getLiftDefaultFloor();
}

/* Get lift current floor */
int LiftHandler::getLiftCurrentFloor(int idx)
{
    return liftObj[idx]->getLiftCurrentFloor();
}

/* Get lift current direction */
int LiftHandler::getLiftDirection(int idx)
{
    return liftObj[idx]->getLiftDirection();
}

/* Get lift current weight */
int LiftHandler::getLiftCurrentWeight(int idx)
{
    return liftObj[idx]->getLiftCurrentWeight();
}

/* Get lift FS (Figure of suitability) */
int LiftHandler::getLiftFS(int idx)
{
    return liftObj[idx]->getLiftFS();
}

/* Remove last lift */
void LiftHandler::removeLastLift()
{
    /* Free memory of last data */
    delete liftObj[liftObj.size()-1];

    /* Remove from vector */
    liftObj.erase(liftObj.end() - 1);
}


