/*-----------------------------------------------------------------------------
 * File      - LiftHandler.h
 * Author    - LEVAX
 * Purpose   - Declare function and objects to set and get data
 *             for Lift Object
 *---------------------------------------------------------------------------*/

#ifndef LIFTHANDLER_H
#define LIFTHANDLER_H

#include "Lift.h"
#include <vector>   /* STL container*/
#include <qDebug>   /* Used as a replacement for cout */

class LiftHandler
{
public:
    LiftHandler();                              /* Constructor */
    ~LiftHandler();                             /* Destructor */

    void createNewLift(int, int, double, int);  /* Create new lift object and store in vector*/
    void resetAll();                            /* Free and reset all lift object in liftObj vector */
    int getNumberOfLiftsObject();               /* Return number of lift objects */
    void removeLastLift();                      /* Remove last lift */

    /* Setters */
    void setLiftID(int, int);                   /* Set lift id */
    void setLiftWeight(int, int);               /* Set lift weight */
    void setLiftSpeed(int, double);             /* Set lift speed */
    void setLiftDefaultFloor(int, int);         /* Set lift default floor */
    void setLiftCurrentFloor(int, int);         /* Set lift current floor */
    void setLiftDirection(int, int);            /* Set lift current direction */
    void setLiftCurrentWeight(int, int);        /* Set lift current weight */
    void setLiftDistanceLeft(int, double);      /* Set Lift distance left to floor travellingTo */
    void setLiftTravellingTo(int, int);         /* Set Lift travelling to */

    /* Getters */
    int getLiftID(int);                         /* Return lift id */
    int getLiftWeight(int);                     /* Return lift weight */
    double getLiftSpeed(int);                   /* Return lift speed */
    int getLiftDefaultFloor(int);               /* Return lift default floor */
    int getLiftCurrentFloor(int);               /* Get lift current floor */
    int getLiftDirection(int);                  /* Get lift current direction */
    int getLiftCurrentWeight(int);              /* Get lift current weight */
    double getLiftDistanceLeft(int);            /* Get Lift distance left to floor travellingTo */
    int getLiftTravellingTo(int);               /* Set Lift travelling to */
    Lift* getLiftObj(int idx);

private:
    std::vector<Lift*> liftObj;
};

#endif // LIFTHANDLER_H
