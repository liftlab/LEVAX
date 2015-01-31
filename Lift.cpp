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
Lift::Lift(int liftID, int maxWeight, double speedMetrePerSecond, int defaultFloor)
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
void Lift::setLiftSpeed(double speedMetrePerSecond)
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
double Lift::getLiftSpeed()
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
