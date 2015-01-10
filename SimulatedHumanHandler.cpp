/*-----------------------------------------------------------------------------
 * File      - SimulatedHumanHandler.cpp
 * Author    - LEVAX
 * Purpose   - Handles SimulatedHuman object
 *---------------------------------------------------------------------------*/

#include "SimulatedHumanHandler.h"

/* Constructor */
SimulatedHumanHandler::SimulatedHumanHandler()
{
    /* Reserve memory */
    simulatedHumanObj.reserve(MAX_PEOPLE);
    nonResSimulatedHumanObj.reserve(MAX_VISITOR);
}

/* Destructor */
SimulatedHumanHandler::~SimulatedHumanHandler()
{
    resetAll();
}

/* Create simulatedHuman */
void SimulatedHumanHandler::createSimulatedHuman(int residentOf)
{
    /* Create new object */
    SimulatedHuman *sh = new SimulatedHuman(residentOf);

    if(residentOf != 0)
    {
        sh->setPersonID(simulatedHumanObj.size()+1);
        simulatedHumanObj.push_back(sh);
    }
    else
    {
        sh->setPersonID(nonResSimulatedHumanObj.size()+1);
        nonResSimulatedHumanObj.push_back(sh);
    }
}

/* Free and reset all simulatedHuman object in simulatedHumanObj vector */
void SimulatedHumanHandler::resetAll()
{
    if(simulatedHumanObj.size() != 0)
    {
        qDebug() << "Freeing memory for resident";
        for(std::size_t i=0;i<simulatedHumanObj.size();i++)
        {
            delete simulatedHumanObj[i];
            qDebug() << "Memory cleared for simulatedHuman resident" << i+1;
        }
        simulatedHumanObj.clear();
    }

    if(nonResSimulatedHumanObj.size() != 0)
    {
        qDebug() << "Freeing memory for visitor";
        for(std::size_t i=0;i<nonResSimulatedHumanObj.size();i++)
        {
            delete nonResSimulatedHumanObj[i];
            qDebug() << "Memory cleared for simulatedHuman visitor" << i+1;
        }
        nonResSimulatedHumanObj.clear();
    }
}

/* Return number of simulatedHuman objects */
int SimulatedHumanHandler::getNumberOfSimulatedHumanObject()
{
    return simulatedHumanObj.size();
}

/* Set person id */
void SimulatedHumanHandler::setPersonID(int idx, int personID, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx]->setPersonID(personID);
    else
        nonResSimulatedHumanObj[idx]->setPersonID(personID);
}

/* Set resident floor */
void SimulatedHumanHandler::setResident(int idx, int residentOf, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx]->setResident(residentOf);
    else
        nonResSimulatedHumanObj[idx]->setResident(0);
}

/* Set weight */
void SimulatedHumanHandler::setWeight(int idx, int weight, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx-1]->setWeight(weight);
    else
        nonResSimulatedHumanObj[idx-1]->setWeight(weight);
}

/* Add floor travelling */
void SimulatedHumanHandler::addFloorTravelling(int idx, int floor, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx-1]->addFloorTravelling(floor);
    else
       nonResSimulatedHumanObj[idx-1]->addFloorTravelling(floor);
}

/* Add travel time */
void SimulatedHumanHandler::addTravelTime(int travelTime, int idx, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx]->addTravelTime(travelTime);
    else
       nonResSimulatedHumanObj[idx]->addTravelTime(travelTime);
}

/* Set number of times traveled */
void SimulatedHumanHandler::setNoOfTimesTravel(int idx, int noOfTimes, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx]->setNoOfTimesTravel(noOfTimes);
    else
       nonResSimulatedHumanObj[idx]->setNoOfTimesTravel(noOfTimes);
}

/* Set the status */
void SimulatedHumanHandler::setStatus(int idx, int status, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx]->setStatus(status);
    else
       nonResSimulatedHumanObj[idx]->setStatus(status);
}

/* Return person id */
int SimulatedHumanHandler::getPersonID(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getPersonID();
    else
        return nonResSimulatedHumanObj[idx]->getPersonID();
}

