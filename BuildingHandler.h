/*-----------------------------------------------------------------------------
 * File      - BuildingHandler.h
 * Author    - LEVAX
 * Purpose   - Declare function and objects to set and get data
 *             for Building Object
 *---------------------------------------------------------------------------*/

#ifndef BUILDINGHANDLER_H
#define BUILDINGHANDLER_H

#include "Building.h"

class BuildingHandler
{
public:
    BuildingHandler();          /* Constructor */
    ~BuildingHandler();         /* Destructor */

    /* Setters */
    void setNoOfFloors(int);    /* Set number of floors */
    void setNoOfLifts(int);     /* Set number of lifts */
    void setMetrePerFloor(int); /* Set metres per floor */
    void setHouseholdPerFloor(int); /* Set household per floor */

    void resetAll();            /* reset all to zero */

    /* Getters */
    int getNoOfFloor();         /* Return number of floors */
    int getNoOfLifts();         /* Return number of lifts */
    int getMetrePerFloor();     /* Return metre per floor */
    int getHouseholdPerFloor(); /* Return number of household per floor */

private:
    Building building;          /* Building object */

};

#endif // BUILDINGHANDLER_H
