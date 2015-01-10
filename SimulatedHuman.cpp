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
    this->noOfTimes = 0;
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

/* Add time travel into building */
void SimulatedHuman::addTravelTime(int timeIn)
{
    this->timeTravel.push_back(timeIn);
}

/* Set number of times travel in and out of building */
 void SimulatedHuman::setNoOfTimesTravel(int noOfTimes)
 {
     this->noOfTimes = noOfTimes;
 }

 /* Set the status of the resident */
void SimulatedHuman::setStatus(int status)
{
    this->status = status;
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

/* Return time travel inside building */
int SimulatedHuman::getTravelTime(int idx)
{
    return this->timeTravel[idx];
}

/* Return number of times travel in and out of building */
int SimulatedHuman::getNoOfTimesTravel()
{
   return this->noOfTimes;
}

/* Return the status of the resident */
int SimulatedHuman::getStatus()
{
    return this->status;
}

/* Remove floor travelling at key */
void SimulatedHuman::removeFloorTravelling(int key)
{
    floorTravelling.erase(key);
}
