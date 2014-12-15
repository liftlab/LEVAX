#include "BuildingHandler.h"

/* Contructor */
BuildingHandler::BuildingHandler()
{
    building.setNoOfFloors(0);
    building.setNoOfLifts(0);
    building.setMetrePerFloor(0);
    building.setHouseholdPerFloor(0);
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

/* Set household per floor */
void BuildingHandler::setHouseholdPerFloor(int householdPerFloor)
{
    building.setHouseholdPerFloor(householdPerFloor);
}

void BuildingHandler::resetAll()
{
    building.setNoOfFloors(0);
    building.setNoOfLifts(0);
    building.setMetrePerFloor(0);
    building.setHouseholdPerFloor(0);
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

/* Return number of household per floor */
int BuildingHandler::getHouseholdPerFloor()
{
    return building.getHouseholdPerFloor();
}
