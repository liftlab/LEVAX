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
    void setHouseholdPerFloor(int); /* Set household per floor */

    /* Getters */
    int getNoOfFloor();        /* Return number of floors */
    int getNoOfLifts();         /* Return number of lifts */
    int getMetrePerFloor();     /* Return metre per floor */
    int getHouseholdPerFloor(); /* Return number of household per floor */

private:
    int noOfFloors;             /* Total number of floors in building model */
    int noOfLifts;              /* Total number of lifts in the building model */
    int metrePerFloor;          /* Specify the number of metres betwen each floor */
    int householdPerFloor;      /* Specify the number of household per floor */

};

#endif // BUILDING_H
