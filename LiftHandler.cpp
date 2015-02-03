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

/* Set Lift distance left to floor travellingTo */
void LiftHandler::setLiftDistanceLeft(int idx, double distanceLeft)
{
    liftObj[idx]->setLiftDistanceLeft(distanceLeft);
}

/* Set Lift travelling to */
void LiftHandler::setLiftTravellingTo(int idx, int travellingTo)
{
    liftObj[idx]->setLiftTravellingTo(travellingTo);
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

/* Get Lift distance left to floor travellingTo */
double LiftHandler::getLiftDistanceLeft(int idx)
{
    return liftObj[idx]->getLiftDistanceLeft();
}

/* Set Lift travelling to */
int LiftHandler::getLiftTravellingTo(int idx)
{
     return liftObj[idx]->getLiftTravellingTo();
}

/* Remove last lift */
void LiftHandler::removeLastLift()
{
    /* Free memory of last data */
    delete liftObj[liftObj.size()-1];

    /* Remove from vector */
    liftObj.erase(liftObj.end() - 1);
}

Lift* LiftHandler::getLiftObj(int idx)
{
    return liftObj[idx];
}