/* Return resident floor */
int SimulatedHumanHandler::getResident(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getResident();
    else
        return nonResSimulatedHumanObj[idx]->getResident();
}

/* Return weight */
int SimulatedHumanHandler::getWeight(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getWeight();
    else
        return nonResSimulatedHumanObj[idx]->getWeight();
}

/* Return floor travelling at index */
int SimulatedHumanHandler::getFloorTravelling(int idxOfPerson, int idxOfFloorTravelling, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idxOfPerson]->getFloorTravelling(idxOfFloorTravelling);
    else
        return nonResSimulatedHumanObj[idxOfPerson]->getFloorTravelling(idxOfFloorTravelling);
}

/* Return the travel time */
int SimulatedHumanHandler::getTravelTime(int idx, int count, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getTravelTime(count);
    else
        return nonResSimulatedHumanObj[idx]->getTravelTime(count);
}

/* Return the number of travel times */
int SimulatedHumanHandler::getNoOfTimesTravel(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getNoOfTimesTravel();
    else
        return nonResSimulatedHumanObj[idx]->getNoOfTimesTravel();
}

/* Return the status */
int SimulatedHumanHandler::getStatus(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getStatus();
    else
        return nonResSimulatedHumanObj[idx]->getStatus();
}

/* Populate simulatedHuman with weight, etc */
void SimulatedHumanHandler::populateSimulatedHuman(int idx, int totalNoOfFloors, bool isResident)
{
    int randomNoOfTimesTravel = 0;

    /* Add own residence floor if its a residence*/
    if(isResident)
       simulatedHumanObj[idx]->addFloorTravelling(simulatedHumanObj[idx]->getResident());      

    /* Generate 3 timings
     * 1 = morning peak
     * 2 = afternoon peak
     * 3 = evening peak
     */
    int randomPeaks = 0;

    /* Generate 1-100
     * 1-15     = Child,
     * 21-30    = Heavier Adult,
     * 36-50    = Thin Adult,
     * 50-100   = Average Adult
     */
    int randHumanType = rand() % 100 + 1;
    int randomFloor, statusType = 0;

    /* Set weight base on type*/
    if(randHumanType >= 1 && randHumanType <= 15)
    {
        /* Generate weight from 10 to 35 kg */
        int max = 35;
        int min = 15;

        if(isResident)
        {
            simulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);

            randomNoOfTimesTravel = rand() % 2 + 1;
            simulatedHumanObj[idx]->setNoOfTimesTravel(randomNoOfTimesTravel);

            /* Generate timing for residents */
            for(int i = 1; i <= simulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
            {
                if(i == 1)
                    simulatedHumanObj[idx]->addTravelTime(1);

                else
                {
                    randomPeaks = rand() % 1 + 2;

                    if(randomPeaks == 2)
                        simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                    else
                        simulatedHumanObj[idx]->addTravelTime(randomPeaks);
                }
            }

            /* Generate 1-100
             * 1-90     = Travel only to their residence,
             * 90-100    = Travel to other floor to find friends
             */
            int travellingType = rand() % 100 + 1;
            if(travellingType >= 90 && travellingType <= 100)
            {
                /* Generate number of friends the object visiting */
                int noOfFriends = rand() % TRAVEL_MAX + 1;
                for(int i=0;i<noOfFriends;i++)
                {
                    randomFloor = 2 + (rand() % (totalNoOfFloors - 2 + 1));
                    simulatedHumanObj[idx]->addFloorTravelling(randomFloor);
                }
            }
        }
        else
        {
            /* Assign weight only if weight is 0 */
            if(nonResSimulatedHumanObj[idx]->getWeight() == 0)
            {
                nonResSimulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);

                randomNoOfTimesTravel = rand() % 2 + 1;
                nonResSimulatedHumanObj[idx]->setNoOfTimesTravel(randomNoOfTimesTravel);

                /* Generate timing for non residents */
                for(int i = 1; i <= nonResSimulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    randomPeaks = rand() % 3 + 1;
                    if(i == 1)
                    {
                        if(randomPeaks == 1)
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else if(randomPeaks == 2)
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }

                    else
                    {
                        if(nonResSimulatedHumanObj[idx]->getTravelTime(i - 1) == 1)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 2)
                                nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                nonResSimulatedHumanObj[idx]->addTravelTime(3);
                        }

                        else if(nonResSimulatedHumanObj[idx]->getTravelTime(i - 1) == 2)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 3)
                                nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                nonResSimulatedHumanObj[idx]->addTravelTime(1);
                        }

                        else
                           nonResSimulatedHumanObj[idx]->addTravelTime(1);
                    }
                }
            }
        }
    }
    else if(randHumanType >= 21 && randHumanType <= 30)
    {
        /* Generate weight from 80 to 150 kg */
        int max = 150;
        int min = 80;

        if(isResident)
        {
            simulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);

            randomNoOfTimesTravel = rand() % 2 + 1;
            simulatedHumanObj[idx]->setNoOfTimesTravel(randomNoOfTimesTravel);

            /* Generate 1-2
             * 1 = Employed
             * 2 = Unemployed
             */
            statusType = rand() % 2 + 1;
            simulatedHumanObj[idx]->setStatus(statusType);

            if(statusType == 1)
            {
                for(int i = 1; i <= simulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    if(i == 1)
                        simulatedHumanObj[idx]->addTravelTime(1);

                    else
                    {
                        randomPeaks = rand() % 1 + 2;

                        if(randomPeaks == 2)
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }
                }
            }

            else
            {
                for(int i = 1; i <= simulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    randomPeaks = rand() % 3 + 1;
                    if(i == 1)
                    {
                        if(randomPeaks == 1)
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else if(randomPeaks == 2)
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }

                    else
                    {
                        if(simulatedHumanObj[idx]->getTravelTime(i - 1) == 1)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 2)
                                simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                simulatedHumanObj[idx]->addTravelTime(3);
                        }

                        else if(simulatedHumanObj[idx]->getTravelTime(i - 1) == 2)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 3)
                                simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                simulatedHumanObj[idx]->addTravelTime(1);
                        }

                        else
                           simulatedHumanObj[idx]->addTravelTime(1);
                    }
                }
            }

            /* Generate 1-100
             * 1-95     = Travel only to their residence,
             * 95-100    = Travel to other floor to find friends
             */
            int travellingType = rand() % 100 + 1;
            if(travellingType >= 95 && travellingType <= 100)
            {
                /* Generate number of friends the object visiting */
                int noOfFriends = rand() % TRAVEL_MAX + 1;
                for(int i=0;i<noOfFriends;i++)
                {
                    randomFloor = 2 + (rand() % (totalNoOfFloors - 2 + 1));
                    simulatedHumanObj[idx]->addFloorTravelling(randomFloor);
                }
            }
        }
        else
        {
            /* Assign weight only if weight is 0 */
            if(nonResSimulatedHumanObj[idx]->getWeight() == 0)
            {
                nonResSimulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);

                randomNoOfTimesTravel = rand() % 2 + 1;
                nonResSimulatedHumanObj[idx]->setNoOfTimesTravel(randomNoOfTimesTravel);

                /* Generate 1-2
                 * 1 = Employed
                 * 2 = Unemployed
                 */
                statusType = rand() % 2 + 1;
                nonResSimulatedHumanObj[idx]->setStatus(statusType);

                for(int i = 1; i <= nonResSimulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    randomPeaks = rand() % 3 + 1;
                    if(i == 1)
                    {
                        if(randomPeaks == 1)
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else if(randomPeaks == 2)
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }

                    else
                    {
                        if(nonResSimulatedHumanObj[idx]->getTravelTime(i - 1) == 1)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 2)
                                nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                nonResSimulatedHumanObj[idx]->addTravelTime(3);
                        }

                        else if(nonResSimulatedHumanObj[idx]->getTravelTime(i - 1) == 2)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 3)
                                nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                nonResSimulatedHumanObj[idx]->addTravelTime(1);
                        }

                        else
                           nonResSimulatedHumanObj[idx]->addTravelTime(1);
                    }
                }
            }
        }
    }
    else if(randHumanType >= 36 && randHumanType <= 50)
    {
        /* Generate weight from 35 to 55 kg */
        int max = 55;
        int min = 35;

        if(isResident)
        {
            simulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);

            randomNoOfTimesTravel = rand() % 2 + 1;
            simulatedHumanObj[idx]->setNoOfTimesTravel(randomNoOfTimesTravel);

            /* Generate 1-2
             * 1 = Employed
             * 2 = Unemployed
             */
            statusType = rand() % 2 + 1;
            simulatedHumanObj[idx]->setStatus(statusType);

            if(statusType == 1)
            {
                for(int i = 1; i <= simulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    if(i == 1)
                        simulatedHumanObj[idx]->addTravelTime(1);

                    else
                    {
                        randomPeaks = rand() % 1 + 2;

                        if(randomPeaks == 2)
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }
                }
            }

            else
            {
                for(int i = 1; i <= simulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    randomPeaks = rand() % 3 + 1;
                    if(i == 1)
                    {
                        if(randomPeaks == 1)
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else if(randomPeaks == 2)
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }

                    else
                    {
                        if(simulatedHumanObj[idx]->getTravelTime(i - 1) == 1)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 2)
                                simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                simulatedHumanObj[idx]->addTravelTime(3);
                        }

                        else if(simulatedHumanObj[idx]->getTravelTime(i - 1) == 2)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 3)
                                simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                simulatedHumanObj[idx]->addTravelTime(1);
                        }

                        else
                           simulatedHumanObj[idx]->addTravelTime(1);
                    }
                }
            }

            /* Generate 1-100
             * 1-90     = Travel only to their residence,
             * 90-100    = Travel to other floor to find friends
             */
            int travellingType = rand() % 100 + 1;
            if(travellingType >= 90 && travellingType <= 100)
            {
                /* Generate number of friends the object visiting */
                int noOfFriends = rand() % TRAVEL_MAX + 1;
                for(int i=0;i<noOfFriends;i++)
                {
                    randomFloor = 2 + (rand() % (totalNoOfFloors - 2 + 1));
                    simulatedHumanObj[idx]->addFloorTravelling(randomFloor);
                }
            }
        }
        else
        {
            /* Assign weight only if weight is 0 */
            if(nonResSimulatedHumanObj[idx]->getWeight() == 0)
            {
                nonResSimulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);

                randomNoOfTimesTravel = rand() % 2 + 1;
                nonResSimulatedHumanObj[idx]->setNoOfTimesTravel(randomNoOfTimesTravel);

                /* Generate 1-2
                 * 1 = Employed
                 * 2 = Unemployed
                 */
                statusType = rand() % 2 + 1;
                nonResSimulatedHumanObj[idx]->setStatus(statusType);

                for(int i = 1; i <= nonResSimulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    randomPeaks = rand() % 3 + 1;
                    if(i == 1)
                    {
                        if(randomPeaks == 1)
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else if(randomPeaks == 2)
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }

                    else
                    {
                        if(nonResSimulatedHumanObj[idx]->getTravelTime(i - 1) == 1)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 2)
                                nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                nonResSimulatedHumanObj[idx]->addTravelTime(3);
                        }

                        else if(nonResSimulatedHumanObj[idx]->getTravelTime(i - 1) == 2)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 3)
                                nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                nonResSimulatedHumanObj[idx]->addTravelTime(1);
                        }

                        else
                           nonResSimulatedHumanObj[idx]->addTravelTime(1);
                    }
                }
            }
        }
    }
    else
    {
        /* Generate weight from 55 to 79 kg */
        int max = 79;
        int min = 55;

        if(isResident)
        {
            simulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);

            randomNoOfTimesTravel = rand() % 2 + 1;
            simulatedHumanObj[idx]->setNoOfTimesTravel(randomNoOfTimesTravel);

            /* Generate 1-2
             * 1 = Employed
             * 2 = Unemployed
             */
            statusType = rand() % 2 + 1;
            simulatedHumanObj[idx]->setStatus(statusType);

            if(statusType == 1)
            {
                for(int i = 1; i <= simulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    if(i == 1)
                        simulatedHumanObj[idx]->addTravelTime(1);

                    else
                    {
                        randomPeaks = rand() % 1 + 2;

                        if(randomPeaks == 2)
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }
                }
            }

            else
            {
                for(int i = 1; i <= simulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    randomPeaks = rand() % 3 + 1;
                    if(i == 1)
                    {
                        if(randomPeaks == 1)
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else if(randomPeaks == 2)
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            simulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }

                    else
                    {
                        if(simulatedHumanObj[idx]->getTravelTime(i - 1) == 1)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 2)
                                simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                simulatedHumanObj[idx]->addTravelTime(3);
                        }

                        else if(simulatedHumanObj[idx]->getTravelTime(i - 1) == 2)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 3)
                                simulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                simulatedHumanObj[idx]->addTravelTime(1);
                        }

                        else
                           simulatedHumanObj[idx]->addTravelTime(1);
                    }
                }
            }

            /* Generate 1-100
             * 1-85     = Travel only to their residence,
             * 85-100    = Travel to other floor to find friends
             */
            int travellingType = rand() % 100 + 1;
            if(travellingType >= 85 && travellingType <= 100)
            {
                /* Generate number of friends the object visiting */
                int noOfFriends = rand() % TRAVEL_MAX + 1;
                for(int i=0;i<noOfFriends;i++)
                {
                    randomFloor = 2 + (rand() % (totalNoOfFloors - 2 + 1));
                    simulatedHumanObj[idx]->addFloorTravelling(randomFloor);
                }
            }
        }
        else
        {
            /* Assign weight only if weight is 0 */
            if(nonResSimulatedHumanObj[idx]->getWeight() == 0)
            {
                nonResSimulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);

                randomNoOfTimesTravel = rand() % 2 + 1;
                nonResSimulatedHumanObj[idx]->setNoOfTimesTravel(randomNoOfTimesTravel);

                /* Generate 1-2
                 * 1 = Employed
                 * 2 = Unemployed
                 */
                statusType = rand() % 2 + 1;
                nonResSimulatedHumanObj[idx]->setStatus(statusType);

                for(int i = 1; i <= nonResSimulatedHumanObj[idx]->getNoOfTimesTravel(); i++)
                {
                    randomPeaks = rand() % 3 + 1;
                    if(i == 1)
                    {
                        if(randomPeaks == 1)
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else if(randomPeaks == 2)
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                        else
                            nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);
                    }

                    else
                    {
                        if(nonResSimulatedHumanObj[idx]->getTravelTime(i - 1) == 1)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 2)
                                nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                nonResSimulatedHumanObj[idx]->addTravelTime(3);
                        }

                        else if(nonResSimulatedHumanObj[idx]->getTravelTime(i - 1) == 2)
                        {
                            randomPeaks = rand() % 1 + 2;
                            if(randomPeaks == 3)
                                nonResSimulatedHumanObj[idx]->addTravelTime(randomPeaks);

                            else
                                nonResSimulatedHumanObj[idx]->addTravelTime(1);
                        }

                        else
                           nonResSimulatedHumanObj[idx]->addTravelTime(1);
                    }
                }
            }
        }
    }

    if(!isResident)
    {
        // If floor exist, remove and re-add for building floor consistency
        if(getFloorTravellingSize(idx, false) > 0)
            removeFloorTravelling(idx, getFloorTravelling(idx, 0, false), false);

        randomFloor = 2 + (rand() % (totalNoOfFloors - 2 + 1));
        nonResSimulatedHumanObj[idx]->addFloorTravelling(randomFloor);
    }
}

