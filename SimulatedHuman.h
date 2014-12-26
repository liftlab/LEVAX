/*-----------------------------------------------------------------------------
 * File      - SimulatedHuman.h
 * Author    - LEVAX
 * Purpose   - Declare SimulatedHuman structure and its functions.
 *---------------------------------------------------------------------------*/

#ifndef SIMULATEDHUMAN_H
#define SIMULATEDHUMAN_H

#include <map>

class SimulatedHuman
{
public:
    SimulatedHuman();                           /* Empty Constructor */
    SimulatedHuman(int);                        /* Constructor */
    ~SimulatedHuman();                          /* Destructor */

    int getFloorTravellingSize();               /* Get size of floorTravelling */

    /* Setters */
    void setPersonID(int);                      /* Set person id */
    void setResident(int);                      /* Set resident floor */
    void setWeight(int);                        /* Set weight */
    void addFloorTravelling(int);               /* Add floor travelling */

    /* Getters */
    int getPersonID();                          /* Return person id */
    int getResident();                          /* Return resident floor */
    int getWeight();                            /* Return weight */
    int getFloorTravelling(int);                /* Return floor travelling at index*/

private:
    int personID;
    int residentOf;
    int weight;
    std::map<int,int> floorTravelling;          /* Stores the floor the simulatedHumans will travel around */
};

#endif // SIMULATEDHUMAN_H
