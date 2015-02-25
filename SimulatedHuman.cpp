/*-----------------------------------------------------------------------------
 * File      - SimulatedHuman.cpp
 * Author    - LEVAX
 * Purpose   - SimulatedHuman object class. Stores SimulatedHuman structure
 *---------------------------------------------------------------------------*/

#include <SimulatedHuman.h>

/* Empty Constructor */
SimulatedHuman::SimulatedHuman()
{
}

/* Constructor */
SimulatedHuman::SimulatedHuman(int residentOf)
{
    this->personID = 0;
    this->residentOf = residentOf;
    this->weight = 0;
}

/* Destructor */
SimulatedHuman::~SimulatedHuman()
{
}

/* Set person id */
void SimulatedHuman::setPersonID(int personID)
{
    this->personID = personID;
}

/* Set resident floor */
void SimulatedHuman::setResident(int residentOf)
{
    this->residentOf = residentOf;
}

/* Set weight */
void SimulatedHuman::setWeight(int weight)
{
    this->weight = weight;
}

/* Add time travel into building */
void SimulatedHuman::addTravelTime(int timeIn, int floor)
{
    this->timeTravel.push_back(make_pair<int, int>(timeIn, floor));
}

/* Set the travel destination */
void SimulatedHuman::setTravelLevel(int idx, int value)
{
    timeTravel[idx].second = value;
}

/* Return person id */
int SimulatedHuman::getPersonID()
{
    return personID;
}

/* Return resident floor */
int SimulatedHuman::getResident()
{
    return residentOf;
}

/* Return weight */
int SimulatedHuman::getWeight()
{
    return weight;
}

/* Return time travel inside building */
int SimulatedHuman::getTravelTime(int idx)
{
    return this->timeTravel[idx].first;
}

/* Return the floor travelling respective to the time */
int SimulatedHuman::getTravelFloor(int idx)
{
    return this->timeTravel[idx].second;
}

/* Return number of times travel in and out of building */
int SimulatedHuman::getNoOfTimesTravel()
{
   return this->timeTravel.size();
}
