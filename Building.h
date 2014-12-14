#ifndef BUILDING_H
#define BUILDING_H

#include <iostream>
using namespace std;

class Building
{
public:
    Building();                 /* Constructor */
    ~Building();                /* Destructor */

    /* Setters */
    void setNoOfFloors(int);    /* Set number of floors */

    /* Getters */
    int getTotalFloor();        /* Return number of floors */

private:
    int noOfFloors;             /* Total number of floors in building model */

};

#endif // BUILDING_H
