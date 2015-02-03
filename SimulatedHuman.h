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

    /* Setters */
    void setPersonID(int);                      /* Set person id */
    void setResident(int);                      /* Set resident floor */
    void setWeight(int);                        /* Set weight */
    void addTravelTime(int, int);               /* Add time travel into building */
    void setTravelLevel(int, int);

    /* Getters */
    int getPersonID();                          /* Return person id */
    int getResident();                          /* Return resident floor */
    int getWeight();                            /* Return weight */
    int getTravelTime(int);                     /* Return time travel inside building */
    int getTravelFloor(int);                    /* Return the floor travelling respective to the time */
    int getNoOfTimesTravel();                   /* Return number of times travel in and out of building */

private:
    int personID;
    int residentOf;
    int weight;
    std::vector<pair<int, int> > timeTravel;    /* Stores the time travel */
};

#endif // SIMULATEDHUMAN_H
