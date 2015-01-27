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
