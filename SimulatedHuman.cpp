/*-----------------------------------------------------------------------------
 * File      - SimulatedHuman.cpp
 * Author    - LEVAX
 * Purpose   - SimulatedHuman object class. Stores SimulatedHuman structure
 *---------------------------------------------------------------------------*/

#include <SimulatedHuman.h>

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

/* Get size of floorTravelling */
int SimulatedHuman::getFloorTravellingSize()
{
    return floorTravelling.size();
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

/* Add floor travelling */
void SimulatedHuman::addFloorTravelling(int floor)
{
    this->floorTravelling[floor] = floor;
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

/* Return floor travelling at index*/
int SimulatedHuman::getFloorTravelling(int idx)
{
    std::map<int, int>::const_iterator it = floorTravelling.begin();
    /* iterate to idx position */
    for(int i = 0; i < idx; i++)
        it++;

    return it->second;
}
