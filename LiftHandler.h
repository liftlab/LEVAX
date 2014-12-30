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

    void createNewLift(int, int, double);       /* Create new lift object and store in vector*/
    void resetAll();                            /* Free and reset all lift object in liftObj vector */
    int getNumberOfLiftsObject();               /* Return number of lift objects */
    void removeLastLift();                      /* Remove last lift */

    /* Setters */
    void setLiftID(int, int);                   /* Set lift id */
    void setLiftWeight(int, int);               /* Set lift weight */
    void setLiftSpeed(int, double);             /* Set lift speed */

    /* Getters */
    int getLiftID(int);                         /* Return lift id */
    int getLiftWeight(int);                     /* Return lift weight */
    double getLiftSpeed(int);                   /* Return lift speed */

    /* FOR DEBUGGING PURPOSES ONLY */
    void getAllLiftData();                      /* Print all lift object data for debugging */

private:
    std::vector<Lift*> liftObj;
};

#endif // LIFTHANDLER_H
