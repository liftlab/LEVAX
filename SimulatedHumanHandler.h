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

#define TRAVEL_MAX 3                                    /* Maximum no of floors people will travel */
#define MAX_PEOPLE 1000                                 /* Maximum no of people */
#define MAX_VISITOR 500                                 /* Maximum no of visitor */

class SimulatedHumanHandler
{
public:
    SimulatedHumanHandler();                            /* Constructor */
    ~SimulatedHumanHandler();                           /* Destructor */

    void createSimulatedHuman(int);                     /* Create simulatedHuman */
    void resetAll();                                    /* Free and reset all simulatedHuman object in simulatedHumanObj vector */
    int getNumberOfSimulatedHumanObject();              /* Return number of simulatedHuman objects */
    int getNumberOfVisitorObj();                        /* Return number of resident objects */
    void removeLastData(bool);                          /* Remove last simulatedHuman from vector */
    void populateSimulatedHuman(int, int, bool);        /* Populate simulatedHuman with weight, etc */
    void removeFloorTravelling(int, int, bool);         /* Remove floor travelling of person idx with key */
    void updateFloorsEvenly(int, int, int);             /* Update floor evenly based on average number of people per floor */

    /* Setters */
    void setPersonID(int, int, bool);                   /* Set person id */
    void setResident(int, int, bool);                   /* Set resident floor */
    void setWeight(int, int, bool);                     /* Set weight */
    void addFloorTravelling(int, int, bool);            /* Add floor travelling */

    /* Getters */
    int getPersonID(int, bool);                         /* Return person id */
    int getResident(int, bool);                         /* Return resident floor */
    int getWeight(int, bool);                           /* Return weight */
    int getFloorTravelling(int, int, bool);             /* Return floor travelling at index */
    int getFloorTravellingSize(int, bool);              /* Get vector size of floorTravelling */

    /* FOR DEBUGGING PURPOSES ONLY */
    void getAllPersonData();                            /* Print all simulatedHuman object data for debugging */

private:
    std::vector<SimulatedHuman*> simulatedHumanObj;         /* Stores resident */
    std::vector<SimulatedHuman*> nonResSimulatedHumanObj;   /* Stores visitor */
};


#endif // SIMULATEDHUMANHANDLER_H
