/*-----------------------------------------------------------------------------
 * File      - Lift.cpp
 * Author    - LEVAX
 * Purpose   - Lift object class. Stores lift structure
 *---------------------------------------------------------------------------*/

#include "Lift.h"

/* Constructor */
Lift::Lift()
{
}

/* Constructor */
Lift::Lift(int liftID, int maxWeight, int speedMetrePerSecond, int defaultFloor)
{
    this->liftID = liftID;
    this->maxWeight = maxWeight;
    this->speedMetrePerSecond = speedMetrePerSecond;
    this->defaultFloor = defaultFloor;

    this->currentFloor = defaultFloor;
    this->direction = +1; // -1 = DOWN, +1 = UP
    this->currentWeight = 0;

    this->distanceLeft = 0;
    this->travellingTo = defaultFloor;
    this->isMoving = false;
    this->moveNextRound = false;

    this->distanceCount = 0;
    this->totalDistance = 0;

    this->parkingLift = false;
}

/* Destructor */
Lift::~Lift()
{

}

/* Set lift id */
void Lift::setLiftID(int liftID)
{
    this->liftID = liftID;
}

/* Set lift weight */
void Lift::setLiftWeight(int maxWeight)
{
    this->maxWeight = maxWeight;
}

/* Set lift speed */
void Lift::setLiftSpeed(int speedMetrePerSecond)
{
    this->speedMetrePerSecond = speedMetrePerSecond;
}

/* Set lift default floor */
void Lift::setLiftDefaultFloor(int defaultFloor)
{
    this->defaultFloor = defaultFloor;
}

/* Set lift current floor */
void Lift::setLiftCurrentFloor(int currentFloor)
{
    this->currentFloor = currentFloor;
}

/* Set lift current direction */
void Lift::setLiftDirection(int direction)
{
    this->direction = direction;
}

/* Set lift current weight */
void Lift::setLiftCurrentWeight(int currentWeight)
{
    this->currentWeight = currentWeight;
}

/* Set lift moving status */
void Lift::setLiftIsMoving(bool isMoving)
{
    this->isMoving = isMoving;
}

/* Set Lift distance left to floor travellingTo */
void Lift::setLiftDistanceLeft(double distanceLeft)
{
    this->distanceLeft = distanceLeft;
}

/* Set Lift travelling to */
void Lift::setLiftTravellingTo(int travellingTo)
{
    this->travellingTo = travellingTo;
}

/* Set Lift to move only next second */
void Lift::setLiftMoveNextRound(bool moveNextRound)
{
    this->moveNextRound = moveNextRound;
}

/* Set Lift distanceCount */
void Lift::setLiftDistanceCount(int val)
{
    this->distanceCount = val;
}

/* Set Lift totalDistance */
void Lift::setLiftTotalDistance(int val)
{
    this->totalDistance = val;
}

/* Set Lift park */
void Lift::setLiftPark(bool parkingLift)
{
    this->parkingLift = parkingLift;
}

/* Return lift id */
int Lift::getLiftID()
{
    return liftID;
}

/* Return lift weight */
int Lift::getLiftWeight()
{
    return maxWeight;
}

/* Return lift speed */
int Lift::getLiftSpeed()
{
    return speedMetrePerSecond;
}

/* Return lift default floor */
int Lift::getLiftDefaultFloor()
{
    return defaultFloor;
}

/* Get lift current floor */
int Lift::getLiftCurrentFloor()
{
    return currentFloor;
}

/* Get lift current direction */
int Lift::getLiftDirection()
{
    return direction;
}

/* Get lift current weight */
int Lift::getLiftCurrentWeight()
{
    return currentWeight;
}

/* Get Lift total distance to next floor (travellingTo) */
bool Lift::getLiftIsMoving()
{
    return isMoving;
}

/* Get Lift distance left to floor travellingTo */
double Lift::getLiftDistanceLeft()
{
    return distanceLeft;
}

/* Set Lift travelling to */
int Lift::getLiftTravellingTo()
{
    return travellingTo;
}

/* Get Lift to move only next second */
bool Lift::getLiftMoveNextRound()
{
    return moveNextRound;
}

/* Get Lift distanceCount */
int Lift::getLiftDistanceCount()
{
    return distanceCount;
}

/* Get Lift totalDistance */
int Lift::getLiftTotalDistance()
{
    return totalDistance;
}

/* Get Lift park */
bool Lift::getLiftPark()
{
    return parkingLift;
}

/* Reset lift data */
void Lift::resetLift(int noOfFloors)
{
    currentFloor = defaultFloor;

    if(currentFloor == noOfFloors)
        direction = -1;
    else
        direction = +1;

    currentWeight = 0;
    distanceLeft = 0;
    travellingTo = defaultFloor;
    isMoving = false;
    moveNextRound = false;
    isMoving = false;
    moveNextRound = false;
    distanceCount = 0;
    totalDistance = 0;
    parkingLift = false;
}
