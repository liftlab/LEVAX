/*-----------------------------------------------------------------------------
 * File      - Lift.h
 * Author    - LEVAX
 * Purpose   - Declare Lift structure and its functions.
 *---------------------------------------------------------------------------*/

#ifndef LIFT_H
#define LIFT_H

class Lift
{
public:
    Lift();                                 /* Constructor */
    Lift(int, int, double, int);            /* Constructor */
    ~Lift();                                /* Destructor */

    /* Setters */
    void setLiftID(int);                    /* Set lift id */
    void setLiftWeight(int);                /* Set lift weight */
    void setLiftSpeed(double);              /* Set lift speed */
    void setLiftDefaultFloor(int);          /* Set lift default floor */
    void setLiftCurrentFloor(int);          /* Set lift current floor */
    void setLiftDirection(int);             /* Set lift current direction */
    void setLiftCurrentWeight(int);         /* Set lift current weight */
    void setLiftFS(int);                    /* Set Lift FS (Figure of suitability) */

    /* Getters */
    int getLiftID();                        /* Return lift id */
    int getLiftWeight();                    /* Return lift weight */
    double getLiftSpeed();                  /* Return lift speed */
    int getLiftDefaultFloor();              /* Return lift default floor */
    int getLiftCurrentFloor();              /* Get lift current floor */
    int getLiftDirection();                 /* Get lift current direction */
    int getLiftCurrentWeight();             /* Get lift current weight */
    int getLiftFS();                        /* Get lift FS (Figure of suitability) */

private:
    int liftID;
    int maxWeight;
    double speedMetrePerSecond;
    int defaultFloor;

    int currentFloor;
    int direction;
    int currentWeight;

    int FS;
};

#endif // LIFT_H
