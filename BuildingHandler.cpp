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
void BuildingHandler::createBuilding(int noOfFloors)
{
        //building = Building(noOfFloors);
    building.setNoOfFloors(noOfFloors);
}

/* Return number of floors */
int BuildingHandler::getTotalFloor()
{
    return building.getTotalFloor();
}
