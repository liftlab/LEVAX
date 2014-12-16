/*-----------------------------------------------------------------------------
 * File      - Building.cpp
 * Author    - LEVAX
 * Purpose   - Building object class. Stores building structure
 *---------------------------------------------------------------------------*/

#include "Building.h"

/* Constructor */
Building::Building()
{
    this->noOfFloors=0;
}

/* Destructor */
Building::~Building()
{
}

/* Set number of floors */
void Building::setNoOfFloors(int noOfFloors)
{
    this->noOfFloors=noOfFloors;
}

/* Set number of lifts */
void Building::setNoOfLifts(int noOfLifts)
{
    this->noOfLifts=noOfLifts;
}

/* Set metres per floor */
void Building::setMetrePerFloor(int metrePerFloor)
{
   this->metrePerFloor=metrePerFloor;
}

/* Set household per floor */
void Building::setHouseholdPerFloor(int householdPerFloor)
{
    this->householdPerFloor=householdPerFloor;
}

/* Return number of floors */
int Building::getNoOfFloor()
{
    return noOfFloors;
}

/* Return number of lifts */
int Building::getNoOfLifts()
{
    return noOfLifts;
}

/* Return metre per floor */
int Building::getMetrePerFloor()
{
    return metrePerFloor;
}

/* Return number of household per floor */
int Building::getHouseholdPerFloor()
{
    return householdPerFloor;
}
