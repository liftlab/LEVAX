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
    void addTravelTime(int, int, int, bool);            /* Add travel time */

    /* Getters */
    int getPersonID(int, bool);                         /* Return person id */
    int getResident(int, bool);                         /* Return resident floor */
    int getWeight(int, bool);                           /* Return weight */
    int getTravelTime(int, int, bool);                  /* Return the travel time */
    int getTravelFloor(int, int, bool);                 /* Return the travel floor with respect to the time */
    int getNoOfTimesTravel(int, bool);                  /* Return the number of travel times */
    SimulatedHuman* getSimulatedHuman(int, bool);

private:
    std::vector<SimulatedHuman*> simulatedHumanObj;         /* Stores resident */
    std::vector<SimulatedHuman*> nonResSimulatedHumanObj;   /* Stores visitor */
};


#endif // SIMULATEDHUMANHANDLER_H
