/*-----------------------------------------------------------------------------
 * File      - SimulatedHuman.h
 * Author    - LEVAX
 * Purpose   - Declare SimulatedHuman structure and its functions.
 *---------------------------------------------------------------------------*/

#ifndef SIMULATEDHUMAN_H
#define SIMULATEDHUMAN_H

#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class SimulatedHuman
{
public:
    SimulatedHuman();                           /* Empty Constructor */
    SimulatedHuman(int);                        /* Constructor */
    ~SimulatedHuman();                          /* Destructor */

    int getFloorTravellingSize();               /* Get size of floorTravelling */
    void removeFloorTravelling(int);            /* Remove floor travelling */

    /* Setters */
    void setPersonID(int);                      /* Set person id */
    void setResident(int);                      /* Set resident floor */
    void setWeight(int);                        /* Set weight */
    void addFloorTravelling(int);               /* Add floor travelling */
    void addTravelTime(int);                    /* Add time travel into building */
    void setNoOfTimesTravel(int);               /* Set number of times travel in and out of building */
    void setStatus(int);                        /* Set the status of the resident */

    /* Getters */
    int getPersonID();                          /* Return person id */
    int getResident();                          /* Return resident floor */
    int getWeight();                            /* Return weight */
    int getFloorTravelling(int);                /* Return floor travelling at index */
    int getTravelTime(int);                     /* Return time travel inside building */
    int getNoOfTimesTravel();                   /* Return number of times travel in and out of building */
    int getStatus();                            /* Return the status of the resident */

private:
    int personID;
    int residentOf;
    int weight;
    int noOfTimes;
    int status;
    std::map<int,int> floorTravelling;          /* Stores the floor the simulatedHumans will travel around */
    std::vector<int> timeTravel;    /* Stores the time travel */
};

#endif // SIMULATEDHUMAN_H
