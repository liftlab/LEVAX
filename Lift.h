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
    Lift(int, int, int);                /* Constructor */
    ~Lift();                            /* Destructor */

    /* Setters */
    void setLiftID(int);                /* Set lift id */
    void setLiftWeight(int);            /* Set lift weight */
    void setLiftSpeed(int);             /* Set lift speed */

    /* Getters */
    int getLiftID();                    /* Return lift id */
    int getLiftWeight();                /* Return lift weight */
    int getLiftSpeed();                 /* Return lift speed */

private:
    int liftID;
    int maxWeight;
    int speedSecPerFloor;
};

#endif // LIFT_H
