#ifndef BUILDINGHANDLER_H
#define BUILDINGHANDLER_H

#include "Building.h"

class BuildingHandler
{
public:
    BuildingHandler();          /* Constructor */
    ~BuildingHandler();         /* Destructor */

    /* Setters */
    void createBuilding(int);   /* Set number of floors */

    /* Getters */
    int getTotalFloor();        /* Return number of floors */

private:
    Building building;          /* Building object */

};

#endif // BUILDINGHANDLER_H