/* Remove last simulatedHuman from vector. Bool flag true=resident false=visitor */
void SimulatedHumanHandler::removeLastData(bool isResident)
{
    if(isResident)
    {
        /* Free memory of last data */
        delete simulatedHumanObj[simulatedHumanObj.size()-1];

        /* Remove from vector */
        simulatedHumanObj.erase(simulatedHumanObj.end() - 1);
    }
    else /* Delete visitor */
    {
        /* Free memory of last data */
        delete nonResSimulatedHumanObj[nonResSimulatedHumanObj.size()-1];

        /* Remove from vector */
        nonResSimulatedHumanObj.erase(nonResSimulatedHumanObj.end() - 1);
    }
}

/* Get vector size of floorTravelling */
int SimulatedHumanHandler::getFloorTravellingSize(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getFloorTravellingSize();
    else
        return nonResSimulatedHumanObj[idx]->getFloorTravellingSize();
}

/* Return number of resident objects */
int SimulatedHumanHandler::getNumberOfVisitorObj()
{
    return nonResSimulatedHumanObj.size();
}

/* Print all simulatedHuman object data for debugging */
void SimulatedHumanHandler::getAllPersonData()
{
    for(std::size_t i=0;i<simulatedHumanObj.size();i++)
    {
        std::size_t numberOfFloorTravelling = simulatedHumanObj[i]->getFloorTravellingSize();

        qDebug() << "PersonID" << simulatedHumanObj[i]->getPersonID();
        qDebug() << "Weight" << simulatedHumanObj[i]->getWeight();
        qDebug() << "Resident of" << simulatedHumanObj[i]->getResident();

        QDebug debug = qDebug();
        qDebug() << "Travelling: ";
        for(std::size_t j=0; j<numberOfFloorTravelling;j++)
            debug << getFloorTravelling(i, j, true);
    }

    for(std::size_t i=0;i<nonResSimulatedHumanObj.size();i++)
    {
        std::size_t numberOfFloorTravelling = nonResSimulatedHumanObj[i]->getFloorTravellingSize();

        qDebug() << "VisitorID" << nonResSimulatedHumanObj[i]->getPersonID();
        qDebug() << "Weight" << nonResSimulatedHumanObj[i]->getWeight();
        qDebug() << "Not a resident";

        QDebug debug = qDebug();
        qDebug() << "Travelling: ";
        for(std::size_t j=0; j<numberOfFloorTravelling;j++)
            debug << getFloorTravelling(i, j, false);
    }
}

