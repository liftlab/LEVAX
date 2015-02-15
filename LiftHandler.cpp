/*-----------------------------------------------------------------------------
 * File      - LiftHandler.cpp
 * Author    - LEVAX
 * Purpose   - Handles Lift object
 *---------------------------------------------------------------------------*/

#include "LiftHandler.h"

/* Constructor */
LiftHandler::LiftHandler()
{
    createNewLift(1, 400, 3, 1);
}

/* Destructor */
LiftHandler::~LiftHandler()
{
   resetAll();
}

/* Create new lift object and store in vector*/
void LiftHandler::createNewLift(int liftID, int maxWeight, int speedMetrePerSecond, int defaultFloor)
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
void LiftHandler::setLiftSpeed(int idx, int speedMetrePerSecond)
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

/* Set Lift distance moving status */
void LiftHandler::setLiftIsMoving(int idx, bool isMoving)
{
    liftObj[idx]->setLiftIsMoving(isMoving);
}

/* Set lift distance to next target (travellingTo) */
void LiftHandler::setLiftDistanceLeft(int idx, double distanceLeft)
{
    liftObj[idx]->setLiftDistanceLeft(distanceLeft);
}

/* Set Lift travelling to */
void LiftHandler::setLiftTravellingTo(int idx, int travellingTo)
{
    liftObj[idx]->setLiftTravellingTo(travellingTo);
}

/* Set Lift to move only next second */
void LiftHandler::setLiftMoveNextRound(int idx, bool moveNextRound)
{
    liftObj[idx]->setLiftMoveNextRound(moveNextRound);
}

/* Set Lift distanceCount */
void LiftHandler::setLiftDistanceCount(int idx, int val)
{
    liftObj[idx]->setLiftDistanceCount(val);
}

/* Set Lift totalDistance */
void LiftHandler::setLiftTotalDistance(int idx, int val)
{
    liftObj[idx]->setLiftTotalDistance(val);
}

/* Set Lift park */
void LiftHandler::setLiftPark(int idx, bool parkingLift)
{
    liftObj[idx]->setLiftPark(parkingLift);
}

/* set lift isPicking */
void LiftHandler::setLiftPickPassenger(int idx, bool isPicking)
{
    liftObj[idx]->setLiftPickPassenger(isPicking);
}

/* Increase lift distance moved */
void LiftHandler::increaseDistance(int idx, int val)
{
    int distanceCount = liftObj[idx]->getLiftDistanceCount();
    int totalDistance = liftObj[idx]->getLiftTotalDistance();
    liftObj[idx]->setLiftDistanceCount(distanceCount + val);
    liftObj[idx]->setLiftTotalDistance(totalDistance + val);
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
int LiftHandler::getLiftSpeed(int idx)
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

/* Get lift moving status */
bool LiftHandler::getLiftIsMoving(int idx)
{
    return liftObj[idx]->getLiftIsMoving();
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

/* Get Lift to move only next second */
bool LiftHandler::getLiftMoveNextRound(int idx)
{
    return liftObj[idx]->getLiftMoveNextRound();
}

/* Get Lift distanceCount */
int LiftHandler::getLiftDistanceCount(int idx)
{
    return liftObj[idx]->getLiftDistanceCount();
}

/* Get Lift totalDistance */
int LiftHandler::getLiftTotalDistance(int idx)
{
    return liftObj[idx]->getLiftTotalDistance();
}

/* Get Lift park */
bool LiftHandler::getLiftPark(int idx)
{
    return liftObj[idx]->getLiftPark();
}

/* Get lift isPicking */
bool LiftHandler::getLiftPickPassenger(int idx)
{
    return liftObj[idx]->getLiftPickPassenger();
}

void LiftHandler::resetLift(int noOfFloors)
{
    for(size_t i=0;i<liftObj.size();i++)
    {
        liftObj[i]->resetLift(noOfFloors);
    }
}

