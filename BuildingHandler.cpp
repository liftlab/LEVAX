/*-----------------------------------------------------------------------------
 * File      - BuildingHandler.cpp
 * Author    - LEVAX
 * Purpose   - Handles Building object
 *---------------------------------------------------------------------------*/

#include "BuildingHandler.h"

/* Contructor */
BuildingHandler::BuildingHandler()
{

}

/* Destructor */
BuildingHandler::~BuildingHandler()
{
}

/* Set number of floors */
void BuildingHandler::setNoOfFloors(int noOfFloors)
{
    building.setNoOfFloors(noOfFloors);
}

/* Set number of lifts */
void BuildingHandler::setNoOfLifts(int noOfLifts)
{
    building.setNoOfLifts(noOfLifts);
}

/* Set metres per floor */
void BuildingHandler::setMetrePerFloor(int metrePerFloor)
{
    building.setMetrePerFloor(metrePerFloor);
}

/* Reset building model to default */
void BuildingHandler::resetAll()
{
    building.setNoOfFloors(2);
    building.setNoOfLifts(1);
    building.setMetrePerFloor(3);
}

/* Return number of floors */
int BuildingHandler::getNoOfFloor()
{
    return building.getNoOfFloor();
}

/* Return number of lifts */
int BuildingHandler::getNoOfLifts()
{
    return building.getNoOfLifts();
}

/* Return metre per floor */
int BuildingHandler::getMetrePerFloor()
{
    return building.getMetrePerFloor();
}
