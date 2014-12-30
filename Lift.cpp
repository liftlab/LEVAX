/*-----------------------------------------------------------------------------
 * File      - Lift.cpp
 * Author    - LEVAX
 * Purpose   - Lift object class. Stores lift structure
 *---------------------------------------------------------------------------*/

#include "Lift.h"

/* Constructor */
Lift::Lift(int liftID, int maxWeight, double speedMetrePerSecond)
{
    this->liftID = liftID;
    this->maxWeight = maxWeight;
    this->speedMetrePerSecond = speedMetrePerSecond;
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

