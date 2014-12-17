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

    void createNewLift(int, int, int);          /* Create new lift object and store in vector*/
    void resetAll();                            /* Free and reset all lift object in liftObj vector */
    void getAllLiftData();                      /* Print all lift object data for debugging */
    int getNumberOfLiftsObject();               /* Return number of lift objects */

    /* Setters */
    void setLiftID(int, int);                   /* Set lift id */
    void setLiftWeight(int, int);               /* Set lift weight */
    void setLiftSpeed(int, int);                /* Set lift speed */

    /* Getters */
    int getLiftID(int);                         /* Return lift id */
    int getLiftWeight(int);                     /* Return lift weight */
    int getLiftSpeed(int);                      /* Return lift speed */

private:
    std::vector<Lift*> liftObj;
};

#endif // LIFTHANDLER_H
