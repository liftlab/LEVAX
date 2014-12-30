/*-----------------------------------------------------------------------------
 * File      - Building.h
 * Author    - LEVAX
 * Purpose   - Declare building structure and its functions.
 *---------------------------------------------------------------------------*/

#ifndef BUILDING_H
#define BUILDING_H

class Building
{
public:
    Building();                 /* Constructor */
    ~Building();                /* Destructor */

    /* Setters */
    void setNoOfFloors(int);    /* Set number of floors */
    void setNoOfLifts(int);     /* Set number of lifts */
    void setMetrePerFloor(int); /* Set metres per floor */

    /* Getters */
    int getNoOfFloor();         /* Return number of floors */
    int getNoOfLifts();         /* Return number of lifts */
    int getMetrePerFloor();     /* Return metre per floor */

private:
    int noOfFloors;             /* Total number of floors in building model */
    int noOfLifts;              /* Total number of lifts in the building model */
    int metrePerFloor;          /* Specify the number of metres betwen each floor */

};

#endif // BUILDING_H
