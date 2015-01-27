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

    /* Getters */
    int getLiftID();                        /* Return lift id */
    int getLiftWeight();                    /* Return lift weight */
    double getLiftSpeed();                  /* Return lift speed */
    int getLiftDefaultFloor();              /* Return lift default floor */

private:
    int liftID;
    int maxWeight;
    double speedMetrePerSecond;
    int defaultFloor;
};

#endif // LIFT_H
