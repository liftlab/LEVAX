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

    void createNewLift(int, int, int, int);     /* Create new lift object and store in vector*/
    void resetAll();                            /* Free and reset all lift object in liftObj vector */
    int getNumberOfLiftsObject();               /* Return number of lift objects */
    void removeLastLift();                      /* Remove last lift */

    /* Setters */
    void setLiftID(int, int);                   /* Set lift id */
    void setLiftWeight(int, int);               /* Set lift weight */
    void setLiftSpeed(int, int);                /* Set lift speed */
    void setLiftDefaultFloor(int, int);         /* Set lift default floor */
    void setLiftCurrentFloor(int, int);         /* Set lift current floor */
    void setLiftDirection(int, int);            /* Set lift current direction */
    void setLiftCurrentWeight(int, int);        /* Set lift current weight */
    void setLiftIsMoving(int, bool);            /* Set lift moving status */
    void setLiftDistanceLeft(int, double);      /* Set Lift distance to next target (travellingTo) */
    void setLiftTravellingTo(int, int);         /* Set Lift travelling to */
    void setLiftMoveNextRound(int, bool);       /* Set Lift to move only next second */
    void setLiftDistanceCount(int, int);        /* Set Lift distanceCount */
    void setLiftTotalDistance(int, int);        /* Set Lift totalDistance */
    void setLiftPark(int, bool);                /* Set Lift park */
    void setLiftPickPassenger(int, bool);       /* Set Lift isPicking */
    void increaseDistance(int, int);            /* Increase lift distance moved */

    /* Getters */
    int getLiftID(int);                         /* Return lift id */
    int getLiftWeight(int);                     /* Return lift weight */
    int getLiftSpeed(int);                      /* Return lift speed */
    int getLiftDefaultFloor(int);               /* Return lift default floor */
    int getLiftCurrentFloor(int);               /* Get lift current floor */
    int getLiftDirection(int);                  /* Get lift current direction */
    int getLiftCurrentWeight(int);              /* Get lift current weight */
    bool getLiftIsMoving(int);                  /* Get lift moving status */
    double getLiftDistanceLeft(int);            /* Get Lift distance to next target (travellingTo) */
    int getLiftTravellingTo(int);               /* Set Lift travelling to */
    bool getLiftMoveNextRound(int);             /* Get Lift to move only next second */
    int getLiftDistanceCount(int);              /* Get Lift distanceCount */
    int getLiftTotalDistance(int);              /* Get Lift totalDistance */
    bool getLiftPark(int);                      /* Get Lift park */
    bool getLiftPickPassenger(int);             /* Get Lift isPicking */

    void resetLift(int);                        /* Reset lift to default values after simulation completed */

private:
    std::vector<Lift*> liftObj;
};

#endif // LIFTHANDLER_H