/* Remove floor travelling of person idx with key */
void SimulatedHumanHandler::removeFloorTravelling(int idx, int key, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx]->removeFloorTravelling(key);
    else
        nonResSimulatedHumanObj[idx]->removeFloorTravelling(key);
}

/* Update floor evenly based on average number of people per floor */
void SimulatedHumanHandler::updateFloorsEvenly(int avgPerFloor, int totalFloor, int totalPeople)
{

    int maxNum = totalFloor;
    int minNum = 2;
    int randomFloor = 2;

    /* remove all current travelling data for reallocation*/
    for(int i=0;i<getNumberOfSimulatedHumanObject();i++)
        removeFloorTravelling(i, getResident(i,true), true);

    /* If floor and number of people is the same */
    if(avgPerFloor == totalPeople)
    {
        /* Set all people to the total floor */
        for(int i=0;i<totalPeople;i++)
        {
            addFloorTravelling(i+1, totalFloor, true);
            setResident(i,totalFloor,true);
        }
    }
    else
    {
        int currentFloor = 2;

        /* Allocate evenly for all people */
        for(int i=0;i<totalPeople;i++)
        {
            if(currentFloor <= totalFloor)
            {
                int remainder = totalPeople % (totalFloor-1);

                /* If there are remainder, the floor allocated will be random */
                if(i < totalPeople-remainder)
                {
                    addFloorTravelling(i+1, currentFloor, true);
                    setResident(i,currentFloor,true);
                }
                else
                {
                    randomFloor = (rand() % (maxNum-minNum+1)+minNum);
                    addFloorTravelling(i+1, randomFloor, true);
                    setResident(i,randomFloor,true);
                }
            }
            /* Reset current floor back to 2 if it has reached totalFloor */
            if(currentFloor == totalFloor)
                currentFloor = 2;
            else
                currentFloor++;
        }
    }
}

