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

/* Return number of floors */
int Building::getTotalFloor()
{
    return noOfFloors;
}

