/*-----------------------------------------------------------------------------
 * File      - Algorithm.cpp
 * Author    - LEVAX
 * Purpose   - Algorithm object class. Four Different Algorithms
 *---------------------------------------------------------------------------*/

#include "Algorithm.h"

/* Empty Constructor */
Algorithm::Algorithm()
{
}

/* Destructor */
Algorithm::~Algorithm()
{

}

/* Constructor taking 3 parameters as arguments */
Algorithm::Algorithm(BuildingHandler bh, LiftHandler lh, SimulatedHumanHandler shh)
{
    this->bh = bh;
    this->lh = lh;
    this->shh = shh;
}

/* Nearest Car algorithm */
double Algorithm::nearestCar()
{
    return lh.getLiftSpeed(0);
}

/* Fixed Sectoring Common Sector System algorithm */
double Algorithm::fixedSectoringCommonSectorSystem()
{
    return lh.getLiftSpeed(0);
}

/* Fixed Sectoring Priority Timed System algorithm */
double Algorithm::fixedSectoringPriorityTimedSystem()
{
    return lh.getLiftSpeed(0);
}

/* Dynamic Sectoring System algorithm */
double Algorithm::dynamicSectoringSystem()
{
    return lh.getLiftSpeed(0);
}

