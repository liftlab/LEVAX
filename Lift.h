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
    Lift(int, int, double);                /* Constructor */
    ~Lift();                            /* Destructor */

    /* Setters */
    void setLiftID(int);                /* Set lift id */
    void setLiftWeight(int);            /* Set lift weight */
    void setLiftSpeed(double);             /* Set lift speed */

    /* Getters */
    int getLiftID();                    /* Return lift id */
    int getLiftWeight();                /* Return lift weight */
    double getLiftSpeed();                 /* Return lift speed */

private:
    int liftID;
    int maxWeight;
    double speedMetrePerSecond;
};

#endif // LIFT_H
