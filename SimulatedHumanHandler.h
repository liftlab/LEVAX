/*-----------------------------------------------------------------------------
 * File      - SimulatedHumanHandler.h
 * Author    - LEVAX
 * Purpose   - Declare function and objects to set and get data
 *             for SimulatedHuman Object
 *---------------------------------------------------------------------------*/

#ifndef SIMULATEDHUMANHANDLER_H
#define SIMULATEDHUMANHANDLER_H

#include "vector"
#include "SimulatedHuman.h"
#include <qDebug>

class SimulatedHumanHandler
{
public:
    SimulatedHumanHandler();                            /* Constructor */
    ~SimulatedHumanHandler();                           /* Destructor */

    void createSimulatedHuman(int, int);                /* Create simulatedHuman */
    void resetAll();                                    /* Free and reset all simulatedHuman object in simulatedHumanObj vector */
    int getNumberOfSimulatedHumanObject();              /* Return number of simulatedHuman objects */

    /* Setters */
    void setPersonID(int, int);                         /* Set person id */
    void setResident(int, int);                         /* Set resident floor */
    void setWeight(int, int);                           /* Set weight */
    void addFloorTravelling(int, int);                  /* Add floor travelling */

    /* Getters */
    int getPersonID(int);                               /* Return person id */
    int getResident(int);                               /* Return resident floor */
    int getWeight(int);                                 /* Return weight */
    int getFloorTravelling(int, int);                   /* Return floor travelling at index */

    /* FOR DEBUGGING PURPOSES ONLY */
    void getAllPersonData();                            /* Print all simulatedHuman object data for debugging */

private:
    std::vector<SimulatedHuman*> simulatedHumanObj;
};


#endif // SIMULATEDHUMANHANDLER_H
