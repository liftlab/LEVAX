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

/* Add travel time */
void SimulatedHumanHandler::addTravelTime(int travelTime, int floor, int idx, bool isResident)
{
    if(isResident)
        simulatedHumanObj[idx]->addTravelTime(travelTime, floor);
    else
        nonResSimulatedHumanObj[idx]->addTravelTime(travelTime, floor);
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

/* Return the travel time */
int SimulatedHumanHandler::getTravelTime(int idx, int count, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getTravelTime(count);
    else
        return nonResSimulatedHumanObj[idx]->getTravelTime(count);
}

/* Return the travel floor with respect to the time */
int SimulatedHumanHandler::getTravelFloor(int idx, int count, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getTravelFloor(count);
    else
        return nonResSimulatedHumanObj[idx]->getTravelFloor(count);
}

/* Return the number of travel times */
int SimulatedHumanHandler::getNoOfTimesTravel(int idx, bool isResident)
{
    if(isResident)
        return simulatedHumanObj[idx]->getNoOfTimesTravel();
    else
        return nonResSimulatedHumanObj[idx]->getNoOfTimesTravel();
}

/* Populate simulatedHuman with weight, etc */
void SimulatedHumanHandler::populateSimulatedHuman(int idx, int totalNoOfFloors, bool isResident)
{
    /* Generate Weight */
    int min = 0;
    int max = 0;

    /* Generate 1-100
     * 1-15     = Child,
     * 16-30    = Heavier Adult,
     * 31-50    = Thin Adult,
     * 50-100   = Average Adult
     */
    int randHumanType = rand() % 100 + 1;

    if(randHumanType >= 1 && randHumanType <= 15)
    {
        min = 15;
        max = 35;
        if(isResident)
            simulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);
        else
        {
            if(nonResSimulatedHumanObj[idx]->getWeight() == 0)
                nonResSimulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);
        }
    }
    else if(randHumanType >= 16 && randHumanType <= 30)
    {
        min = 80;
        max = 150;
        if(isResident)
            simulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);
        else
        {
            if(nonResSimulatedHumanObj[idx]->getWeight() == 0)
                nonResSimulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);
        }
    }
    else if(randHumanType >= 31 && randHumanType <= 50)
    {
        min = 35;
        max = 55;
        if(isResident)
            simulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);
        else
        {
            if(nonResSimulatedHumanObj[idx]->getWeight() == 0)
                nonResSimulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);
        }
    }
    else
    {
        min = 55;
        max = 79;
        if(isResident)
            simulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);
        else
        {
            if(nonResSimulatedHumanObj[idx]->getWeight() == 0)
                nonResSimulatedHumanObj[idx]->setWeight(rand() % (max - min) + min);
        }
    }

    /* Generate time travel */
    int randomFloor = 2 + (rand() % (totalNoOfFloors - 2 + 1));

    int randomNoOfTimesTravel = 0;

    int randomTime = 0;
    int currentTime = 0;

    if(isResident)
    {
        /* Generate 1-100
         * 1-30     = Schooling
         * 31-80    = Working
         * 81-100   = Others
         */
        int status = rand() % 100 + 1;

        // Is Schooling
        if(status >= 1 && status <= 30)
        {
            randomNoOfTimesTravel = rand() % 2 + 1;

            /* Generate timing */
            for(int i = 1; i <= randomNoOfTimesTravel; i++)
            {
                /* Travel to school and back */
                if(i == 1)
                {
                    /* Student leaves for school at 6AM (21600) to 7:15AM  (26100) */
                    int timeToSchool = rand() % (26100 - 21600) + 21600;
                    simulatedHumanObj[idx]->addTravelTime(timeToSchool, 1);

                    /* Student back from school at 1PM (46800) to 7PM  (68400) */
                    int backFromSchool = rand() % (68400 - 46800) + 46800;
                    simulatedHumanObj[idx]->addTravelTime(backFromSchool, simulatedHumanObj[idx]->getResident());

                    currentTime = backFromSchool + 600;
                }
                else
                {
                    /* Generate a time the student will leave home
                    * Time ranging from back from school to 8PM (72000)
                    */
                    randomTime = rand() % (72000 - currentTime) + currentTime;
                    simulatedHumanObj[idx]->addTravelTime(randomTime, 1);
                    currentTime = randomTime + 600;

                    randomTime = rand() % (79200 - currentTime) + currentTime;
                    simulatedHumanObj[idx]->addTravelTime(randomTime, simulatedHumanObj[idx]->getResident());
                    currentTime = randomTime + 600;
                }
            }
        }
        else if(status >= 31 && status <= 80)
        {
            randomNoOfTimesTravel = rand() % 2 + 1;

            for(int i = 1; i <= randomNoOfTimesTravel; i++)
            {
                if(i == 1)
                {
                    /* Person leaves for work at 5:30AM (19800) to 8:30AM  (30600) */
                    int timeToWork = rand() % (30600 - 19800) + 19800;
                    simulatedHumanObj[idx]->addTravelTime(timeToWork, 1);

                    int backFromWork;
                    if(randomNoOfTimesTravel == 1)
                    {
                        /* Person back from work at 6PM (68400) to 11:59PM  (86399) */
                        backFromWork = rand() % (86399 - 82800) + 82800;
                    }
                    else
                    {
                        /* Person back from work at 6PM (68400) to 8PM  (72000) */
                        backFromWork = rand() % (72000 - 68400) + 68400;
                    }

                    simulatedHumanObj[idx]->addTravelTime(backFromWork, simulatedHumanObj[idx]->getResident());
                    currentTime = backFromWork + 600;
                }
                else
                {
                    /* Generate a time the person will leave home
                     * Time ranging from backFromWork to 23:59PM (86399)
                     */
                    randomTime = rand() % (86399 - currentTime) + currentTime;
                    simulatedHumanObj[idx]->addTravelTime(randomTime, 1);
                    currentTime = randomTime + 600;
                }
            }
        }
        else if(status >= 81 && status <= 100)
        {
            randomNoOfTimesTravel = rand() % 3 + 1;

            int range1 = 0; // 12AM
            int range2 = 86399; // 23:59PM

            int i = 0;
            while(i < randomNoOfTimesTravel)
            {
                if(range2 <= 86399 && range1 <= 86399)
                {
                    /* Person leaves home at range1 to range2 */
                    int timeLeaveHome = rand() % (range2 - range1) + range1;
                    simulatedHumanObj[idx]->addTravelTime(timeLeaveHome, 1);
                    currentTime = timeLeaveHome+600;

                    /* Person reach home at currentTime to currentTime+7hour*/
                    int backToHome = rand() % ((currentTime+25200) - currentTime) + currentTime;

                    /* If the time back home is more than 23:59, we are not interested in it as simulated has ended */
                    if(backToHome <= 86399)
                    {
                        simulatedHumanObj[idx]->addTravelTime(backToHome, simulatedHumanObj[idx]->getResident());
                        currentTime = backToHome+600;

                        range1 = currentTime+14400; // at most 4 hours interval
                        range2 = range1+25200; // range of 7 hours
                    }
                    else
                    {
                        // exit loop
                        i = randomNoOfTimesTravel;
                    }
                }
                else
                {
                    // exit loop;
                    i = randomNoOfTimesTravel;
                }

                /* if more than 23h 59min 59s, stop loop */
                if(currentTime > 86399)
                    i = randomNoOfTimesTravel;
                else
                    i++;
            }
        }
    }
    else
    {
        if(nonResSimulatedHumanObj[idx]->getNoOfTimesTravel() > 0)
        {
            for(int i=0;i<nonResSimulatedHumanObj[idx]->getNoOfTimesTravel();i++)
            {
                if(nonResSimulatedHumanObj[idx]->getTravelFloor(i) != 1)
                    nonResSimulatedHumanObj[idx]->setTravelLevel(i,randomFloor);
            }
        }
        else
        {
            /* Generate 1-100
             * 1-10     = Morning
             * 11-30    = Afternoon
             * 31-60    = Late Afternoon
             * 60-100   = Evening
             */
            int randomVisitingPeak = rand() % 100 + 1;

            if(randomVisitingPeak >= 1 && randomVisitingPeak <= 10)
            {
                /* Visit at 7AM (25200) to 9AM  (25200) */
                randomTime = rand() % (32400 - 25200) + 25200;
                nonResSimulatedHumanObj[idx]->addTravelTime(randomTime, randomFloor);
                currentTime = randomTime + 600;

                /* Generate a time the visitor will leave
                 * Time ranging from currentTime to 12PM (43200)
                 */
                randomTime = rand() % (43200 - currentTime) + currentTime;
                nonResSimulatedHumanObj[idx]->addTravelTime(randomTime, 1);
            }
            else if(randomVisitingPeak >= 11 && randomVisitingPeak <= 30)
            {
                /* Visit at 11AM (39600) to 2PM  (50400) */
                randomTime = rand() % (50400 - 39600) + 39600;
                nonResSimulatedHumanObj[idx]->addTravelTime(randomTime, randomFloor);
                currentTime = randomTime + 600;

                /* Generate a time the visitor will leave
                 * Time ranging from currentTime to 4PM (57600)
                 */
                randomTime = rand() % (57600 - currentTime) + currentTime;
                nonResSimulatedHumanObj[idx]->addTravelTime(randomTime, 1);
            }
            else if(randomVisitingPeak >= 31 && randomVisitingPeak <= 60)
            {
                /* Visit at 5PM (61200) to 7:30PM  (70200) */
                randomTime = rand() % (70200 - 61200) + 61200;
                nonResSimulatedHumanObj[idx]->addTravelTime(randomTime, randomFloor);
                currentTime = randomTime + 600;

                /* Generate a time the visitor will leave
                 * Time ranging from currentTime to 8PM (72000)
                 */
                randomTime = rand() % (72000 - currentTime) + currentTime;
                nonResSimulatedHumanObj[idx]->addTravelTime(randomTime, 1);
            }
            else if(randomVisitingPeak >= 60 && randomVisitingPeak <= 100)
            {
                /* Visit at 7:30PM (61200) to 9PM  (75600) */
                randomTime = rand() % (75600 - 61200) + 61200;
                nonResSimulatedHumanObj[idx]->addTravelTime(randomTime, randomFloor);
                currentTime = randomTime + 600;

                /* Generate a time the visitor will leave
                 * Time ranging from currentTime to 11PM (82800)
                 */
                randomTime = rand() % (82800 - currentTime) + currentTime;
                nonResSimulatedHumanObj[idx]->addTravelTime(randomTime, 1);
            }
        }
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

/* Return number of resident objects */
int SimulatedHumanHandler::getNumberOfVisitorObj()
{
    return nonResSimulatedHumanObj.size();
}

/* Update floor evenly based on average number of people per floor */
void SimulatedHumanHandler::updateFloorsEvenly(int avgPerFloor, int totalFloor, int totalPeople)
{
    int maxNum = totalFloor;
    int minNum = 2;
    int randomFloor = 2;

    /* If floor and number of people is the same */
    if(avgPerFloor == totalPeople)
    {
        /* Set all people to the total floor */
        for(int i=0;i<totalPeople;i++)
        {
            setResident(i,totalFloor,true);

            for(int j=0;j<simulatedHumanObj[i]->getNoOfTimesTravel();j++)
            {
                if(simulatedHumanObj[i]->getTravelFloor(j) != 1)
                    simulatedHumanObj[i]->setTravelLevel(j,totalFloor);
            }
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
                    setResident(i,currentFloor,true);

                    for(int j=0;j<simulatedHumanObj[i]->getNoOfTimesTravel();j++)
                    {
                        if(simulatedHumanObj[i]->getTravelFloor(j) != 1)
                            simulatedHumanObj[i]->setTravelLevel(j,currentFloor);
                    }
                }
                else
                {
                    randomFloor = (rand() % (maxNum-minNum+1)+minNum);

                    setResident(i,randomFloor,true);

                    for(int j=0;j<simulatedHumanObj[i]->getNoOfTimesTravel();j++)
                    {
                        if(simulatedHumanObj[i]->getTravelFloor(j) != 1)
                            simulatedHumanObj[i]->setTravelLevel(j,randomFloor);
                    }
                }
            }
            /* Reset current floor back to 2 if it has reached totalFloor */
            if(currentFloor == totalFloor)
                currentFloor = 2;
            else
                currentFloor++;
        }
    }

    for(size_t i=0;i<nonResSimulatedHumanObj.size();i++)
    {
        int randomFloor = 2 + (rand() % (totalFloor - 2 + 1));

        for(int j=0;j<nonResSimulatedHumanObj[i]->getNoOfTimesTravel();j++)
        {
            if(nonResSimulatedHumanObj[i]->getTravelFloor(j) != 1)
                nonResSimulatedHumanObj[i]->setTravelLevel(j,randomFloor);
        }
    }
}
