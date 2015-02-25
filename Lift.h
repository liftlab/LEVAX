/*-----------------------------------------------------------------------------
 * File      - Lift.h
 * Author    - LEVAX
 * Purpose   - Declare Lift structure and its functions.
 *---------------------------------------------------------------------------*/

#ifndef LIFT_H
#define LIFT_H

#include <map>

class Lift
{
public:
    Lift();                                 /* Constructor */
    Lift(int, int, int, int);               /* Constructor */
    ~Lift();                                /* Destructor */

    /* Setters */
    void setLiftID(int);                    /* Set lift id */
    void setLiftWeight(int);                /* Set lift weight */
    void setLiftSpeed(int);                 /* Set lift speed */
    void setLiftDefaultFloor(int);          /* Set lift default floor */
    void setLiftCurrentFloor(int);          /* Set lift current floor */
    void setLiftDirection(int);             /* Set lift current direction */
    void setLiftCurrentWeight(int);         /* Set lift current weight */
    void setLiftIsMoving(bool);             /* Set lift moving status*/
    void setLiftDistanceLeft(double);       /* Set Lift distance to next target (travellingTo) */
    void setLiftTravellingTo(int);          /* Set Lift travelling to */
    void setLiftMoveNextRound(bool);        /* Set Lift to move only next second */
    void setLiftDistanceCount(int);         /* Set Lift distanceCount */
    void setLiftTotalDistance(int);         /* Set Lift totalDistance */
    void setLiftPark(bool);                 /* Set Lift park */
    void setLiftPickPassenger(bool);        /* Set Lift isPicking */

    /* Getters */
    int getLiftID();                        /* Return lift id */
    int getLiftWeight();                    /* Return lift weight */
    int getLiftSpeed();                     /* Return lift speed */
    int getLiftDefaultFloor();              /* Return lift default floor */
    int getLiftCurrentFloor();              /* Get lift current floor */
    int getLiftDirection();                 /* Get lift current direction */
    int getLiftCurrentWeight();             /* Get lift current weight */
    bool getLiftIsMoving();                 /* Get lift moving status */
    double getLiftDistanceLeft();           /* Get Lift distance to next target (travellingTo)*/
    int getLiftTravellingTo();              /* Get Lift travelling to */
    bool getLiftMoveNextRound();            /* Get Lift to move only next second */
    int getLiftDistanceCount();             /* Get Lift distanceCount */
    int getLiftTotalDistance();             /* Get Lift totalDistance */
    bool getLiftPark();                     /* Get Lift park */
    bool getLiftPickPassenger();            /* Get Lift isPicking */

    void resetLift(int);                    /* Reset lift variables to default values after simulation */

private:
    int liftID;
    int maxWeight;
    int speedMetrePerSecond;
    int defaultFloor;

    /* Values should be reset to default after simulation completed */
    int currentFloor;
    int currentWeight;
    int direction;
    double distanceLeft;
    double distanceCount;
    double totalDistance;
    int travellingTo;
    bool isMoving;
    bool moveNextRound;
    bool parkingLift;
    bool isPicking;

};

#endif // LIFT_H
