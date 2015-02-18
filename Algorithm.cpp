/*-----------------------------------------------------------------------------
 * File      - Algorithm.cpp
 * Author    - LEVAX
 * Purpose   - Algorithm object class. Four Different Algorithms
 *---------------------------------------------------------------------------*/

#include "Algorithm.h"
#include <QElapsedTimer>

/* Empty Constructor */
Algorithm::Algorithm()
{
    noOfLifts = 0;
    noOfFloors = 0;
    noOfResidents = 0;
    noOfVisitors = 0;
}

/* Destructor */
Algorithm::~Algorithm()
{

}

/* Nearest Car algorithm */
pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > Algorithm::nearestCar(BuildingHandler *bh,
                                                                    LiftHandler *lh,
                                                                    SimulatedHumanHandler *shh)
{
    /* Start timer */
    QElapsedTimer timer;
    timer.start();

    /* Simulation Variables */
    noOfLifts = lh->getNumberOfLiftsObject();
    noOfFloors = bh->getNoOfFloor();
    noOfResidents = shh->getNumberOfSimulatedHumanObject();
    noOfVisitors = shh->getNumberOfVisitorObj();

    /* Stores passenger information in current lift */
    vector<vector<PassengerInfo> > passengersInLift;
    passengersInLift.resize(noOfLifts);

    /* Store entire list of passenger information sorted ascending according to time travel */
    vector<PassengerInfo> passengerList = processPassenger(shh);

    /* Store list of passenger that pressed the button */
    vector<WaitingStatus> waitingList;

    /* Stores the path the lifts have to travel */
    vector<map<int, int> > travelPath;
    travelPath.resize(noOfLifts);

    /* Stores lift idle time */
    vector<int> liftIdleTime;
    liftIdleTime.resize(noOfLifts);
    fill(liftIdleTime.begin(), liftIdleTime.end(), 0);


    /* A day consist of 86400 seconds
     * 0 = 00:00:00
     * 86399 = 23:59:59
     */
    int seconds = 0;
    while(seconds < 86399)
    {
        /* Search for passenger that matches the time and push them into waitingList */
        vector<PassengerInfo>::iterator itr = find(passengerList.begin(),passengerList.end(), seconds);
        if(itr != passengerList.end())
        {
            int totalFound = count(passengerList.begin(), passengerList.end(), seconds);
            for(int i=0;i<totalFound;i++)
            {
                WaitingStatus ws;
                ws.pi = &(*itr);
                for(int i=0;i<noOfLifts;i++)
                    ws.FS.push_back(0);
                ws.idealLift = 0;
                waitingList.push_back(ws);

                itr++;
            }
        }

        /* Call lift if waitingList is not empty */
        if(!waitingList.empty())
        {
            /* Compute FS for each lift and choose an ideal lift */
            computeFS(waitingList, lh);

            /* Traverse each individual person and call the lift */
            size_t queueCounter = 0;
            while(queueCounter < waitingList.size())
            {
                /* Call lift if only person is not in lift */
                if(!waitingList[queueCounter].pi->isInLift)
                {
                    int liftNo = waitingList[queueCounter].idealLift;

                    /* Check for ideal lift match */
                    if(waitingList[queueCounter].idealLift == liftNo)
                    {
                        int distance = abs(lh->getLiftCurrentFloor(liftNo) -
                                           waitingList[queueCounter].pi->currentFloor) * bh->getMetrePerFloor();

                        /* Lift is already moving */
                        if(lh->getLiftIsMoving(liftNo))
                        {
                            /* Intecept lift */
                            if(lh->getLiftTravellingTo(liftNo) != waitingList[queueCounter].pi->currentFloor)
                            {
                                /* If lift current weight is into the 30kg buffer, lift will ignore the intercept */
                                if(lh->getLiftCurrentWeight(liftNo) <= (lh->getLiftWeight(liftNo)-30))
                                {
                                    /* Intercept lift if call is within lift travellingTo and lift currentFloor */
                                    distance -= lh->getLiftTotalDistance(liftNo);

                                    if(distance <= lh->getLiftDistanceLeft(liftNo) && distance >= 0)
                                    {
                                        /* UP */
                                        if(lh->getLiftDirection(liftNo) == +1
                                                && waitingList[queueCounter].pi->directionHeading == lh->getLiftDirection(liftNo)
                                                && waitingList[queueCounter].pi->currentFloor > lh->getLiftCurrentFloor(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        } /* DOWN */
                                        else if(lh->getLiftDirection(liftNo) == -1
                                                && waitingList[queueCounter].pi->directionHeading == lh->getLiftDirection(liftNo)
                                                && waitingList[queueCounter].pi->currentFloor < lh->getLiftCurrentFloor(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        } /* UP and PARKING */
                                        else if(lh->getLiftDirection(liftNo) == +1
                                                && waitingList[queueCounter].pi->directionHeading != lh->getLiftDirection(liftNo)
                                                && lh->getLiftPark(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);
                                            lh->setLiftPark(liftNo, false);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        } /* DOWN and PARKING */
                                        else if(lh->getLiftDirection(liftNo) == -1
                                                && waitingList[queueCounter].pi->directionHeading != lh->getLiftDirection(liftNo)
                                                && lh->getLiftPark(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);
                                            lh->setLiftPark(liftNo, false);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        }
                                    }
                                }
                            }
                        }
                        else /* Lift is not moving */
                        {
                            /* Lift is at the same floor as caller */
                            if(lh->getLiftCurrentFloor(liftNo) == waitingList[queueCounter].pi->currentFloor)
                            {
                                /* Remove current floor from travelPath */
                                std::map<int,int>::iterator searchPathIter;
                                searchPathIter = travelPath[liftNo].find(waitingList[queueCounter].pi->currentFloor);
                                if(searchPathIter != travelPath[liftNo].end())
                                    travelPath[liftNo].erase(searchPathIter);

                                /* Calculate weight */
                                int weight = shh->getWeight(waitingList[queueCounter].pi->personNo, waitingList[queueCounter].pi->isResident);
                                int liftCurrentWeight = lh->getLiftCurrentWeight(liftNo);

                                /* If weight is in acceptable limit */
                                if((weight+liftCurrentWeight) <= lh->getLiftWeight(liftNo))
                                {
                                    /* Push to travelPath */
                                    travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->travellingTo, waitingList[queueCounter].pi->travellingTo));

                                    /* Get first value */
                                    int temp = travelPath[liftNo].begin()->first;

                                    /* temp < current floor = DOWN */
                                    if(temp < lh->getLiftCurrentFloor(liftNo))
                                    {
                                        lh->setLiftDirection(liftNo, -1);
                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - travelPath[liftNo].rbegin()->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, travelPath[liftNo].rbegin()->first);
                                    }
                                    else /* UP */
                                    {
                                        lh->setLiftDirection(liftNo, +1);
                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - travelPath[liftNo].begin()->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, travelPath[liftNo].begin()->first);
                                    }

                                    /* Push passenger in lift */
                                    waitingList[queueCounter].pi->isInLift = true;
                                    passengersInLift[liftNo].push_back(*waitingList[queueCounter].pi);
                                    lh->setLiftCurrentWeight(liftNo, (liftCurrentWeight+weight));

                                    /* Store passenger boarding time */
                                    waitingList[queueCounter].pi->liftBoardTime = seconds;

                                    /* Set lift move status */
                                    lh->setLiftMoveNextRound(liftNo, true);
                                    lh->setLiftIsMoving(liftNo,true);
                                }
                            }
                            else
                            {
                                /* If lift is not picking any passenger and a call is made, move lift to call */
                                if(!lh->getLiftPickPassenger(liftNo))
                                {
                                    /* Push to travelPath */
                                    travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));

                                    /* Determine next floor to travel */
                                    if(lh->getLiftDirection(liftNo) == +1)
                                    {
                                        map<int,int>::iterator it = travelPath[liftNo].upper_bound(lh->getLiftCurrentFloor(liftNo));

                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - it->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, it->first);
                                    }
                                    else if(lh->getLiftDirection(liftNo) == -1)
                                    {
                                        map<int,int>::iterator it = travelPath[liftNo].lower_bound(lh->getLiftCurrentFloor(liftNo));
                                        it--;

                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - it->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, it->first);
                                    }

                                    /* Determine direction based on travelling floor */
                                    if(lh->getLiftTravellingTo(liftNo) < lh->getLiftCurrentFloor(liftNo))
                                        lh->setLiftDirection(liftNo, -1);
                                    else
                                        lh->setLiftDirection(liftNo, +1);

                                    /* Allows lift to pick passenger */
                                    lh->setLiftPickPassenger(liftNo, true);

                                    /* Lift to start moving next seconds */
                                    lh->setLiftMoveNextRound(liftNo, true);
                                }
                            }
                        }
                    }
                }
                queueCounter++;
            }
        }

        /* Movement of all lifts */
        for(int i=0;i<noOfLifts;i++)
        {
            /* Count number of passengers waiting for current targetted lift */
            int found = count(waitingList.begin(), waitingList.end(), i);

            /* Park lift if lift has no passenger, and lift is empty and is not picking up any passenger */
            if(found <= 0 && passengersInLift[i].size() <= 0
                    && !lh->getLiftPickPassenger(i) && !lh->getLiftPark(i))
            {
                if(lh->getLiftCurrentFloor(i) != lh->getLiftDefaultFloor(i))
                {
                    /* Set direction to travel back to default floor */
                    if(lh->getLiftDefaultFloor(i) >= lh->getLiftCurrentFloor(i))
                        lh->setLiftDirection(i, +1);
                    else
                        lh->setLiftDirection(i, -1);

                    /* Calculate distance to default floor */
                    int dist = abs(lh->getLiftCurrentFloor(i) - lh->getLiftDefaultFloor(i)) * bh->getMetrePerFloor();
                    lh->setLiftDistanceLeft(i, dist);
                    lh->setLiftTravellingTo(i, lh->getLiftDefaultFloor(i));

                    lh->setLiftDistanceCount(i, 0);
                    lh->setLiftTotalDistance(i, 0);

                    /* Push to travelPath */
                    travelPath[i].insert(pair<int,int>(lh->getLiftDefaultFloor(i), lh->getLiftDefaultFloor(i)));

                    /* Set lift status */
                    lh->setLiftIsMoving(i, true);
                    lh->setLiftPark(i, true);
                }
            }
            /* Lift movement algorithm
             * Move lift only if there are passengers,
             * lift is parking or picking up passenger
             */
            if(passengersInLift[i].size() > 0 || lh->getLiftPark(i) || lh->getLiftPickPassenger(i))
            {
                /* Set lift moving status */
                lh->setLiftIsMoving(i,true);

                // If lift is moving and moveNextRound is false, calculate movements based on speed
                if(lh->getLiftIsMoving(i) && !lh->getLiftMoveNextRound(i))
                {
                    /* Lift movement variables */
                    int distance = lh->getLiftDistanceLeft(i);
                    int speed = lh->getLiftSpeed(i);
                    int metrePerFloor = bh->getMetrePerFloor();

                    /* Continue moving until speed or distance has been met */
                    while(lh->getLiftDistanceCount(i) <= speed && lh->getLiftDistanceLeft(i) >= 0)
                    {
                        /* increase distance by 1 metre per loop */
                        lh->increaseDistance(i, 1);
                        lh->setLiftDistanceLeft(i, lh->getLiftDistanceLeft(i)-1);

                        /* Check if lift has traveled one floor */
                        if(lh->getLiftTotalDistance(i)%metrePerFloor == 0)
                        {
                            /* Update current floor */
                            if(lh->getLiftDirection(i) == +1)
                            {
                                if(lh->getLiftCurrentFloor(i) < bh->getNoOfFloor())
                                    lh->setLiftCurrentFloor(i, lh->getLiftCurrentFloor(i)+1);
                                else
                                {
                                    lh->setLiftCurrentFloor(i, bh->getNoOfFloor());
                                    lh->setLiftDirection(i, -1);
                                }
                            }
                            else
                            {
                                if(lh->getLiftCurrentFloor(i) > 1)
                                    lh->setLiftCurrentFloor(i, lh->getLiftCurrentFloor(i)-1);
                                else
                                {
                                    lh->setLiftCurrentFloor(i, 1);
                                    lh->setLiftDirection(i, +1);
                                }
                            }

                            /* If lift is at the highest floor, lift direction is DOWN */
                            if(lh->getLiftCurrentFloor(i) == bh->getNoOfFloor())
                                lh->setLiftDirection(i, -1);

                        }

                        /* If lift distance left is 0 */
                        if(lh->getLiftDistanceLeft(i) <= 0)
                        {
                            /* Remove current floor from travelPath */
                            std::map<int,int>::iterator searchPathIter;
                            searchPathIter = travelPath[i].find(lh->getLiftCurrentFloor(i));
                            if(searchPathIter != travelPath[i].end())
                                travelPath[i].erase(searchPathIter);

                            /* Reset movement variable */
                            lh->setLiftDistanceCount(i, 0);
                            lh->setLiftTotalDistance(i, 0);
                            lh->setLiftDistanceLeft(i, 0);
                            lh->setLiftIsMoving(i, false);

                            /* If travelPath is not empty, find next floor to travel */
                            if(!travelPath[i].empty())
                            {
                                /* Get first value */
                                int temp = travelPath[i].begin()->first;

                                /* temp < current floor = DOWN */
                                if(temp < lh->getLiftCurrentFloor(i))
                                {
                                    lh->setLiftDirection(i, -1);
                                    distance = abs(lh->getLiftCurrentFloor(i) - travelPath[i].rbegin()->first) * bh->getMetrePerFloor();
                                    lh->setLiftDistanceLeft(i, distance);
                                    lh->setLiftTravellingTo(i, travelPath[i].rbegin()->first);

                                }
                                else /* UP */
                                {
                                    lh->setLiftDirection(i, +1);
                                    distance = abs(lh->getLiftCurrentFloor(i) - travelPath[i].begin()->first) * bh->getMetrePerFloor();
                                    lh->setLiftDistanceLeft(i, distance);
                                    lh->setLiftTravellingTo(i, travelPath[i].begin()->first);
                                }
                            }

                            /* Reset lift status if true*/
                            if(lh->getLiftPark(i))
                                lh->setLiftPark(i, false);

                            if(lh->getLiftPickPassenger(i))
                                lh->setLiftPickPassenger(i, false);

                            /* clear passengers that reached destination */
                            vector<PassengerInfo>::iterator iter = passengersInLift[i].begin();
                            for( ; iter != passengersInLift[i].end(); )
                            {
                                iter->isInLift = false;

                                /* find passengers from waitingList that matches the info
                                 * (isResident, personNo, travelNo, travellingTo)
                                 */
                                pair<PassengerInfo, int> searchPair;
                                searchPair = make_pair(*iter, lh->getLiftCurrentFloor(i));

                                /* Find and remove passenger from waitingList that matches the searchPair */
                                vector<WaitingStatus>::iterator itr = find(waitingList.begin(),waitingList.end(), searchPair);
                                if(itr != waitingList.end())
                                {
                                    int personIndex = itr->pi->personNo;
                                    bool isResident = itr->pi->isResident;

                                    /* Time person exit lift */
                                    itr->pi->liftExitTime = seconds+1;

                                    int weight = shh->getWeight(personIndex, isResident);
                                    int liftCurrentWeight = lh->getLiftCurrentWeight(i);
                                    lh->setLiftCurrentWeight(i, (liftCurrentWeight-weight));

                                    /* Erase */
                                    waitingList.erase(itr);
                                }

                                /* Remove passengers from passengersInLift that matches the info travellingTo */
                                if(iter->travellingTo == lh->getLiftCurrentFloor(i))
                                {
                                    /* Erase */
                                    passengersInLift[i].erase(iter);
                                }
                                else
                                {
                                    ++iter;
                                }
                            }
                            break;
                        }

                        /* If speed has been met, reset and break out of loop */
                        if(lh->getLiftDistanceCount(i) >= speed)
                        {
                            lh->setLiftDistanceCount(i,0);
                            break;
                        }
                    }
                }
                else
                {
                    lh->setLiftMoveNextRound(i, false);
                }
            }
            else
            {
                if(!lh->getLiftIsMoving(i))
                {
                    int time = liftIdleTime[i];
                    liftIdleTime[i] = (time+1);
                }
                lh->setLiftIsMoving(i,false);
            }
        }
        seconds++;
    }

    /* Get summary of simulation */
    QString summary = getSummary(passengerList);

    /* Get time related information <elapsedTime, <averageWaitingTime, averageTravelTime>> */
    pair<pair<double, vector<int> >, pair<int, int> > averageTime;
    averageTime = getTiming(convertToSeconds(timer.elapsed()), passengerList, liftIdleTime);

    /* Return information <Summary, <elapsedTime, <averageWaitingTime, averageTravelTime>> */
    pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > result = make_pair(summary, averageTime);

    /* Reset variables */
    lh->resetLift(noOfFloors);
    noOfLifts = 0;
    noOfFloors = 0;
    noOfResidents = 0;
    noOfVisitors = 0;

    /* Clear data and reset data */
    waitingList.clear();
    passengerList.clear();
    passengersInLift.clear();
    liftIdleTime.clear();

    /* Return result */
    return result;
}

/* Fixed Sectoring Common Sector System algorithm */
pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > Algorithm::fixedSectoringCommonSectorSystem(BuildingHandler *bh, LiftHandler *lh, SimulatedHumanHandler *shh)
{
    /* Start timer */
    QElapsedTimer timer;
    timer.start();

    /* Simulation Variables */
    noOfLifts = lh->getNumberOfLiftsObject();
    noOfFloors = bh->getNoOfFloor();
    noOfResidents = shh->getNumberOfSimulatedHumanObject();
    noOfVisitors = shh->getNumberOfVisitorObj();

    /* Stores passenger information in current lift */
    vector<vector<PassengerInfo> > passengersInLift;
    passengersInLift.resize(noOfLifts);

    /* Store entire list of passenger information sorted ascending according to time travel */
    vector<PassengerInfo> passengerList = processPassenger(shh);

    /* Store list of passenger that pressed the button */
    vector<WaitingStatus> waitingList;

    /* Stores the path the lifts have to travel */
    vector<map<int, int> > travelPath;
    travelPath.resize(noOfLifts);

    /* Stores lift idle time */
    vector<int> liftIdleTime;
    liftIdleTime.resize(noOfLifts);
    fill(liftIdleTime.begin(), liftIdleTime.end(), 0);

    /* Divide lift into multiple sector */
    splitLiftBySector();

    /* A day consist of 86400 seconds
     * 0 = 00:00:00
     * 86399 = 23:59:59
     */
    int seconds = 0;
    while(seconds < 86399)
    {
        /* Search for passenger that matches the time and push them into waitingList */
        vector<PassengerInfo>::iterator itr = find(passengerList.begin(),passengerList.end(), seconds);
        if(itr != passengerList.end())
        {
            int totalFound = count(passengerList.begin(), passengerList.end(), seconds);
            for(int i=0;i<totalFound;i++)
            {
                WaitingStatus ws;
                ws.pi = &(*itr);
                for(int i=0;i<noOfLifts;i++)
                    ws.FS.push_back(0);
                ws.idealLift = 0;
                waitingList.push_back(ws);

                itr++;
            }
        }

        /* Call lift if waitingList is not empty */
        if(!waitingList.empty())
        {
            /* Choose an ideal lift by sector */
            getLiftBySector(waitingList);

            /* Traverse each individual person and call the lift */
            size_t queueCounter = 0;
            while(queueCounter < waitingList.size())
            {
                /* Call lift if only person is not in lift */
                if(!waitingList[queueCounter].pi->isInLift)
                {
                    int liftNo = waitingList[queueCounter].idealLift;

                    /* Check for ideal lift match */
                    if(waitingList[queueCounter].idealLift == liftNo)
                    {
                        int distance = abs(lh->getLiftCurrentFloor(liftNo) -
                                           waitingList[queueCounter].pi->currentFloor) * bh->getMetrePerFloor();

                        /* Lift is already moving */
                        if(lh->getLiftIsMoving(liftNo))
                        {
                            /* Intecept lift */
                            if(lh->getLiftTravellingTo(liftNo) != waitingList[queueCounter].pi->currentFloor)
                            {
                                /* If lift current weight is into the 30kg buffer, lift will ignore the intercept */
                                if(lh->getLiftCurrentWeight(liftNo) <= (lh->getLiftWeight(liftNo)-30))
                                {
                                    /* Intercept lift if call is within lift travellingTo and lift currentFloor */
                                    distance -= lh->getLiftTotalDistance(liftNo);

                                    if(distance <= lh->getLiftDistanceLeft(liftNo) && distance >= 0)
                                    {
                                        /* UP */
                                        if(lh->getLiftDirection(liftNo) == +1
                                                && waitingList[queueCounter].pi->directionHeading == lh->getLiftDirection(liftNo)
                                                && waitingList[queueCounter].pi->currentFloor > lh->getLiftCurrentFloor(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        } /* DOWN */
                                        else if(lh->getLiftDirection(liftNo) == -1
                                                && waitingList[queueCounter].pi->directionHeading == lh->getLiftDirection(liftNo)
                                                && waitingList[queueCounter].pi->currentFloor < lh->getLiftCurrentFloor(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        } /* UP and PARKING */
                                        else if(lh->getLiftDirection(liftNo) == +1
                                                && waitingList[queueCounter].pi->directionHeading != lh->getLiftDirection(liftNo)
                                                && lh->getLiftPark(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);
                                            lh->setLiftPark(liftNo, false);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        } /* DOWN and PARKING */
                                        else if(lh->getLiftDirection(liftNo) == -1
                                                && waitingList[queueCounter].pi->directionHeading != lh->getLiftDirection(liftNo)
                                                && lh->getLiftPark(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);
                                            lh->setLiftPark(liftNo, false);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        }
                                    }
                                }
                            }
                        }
                        else /* Lift is not moving */
                        {
                            /* Lift is at the same floor as caller */
                            if(lh->getLiftCurrentFloor(liftNo) == waitingList[queueCounter].pi->currentFloor)
                            {
                                /* Remove current floor from travelPath */
                                std::map<int,int>::iterator searchPathIter;
                                searchPathIter = travelPath[liftNo].find(waitingList[queueCounter].pi->currentFloor);
                                if(searchPathIter != travelPath[liftNo].end())
                                    travelPath[liftNo].erase(searchPathIter);

                                /* Calculate weight */
                                int weight = shh->getWeight(waitingList[queueCounter].pi->personNo, waitingList[queueCounter].pi->isResident);
                                int liftCurrentWeight = lh->getLiftCurrentWeight(liftNo);

                                /* If weight is in acceptable limit */
                                if((weight+liftCurrentWeight) <= lh->getLiftWeight(liftNo))
                                {
                                    /* Push to travelPath */
                                    travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->travellingTo, waitingList[queueCounter].pi->travellingTo));

                                    /* Get first value */
                                    int temp = travelPath[liftNo].begin()->first;

                                    /* temp < current floor = DOWN */
                                    if(temp < lh->getLiftCurrentFloor(liftNo))
                                    {
                                        lh->setLiftDirection(liftNo, -1);
                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - travelPath[liftNo].rbegin()->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, travelPath[liftNo].rbegin()->first);
                                    }
                                    else /* UP */
                                    {
                                        lh->setLiftDirection(liftNo, +1);
                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - travelPath[liftNo].begin()->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, travelPath[liftNo].begin()->first);
                                    }

                                    /* Push passenger in lift */
                                    waitingList[queueCounter].pi->isInLift = true;
                                    passengersInLift[liftNo].push_back(*waitingList[queueCounter].pi);
                                    lh->setLiftCurrentWeight(liftNo, (liftCurrentWeight+weight));

                                    /* Store passenger boarding time */
                                    waitingList[queueCounter].pi->liftBoardTime = seconds;

                                    /* Set lift move status */
                                    lh->setLiftMoveNextRound(liftNo, true);
                                    lh->setLiftIsMoving(liftNo,true);
                                }
                            }
                            else
                            {
                                /* If lift is not picking any passenger and a call is made, move lift to call */
                                if(!lh->getLiftPickPassenger(liftNo))
                                {
                                    /* Push to travelPath */
                                    travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));

                                    /* Determine next floor to travel */
                                    if(lh->getLiftDirection(liftNo) == +1)
                                    {
                                        map<int,int>::iterator it = travelPath[liftNo].upper_bound(lh->getLiftCurrentFloor(liftNo));

                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - it->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, it->first);
                                    }
                                    else if(lh->getLiftDirection(liftNo) == -1)
                                    {
                                        map<int,int>::iterator it = travelPath[liftNo].lower_bound(lh->getLiftCurrentFloor(liftNo));
                                        it--;

                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - it->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, it->first);
                                    }

                                    /* Determine direction based on travelling floor */
                                    if(lh->getLiftTravellingTo(liftNo) < lh->getLiftCurrentFloor(liftNo))
                                        lh->setLiftDirection(liftNo, -1);
                                    else
                                        lh->setLiftDirection(liftNo, +1);

                                    /* Allows lift to pick passenger */
                                    lh->setLiftPickPassenger(liftNo, true);

                                    /* Lift to start moving next seconds */
                                    lh->setLiftMoveNextRound(liftNo, true);
                                }
                            }
                        }
                    }
                }
                queueCounter++;
            }
        }

        /* Movement of all lifts */
        for(int i=0;i<noOfLifts;i++)
        {
            /* Count number of passengers waiting for current targetted lift */
            int found = count(waitingList.begin(), waitingList.end(), i);

            /* Park lift if lift has no passenger, and lift is empty and is not picking up any passenger */
            if(found <= 0 && passengersInLift[i].size() <= 0
                    && !lh->getLiftPickPassenger(i) && !lh->getLiftPark(i))
            {
                if(lh->getLiftCurrentFloor(i) != liftSector[i].first)
                {
                    /* Set direction to travel back to default floor */
                    if(liftSector[i].first >= lh->getLiftCurrentFloor(i))
                        lh->setLiftDirection(i, +1);
                    else
                        lh->setLiftDirection(i, -1);

                    /* Calculate distance to default floor */
                    int dist = abs(lh->getLiftCurrentFloor(i) - liftSector[i].first) * bh->getMetrePerFloor();
                    lh->setLiftDistanceLeft(i, dist);
                    lh->setLiftTravellingTo(i, liftSector[i].first);

                    lh->setLiftDistanceCount(i, 0);
                    lh->setLiftTotalDistance(i, 0);

                    /* Push to travelPath */
                    travelPath[i].insert(pair<int,int>(liftSector[i].first, liftSector[i].first));

                    /* Set lift status */
                    lh->setLiftIsMoving(i, true);
                    lh->setLiftPark(i, true);
                }
            }
            /* Lift movement algorithm
             * Move lift only if there are passengers,
             * lift is parking or picking up passenger
             */
            if(passengersInLift[i].size() > 0 || lh->getLiftPark(i) || lh->getLiftPickPassenger(i))
            {
                /* Set lift moving status */
                lh->setLiftIsMoving(i,true);

                // If lift is moving and moveNextRound is false, calculate movements based on speed
                if(lh->getLiftIsMoving(i) && !lh->getLiftMoveNextRound(i))
                {
                    /* Lift movement variables */
                    int distance = lh->getLiftDistanceLeft(i);
                    int speed = lh->getLiftSpeed(i);
                    int metrePerFloor = bh->getMetrePerFloor();

                    /* Continue moving until speed or distance has been met */
                    while(lh->getLiftDistanceCount(i) <= speed && lh->getLiftDistanceLeft(i) >= 0)
                    {
                        /* increase distance by 1 metre per loop */
                        lh->increaseDistance(i, 1);
                        lh->setLiftDistanceLeft(i, lh->getLiftDistanceLeft(i)-1);

                        /* Check if lift has traveled one floor */
                        if(lh->getLiftTotalDistance(i)%metrePerFloor == 0)
                        {
                            /* Update current floor */
                            if(lh->getLiftDirection(i) == +1)
                            {
                                if(lh->getLiftCurrentFloor(i) < bh->getNoOfFloor())
                                    lh->setLiftCurrentFloor(i, lh->getLiftCurrentFloor(i)+1);
                                else
                                {
                                    lh->setLiftCurrentFloor(i, bh->getNoOfFloor());
                                    lh->setLiftDirection(i, -1);
                                }
                            }
                            else
                            {
                                if(lh->getLiftCurrentFloor(i) > 1)
                                    lh->setLiftCurrentFloor(i, lh->getLiftCurrentFloor(i)-1);
                                else
                                {
                                    lh->setLiftCurrentFloor(i, 1);
                                    lh->setLiftDirection(i, +1);
                                }
                            }

                            /* If lift is at the highest floor, lift direction is DOWN */
                            if(lh->getLiftCurrentFloor(i) == bh->getNoOfFloor())
                                lh->setLiftDirection(i, -1);
                        }

                        /* If lift distance left is 0 */
                        if(lh->getLiftDistanceLeft(i) <= 0)
                        {
                            /* Remove current floor from travelPath */
                            std::map<int,int>::iterator searchPathIter;
                            searchPathIter = travelPath[i].find(lh->getLiftCurrentFloor(i));
                            if(searchPathIter != travelPath[i].end())
                                travelPath[i].erase(searchPathIter);

                            /* Reset movement variable */
                            lh->setLiftDistanceCount(i, 0);
                            lh->setLiftTotalDistance(i, 0);
                            lh->setLiftDistanceLeft(i, 0);
                            lh->setLiftIsMoving(i, false);

                            /* If travelPath is not empty, find next floor to travel */
                            if(!travelPath[i].empty())
                            {
                                /* Get first value */
                                int temp = travelPath[i].begin()->first;

                                /* temp < current floor = DOWN */
                                if(temp < lh->getLiftCurrentFloor(i))
                                {
                                    lh->setLiftDirection(i, -1);
                                    distance = abs(lh->getLiftCurrentFloor(i) - travelPath[i].rbegin()->first) * bh->getMetrePerFloor();
                                    lh->setLiftDistanceLeft(i, distance);
                                    lh->setLiftTravellingTo(i, travelPath[i].rbegin()->first);

                                }
                                else /* UP */
                                {
                                    lh->setLiftDirection(i, +1);
                                    distance = abs(lh->getLiftCurrentFloor(i) - travelPath[i].begin()->first) * bh->getMetrePerFloor();
                                    lh->setLiftDistanceLeft(i, distance);
                                    lh->setLiftTravellingTo(i, travelPath[i].begin()->first);
                                }
                            }

                            /* Reset lift status if true*/
                            if(lh->getLiftPark(i))
                                lh->setLiftPark(i, false);

                            if(lh->getLiftPickPassenger(i))
                                lh->setLiftPickPassenger(i, false);

                            /* clear passengers that reached destination */
                            vector<PassengerInfo>::iterator iter = passengersInLift[i].begin();
                            for( ; iter != passengersInLift[i].end(); )
                            {
                                iter->isInLift = false;

                                /* find passengers from waitingList that matches the info
                                 * (isResident, personNo, travelNo, travellingTo)
                                 */
                                pair<PassengerInfo, int> searchPair;
                                searchPair = make_pair(*iter, lh->getLiftCurrentFloor(i));

                                /* Find and remove passenger from waitingList that matches the searchPair */
                                vector<WaitingStatus>::iterator itr = find(waitingList.begin(),waitingList.end(), searchPair);
                                if(itr != waitingList.end())
                                {
                                    int personIndex = itr->pi->personNo;
                                    bool isResident = itr->pi->isResident;

                                    /* Time person exit lift */
                                    itr->pi->liftExitTime = seconds+1;

                                    int weight = shh->getWeight(personIndex, isResident);
                                    int liftCurrentWeight = lh->getLiftCurrentWeight(i);
                                    lh->setLiftCurrentWeight(i, (liftCurrentWeight-weight));

                                    /* Erase */
                                    waitingList.erase(itr);
                                }

                                /* Remove passengers from passengersInLift that matches the info travellingTo */
                                if(iter->travellingTo == lh->getLiftCurrentFloor(i))
                                {
                                    /* Erase */
                                    passengersInLift[i].erase(iter);
                                }
                                else
                                {
                                    ++iter;
                                }
                            }
                            break;
                        }

                        /* If speed has been met, reset and break out of loop */
                        if(lh->getLiftDistanceCount(i) >= speed)
                        {
                            lh->setLiftDistanceCount(i,0);
                            break;
                        }
                    }
                }
                else
                {
                    lh->setLiftMoveNextRound(i, false);
                }
            }
            else
            {
                if(!lh->getLiftIsMoving(i))
                {
                    int time = liftIdleTime[i];
                    liftIdleTime[i] = (time+1);
                }
                lh->setLiftIsMoving(i,false);
            }
        }
        seconds++;
    }

    /* Get summary of simulation */
    QString summary = getSummary(passengerList);

    /* Get time related information <elapsedTime, <averageWaitingTime, averageTravelTime>> */
    pair<pair<double, vector<int> >, pair<int, int> > averageTime;
    averageTime = getTiming(convertToSeconds(timer.elapsed()), passengerList, liftIdleTime);

    /* Return information <Summary, <elapsedTime, <averageWaitingTime, averageTravelTime>> */
    pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > result = make_pair(summary, averageTime);

    /* Reset variables */
    lh->resetLift(noOfFloors);
    noOfLifts = 0;
    noOfFloors = 0;
    noOfResidents = 0;
    noOfVisitors = 0;

    /* Clear data and reset data */
    waitingList.clear();
    passengerList.clear();
    passengersInLift.clear();
    liftIdleTime.clear();
    liftSector.clear();

    /* Return result */
    return result;
}

/* Fixed Sectoring Priority Timed System algorithm */
pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > Algorithm::fixedSectoringPriorityTimedSystem(BuildingHandler *bh, LiftHandler *lh, SimulatedHumanHandler *shh)
{
    /* Start timer */
    QElapsedTimer timer;
    timer.start();

    /* Simulation Variables */
    noOfLifts = lh->getNumberOfLiftsObject();
    noOfFloors = bh->getNoOfFloor();
    noOfResidents = shh->getNumberOfSimulatedHumanObject();
    noOfVisitors = shh->getNumberOfVisitorObj();

    /* Stores passenger information in current lift */
    vector<vector<PassengerInfo> > passengersInLift;
    passengersInLift.resize(noOfLifts);

    /* Store entire list of passenger information sorted ascending according to time travel */
    vector<PassengerInfo> passengerList = processPassenger(shh);

    /* Store list of passenger that pressed the button */
    vector<WaitingStatus> waitingList;

    /* Stores the path the lifts have to travel */
    vector<map<int, int> > travelPath;
    travelPath.resize(noOfLifts);

    /* Stores lift idle time */
    vector<int> liftIdleTime;
    liftIdleTime.resize(noOfLifts);
    fill(liftIdleTime.begin(), liftIdleTime.end(), 0);

    /* Divide lift into multiple sector */
    splitLiftBySector();

    /* A day consist of 86400 seconds
     * 0 = 00:00:00
     * 86399 = 23:59:59
     */
    int seconds = 0;
    while(seconds < 86399)
    {
        /* Search for passenger that matches the time and push them into waitingList */
        vector<PassengerInfo>::iterator itr = find(passengerList.begin(),passengerList.end(), seconds);
        if(itr != passengerList.end())
        {
            int totalFound = count(passengerList.begin(), passengerList.end(), seconds);
            for(int i=0;i<totalFound;i++)
            {
                WaitingStatus ws;
                ws.pi = &(*itr);
                for(int i=0;i<noOfLifts;i++)
                    ws.FS.push_back(0);
                ws.idealLift = 0;
                waitingList.push_back(ws);

                itr++;
            }
        }

        /* Call lift if waitingList is not empty */
        if(!waitingList.empty())
        {
            /* Choose an ideal lift by sector */
            getLiftBySector(waitingList);

            /* Traverse each individual person and call the lift */
            size_t queueCounter = 0;
            while(queueCounter < waitingList.size())
            {
                /* Call lift if only person is not in lift */
                if(!waitingList[queueCounter].pi->isInLift)
                {
                    int liftNo = waitingList[queueCounter].idealLift;

                    /* Check for ideal lift match */
                    if(waitingList[queueCounter].idealLift == liftNo)
                    {
                        int distance = abs(lh->getLiftCurrentFloor(liftNo) -
                                           waitingList[queueCounter].pi->currentFloor) * bh->getMetrePerFloor();

                        /* Lift is already moving */
                        if(lh->getLiftIsMoving(liftNo))
                        {
                            /* Intecept lift */
                            if(lh->getLiftTravellingTo(liftNo) != waitingList[queueCounter].pi->currentFloor)
                            {
                                /* If lift current weight is into the 30kg buffer, lift will ignore the intercept */
                                if(lh->getLiftCurrentWeight(liftNo) <= (lh->getLiftWeight(liftNo)-30))
                                {
                                    /* Intercept lift if call is within lift travellingTo and lift currentFloor */
                                    distance -= lh->getLiftTotalDistance(liftNo);

                                    if(distance <= lh->getLiftDistanceLeft(liftNo) && distance >= 0)
                                    {
                                        /* UP */
                                        if(lh->getLiftDirection(liftNo) == +1
                                                && waitingList[queueCounter].pi->directionHeading == lh->getLiftDirection(liftNo)
                                                && waitingList[queueCounter].pi->currentFloor > lh->getLiftCurrentFloor(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);

                                            if (travelPath[liftNo].find(waitingList[queueCounter].pi->currentFloor) == travelPath[liftNo].end())
                                            {
                                                /* if not found, push new data to travelPath */
                                                travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, 0));
                                            }
                                            else
                                            {
                                                /* if found, update priority time */
                                                int tempCurFloor = waitingList[queueCounter].pi->currentFloor;
                                                int priorityTime = travelPath[liftNo][tempCurFloor];
                                                travelPath[liftNo][tempCurFloor] = priorityTime+1; // increase by 1
                                            }

                                        } /* DOWN */
                                        else if(lh->getLiftDirection(liftNo) == -1
                                                && waitingList[queueCounter].pi->directionHeading == lh->getLiftDirection(liftNo)
                                                && waitingList[queueCounter].pi->currentFloor < lh->getLiftCurrentFloor(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);

                                            if (travelPath[liftNo].find(waitingList[queueCounter].pi->currentFloor) == travelPath[liftNo].end())
                                            {
                                                /* if not found, push new data to travelPath */
                                                travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, 0));
                                            }
                                            else
                                            {
                                                /* if found, update priority time */
                                                int tempCurFloor = waitingList[queueCounter].pi->currentFloor;
                                                int priorityTime = travelPath[liftNo][tempCurFloor];
                                                travelPath[liftNo][tempCurFloor] = priorityTime+1; // increase by 1
                                            }

                                        } /* UP and PARKING */
                                        else if(lh->getLiftDirection(liftNo) == +1
                                                && waitingList[queueCounter].pi->directionHeading != lh->getLiftDirection(liftNo)
                                                && lh->getLiftPark(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);
                                            lh->setLiftPark(liftNo, false);

                                            if (travelPath[liftNo].find(waitingList[queueCounter].pi->currentFloor) == travelPath[liftNo].end())
                                            {
                                                /* if not found, push new data to travelPath */
                                                travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, 0));
                                            }
                                            else
                                            {
                                                /* if found, update priority time */
                                                int tempCurFloor = waitingList[queueCounter].pi->currentFloor;
                                                int priorityTime = travelPath[liftNo][tempCurFloor];
                                                travelPath[liftNo][tempCurFloor] = priorityTime+1; // increase by 1
                                            }

                                        } /* DOWN and PARKING */
                                        else if(lh->getLiftDirection(liftNo) == -1
                                                && waitingList[queueCounter].pi->directionHeading != lh->getLiftDirection(liftNo)
                                                && lh->getLiftPark(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);
                                            lh->setLiftPark(liftNo, false);

                                            if (travelPath[liftNo].find(waitingList[queueCounter].pi->currentFloor) == travelPath[liftNo].end())
                                            {
                                                /* if not found, push new data to travelPath */
                                                travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, 0));
                                            }
                                            else
                                            {
                                                /* if found, update priority time */
                                                int tempCurFloor = waitingList[queueCounter].pi->currentFloor;
                                                int priorityTime = travelPath[liftNo][tempCurFloor];
                                                travelPath[liftNo][tempCurFloor] = priorityTime+1; // increase by 1
                                            }

                                        }
                                    }
                                }
                            }
                        }
                        else /* Lift is not moving */
                        {
                            /* Lift is at the same floor as caller */
                            if(lh->getLiftCurrentFloor(liftNo) == waitingList[queueCounter].pi->currentFloor)
                            {
                                /* Remove current floor from travelPath */
                                std::map<int,int>::iterator searchPathIter;
                                searchPathIter = travelPath[liftNo].find(waitingList[queueCounter].pi->currentFloor);
                                if(searchPathIter != travelPath[liftNo].end())
                                    travelPath[liftNo].erase(searchPathIter);

                                /* Calculate weight */
                                int weight = shh->getWeight(waitingList[queueCounter].pi->personNo, waitingList[queueCounter].pi->isResident);
                                int liftCurrentWeight = lh->getLiftCurrentWeight(liftNo);

                                /* If weight is in acceptable limit */
                                if((weight+liftCurrentWeight) <= lh->getLiftWeight(liftNo))
                                {
                                    /* Push to travelPath */
                                    travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->travellingTo, 0));

                                    /* Get first value */
                                    int temp = travelPath[liftNo].begin()->first;

                                    /* temp < current floor = DOWN */
                                    if(temp < lh->getLiftCurrentFloor(liftNo))
                                    {
                                        lh->setLiftDirection(liftNo, -1);
                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - travelPath[liftNo].rbegin()->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, travelPath[liftNo].rbegin()->first);
                                    }
                                    else /* UP */
                                    {
                                        lh->setLiftDirection(liftNo, +1);
                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - travelPath[liftNo].begin()->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, travelPath[liftNo].begin()->first);
                                    }

                                    /* Push passenger in lift */
                                    waitingList[queueCounter].pi->isInLift = true;
                                    passengersInLift[liftNo].push_back(*waitingList[queueCounter].pi);
                                    lh->setLiftCurrentWeight(liftNo, (liftCurrentWeight+weight));

                                    /* Store passenger boarding time */
                                    waitingList[queueCounter].pi->liftBoardTime = seconds;

                                    /* Set lift move status */
                                    lh->setLiftMoveNextRound(liftNo, true);
                                    lh->setLiftIsMoving(liftNo,true);
                                }
                            }
                            else
                            {
                                /* If lift is not picking any passenger and a call is made, move lift to call */
                                if(!lh->getLiftPickPassenger(liftNo))
                                {
                                    /* Push to travelPath */
                                    travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, 0));

                                    /* Determine next floor to travel */
                                    if(lh->getLiftDirection(liftNo) == +1)
                                    {
                                        map<int,int>::iterator it = travelPath[liftNo].upper_bound(lh->getLiftCurrentFloor(liftNo));

                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - it->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, it->first);
                                    }
                                    else if(lh->getLiftDirection(liftNo) == -1)
                                    {
                                        map<int,int>::iterator it = travelPath[liftNo].lower_bound(lh->getLiftCurrentFloor(liftNo));
                                        it--;

                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - it->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, it->first);
                                    }

                                    /* Determine direction based on travelling floor */
                                    if(lh->getLiftTravellingTo(liftNo) < lh->getLiftCurrentFloor(liftNo))
                                        lh->setLiftDirection(liftNo, -1);
                                    else
                                        lh->setLiftDirection(liftNo, +1);

                                    /* Allows lift to pick passenger */
                                    lh->setLiftPickPassenger(liftNo, true);

                                    /* Lift to start moving next seconds */
                                    lh->setLiftMoveNextRound(liftNo, true);
                                }
                            }
                        }
                    }
                }
                queueCounter++;
            }
        }

        /* Movement of all lifts */
        for(int i=0;i<noOfLifts;i++)
        {
            /* Count number of passengers waiting for current targetted lift */
            int found = count(waitingList.begin(), waitingList.end(), i);

            /* Park lift if lift has no passenger, and lift is empty and is not picking up any passenger */
            if(found <= 0 && passengersInLift[i].size() <= 0
                    && !lh->getLiftPickPassenger(i) && !lh->getLiftPark(i))
            {
                if(lh->getLiftCurrentFloor(i) != liftSector[i].first)
                {
                    /* Set direction to travel back to default floor */
                    if(liftSector[i].first >= lh->getLiftCurrentFloor(i))
                        lh->setLiftDirection(i, +1);
                    else
                        lh->setLiftDirection(i, -1);

                    /* Calculate distance to default floor */
                    int dist = abs(lh->getLiftCurrentFloor(i) - liftSector[i].first) * bh->getMetrePerFloor();
                    lh->setLiftDistanceLeft(i, dist);
                    lh->setLiftTravellingTo(i, liftSector[i].first);

                    lh->setLiftDistanceCount(i, 0);
                    lh->setLiftTotalDistance(i, 0);

                    /* Push to travelPath */
                    travelPath[i].insert(pair<int,int>(liftSector[i].first, 0));

                    /* Set lift status */
                    lh->setLiftIsMoving(i, true);
                    lh->setLiftPark(i, true);
                }
            }
            /* Lift movement algorithm
             * Move lift only if there are passengers,
             * lift is parking or picking up passenger
             */
            if(passengersInLift[i].size() > 0 || lh->getLiftPark(i) || lh->getLiftPickPassenger(i))
            {
                /* Set lift moving status */
                lh->setLiftIsMoving(i,true);

                // If lift is moving and moveNextRound is false, calculate movements based on speed
                if(lh->getLiftIsMoving(i) && !lh->getLiftMoveNextRound(i))
                {
                    /* Lift movement variables */
                    int distance = lh->getLiftDistanceLeft(i);
                    int speed = lh->getLiftSpeed(i);
                    int metrePerFloor = bh->getMetrePerFloor();

                    /* Continue moving until speed or distance has been met */
                    while(lh->getLiftDistanceCount(i) <= speed && lh->getLiftDistanceLeft(i) >= 0)
                    {
                        /* increase distance by 1 metre per loop */
                        lh->increaseDistance(i, 1);
                        lh->setLiftDistanceLeft(i, lh->getLiftDistanceLeft(i)-1);

                        /* Check if lift has traveled one floor */
                        if(lh->getLiftTotalDistance(i)%metrePerFloor == 0)
                        {
                            /* Update current floor */
                            if(lh->getLiftDirection(i) == +1)
                            {
                                if(lh->getLiftCurrentFloor(i) < bh->getNoOfFloor())
                                    lh->setLiftCurrentFloor(i, lh->getLiftCurrentFloor(i)+1);
                                else
                                {
                                    lh->setLiftCurrentFloor(i, bh->getNoOfFloor());
                                    lh->setLiftDirection(i, -1);
                                }
                            }
                            else
                            {
                                if(lh->getLiftCurrentFloor(i) > 1)
                                    lh->setLiftCurrentFloor(i, lh->getLiftCurrentFloor(i)-1);
                                else
                                {
                                    lh->setLiftCurrentFloor(i, 1);
                                    lh->setLiftDirection(i, +1);
                                }
                            }

                            /* If lift is at the highest floor, lift direction is DOWN */
                            if(lh->getLiftCurrentFloor(i) == bh->getNoOfFloor())
                                lh->setLiftDirection(i, -1);
                        }

                        /* If lift distance left is 0 */
                        if(lh->getLiftDistanceLeft(i) <= 0)
                        {
                            /* Remove current floor from travelPath */
                            std::map<int,int>::iterator searchPathIter;
                            searchPathIter = travelPath[i].find(lh->getLiftCurrentFloor(i));
                            if(searchPathIter != travelPath[i].end())
                                travelPath[i].erase(searchPathIter);

                            /* Reset movement variable */
                            lh->setLiftDistanceCount(i, 0);
                            lh->setLiftTotalDistance(i, 0);
                            lh->setLiftDistanceLeft(i, 0);
                            lh->setLiftIsMoving(i, false);

                            /* If travelPath is not empty, find next floor to travel */
                            if(!travelPath[i].empty())
                            {
                                /* Get first value */
                                int temp = travelPath[i].begin()->first;

                                /* temp < current floor = DOWN */
                                if(temp < lh->getLiftCurrentFloor(i))
                                {
                                    int floor = getFloorWithHighestPriority(travelPath[i], lh->getLiftCurrentFloor(i), -1);

                                    lh->setLiftDirection(i, -1);
                                    distance = abs(lh->getLiftCurrentFloor(i) - floor) * bh->getMetrePerFloor();
                                    lh->setLiftDistanceLeft(i, distance);
                                    lh->setLiftTravellingTo(i, floor);

                                }
                                else /* UP */
                                {
                                    int floor = getFloorWithHighestPriority(travelPath[i], lh->getLiftCurrentFloor(i), +1);

                                    lh->setLiftDirection(i, +1);
                                    distance = abs(lh->getLiftCurrentFloor(i) - floor) * bh->getMetrePerFloor();
                                    lh->setLiftDistanceLeft(i, distance);
                                    lh->setLiftTravellingTo(i, floor);
                                }
                            }

                            /* Reset lift status if true*/
                            if(lh->getLiftPark(i))
                                lh->setLiftPark(i, false);

                            if(lh->getLiftPickPassenger(i))
                                lh->setLiftPickPassenger(i, false);

                            /* clear passengers that reached destination */
                            vector<PassengerInfo>::iterator iter = passengersInLift[i].begin();
                            for( ; iter != passengersInLift[i].end(); )
                            {
                                iter->isInLift = false;

                                /* find passengers from waitingList that matches the info
                                 * (isResident, personNo, travelNo, travellingTo)
                                 */
                                pair<PassengerInfo, int> searchPair;
                                searchPair = make_pair(*iter, lh->getLiftCurrentFloor(i));

                                /* Find and remove passenger from waitingList that matches the searchPair */
                                vector<WaitingStatus>::iterator itr = find(waitingList.begin(),waitingList.end(), searchPair);
                                if(itr != waitingList.end())
                                {
                                    int personIndex = itr->pi->personNo;
                                    bool isResident = itr->pi->isResident;

                                    /* Time person exit lift */
                                    itr->pi->liftExitTime = seconds+1;

                                    int weight = shh->getWeight(personIndex, isResident);
                                    int liftCurrentWeight = lh->getLiftCurrentWeight(i);
                                    lh->setLiftCurrentWeight(i, (liftCurrentWeight-weight));

                                    /* Erase */
                                    waitingList.erase(itr);
                                }

                                /* Remove passengers from passengersInLift that matches the info travellingTo */
                                if(iter->travellingTo == lh->getLiftCurrentFloor(i))
                                {
                                    /* Erase */
                                    passengersInLift[i].erase(iter);
                                }
                                else
                                {
                                    ++iter;
                                }
                            }
                            break;
                        }

                        /* If speed has been met, reset and break out of loop */
                        if(lh->getLiftDistanceCount(i) >= speed)
                        {
                            lh->setLiftDistanceCount(i,0);
                            break;
                        }
                    }
                }
                else
                {
                    lh->setLiftMoveNextRound(i, false);
                }
            }
            else
            {
                if(!lh->getLiftIsMoving(i))
                {
                    int time = liftIdleTime[i];
                    liftIdleTime[i] = (time+1);
                }
                lh->setLiftIsMoving(i,false);
            }
        }
        seconds++;
    }

    /* Get summary of simulation */
    QString summary = getSummary(passengerList);

    /* Get time related information <elapsedTime, <averageWaitingTime, averageTravelTime>> */
    pair<pair<double, vector<int> >, pair<int, int> > averageTime;
    averageTime = getTiming(convertToSeconds(timer.elapsed()), passengerList, liftIdleTime);

    /* Return information <Summary, <elapsedTime, <averageWaitingTime, averageTravelTime>> */
    pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > result = make_pair(summary, averageTime);

    /* Reset variables */
    lh->resetLift(noOfFloors);
    noOfLifts = 0;
    noOfFloors = 0;
    noOfResidents = 0;
    noOfVisitors = 0;

    /* Clear data and reset data */
    waitingList.clear();
    passengerList.clear();
    passengersInLift.clear();
    liftIdleTime.clear();
    liftSector.clear();

    /* Return result */
    return result;
}

/* Dynamic Sectoring System algorithm */
pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > Algorithm::dynamicSectoringSystem(BuildingHandler *bh, LiftHandler *lh, SimulatedHumanHandler *shh)
{
    /* Start timer */
    QElapsedTimer timer;
    timer.start();

    /* Simulation Variables */
    noOfLifts = lh->getNumberOfLiftsObject();
    noOfFloors = bh->getNoOfFloor();
    noOfResidents = shh->getNumberOfSimulatedHumanObject();
    noOfVisitors = shh->getNumberOfVisitorObj();

    /* Stores passenger information in current lift */
    vector<vector<PassengerInfo> > passengersInLift;
    passengersInLift.resize(noOfLifts);

    /* Store entire list of passenger information sorted ascending according to time travel */
    vector<PassengerInfo> passengerList = processPassenger(shh);

    /* Store list of passenger that pressed the button */
    vector<WaitingStatus> waitingList;

    /* Stores the path the lifts have to travel */
    vector<map<int, int> > travelPath;
    travelPath.resize(noOfLifts);

    /* Stores lift idle time */
    vector<int> liftIdleTime;
    liftIdleTime.resize(noOfLifts);
    fill(liftIdleTime.begin(), liftIdleTime.end(), 0);


    /* A day consist of 86400 seconds
     * 0 = 00:00:00
     * 86399 = 23:59:59
     */
    int seconds = 0;
    while(seconds < 86399)
    {
        /* Search for passenger that matches the time and push them into waitingList */
        vector<PassengerInfo>::iterator itr = find(passengerList.begin(),passengerList.end(), seconds);
        if(itr != passengerList.end())
        {
            int totalFound = count(passengerList.begin(), passengerList.end(), seconds);
            for(int i=0;i<totalFound;i++)
            {
                WaitingStatus ws;
                ws.pi = &(*itr);
                for(int i=0;i<noOfLifts;i++)
                    ws.FS.push_back(0);
                ws.idealLift = 0;
                waitingList.push_back(ws);

                itr++;
            }
        }

        /* Call lift if waitingList is not empty */
        if(!waitingList.empty())
        {
            /* Compute dynamic sector for each lift and choose an ideal lift */
            computeDynamicSector(waitingList, lh);

            /* Traverse each individual person and call the lift */
            size_t queueCounter = 0;
            while(queueCounter < waitingList.size())
            {
                /* Call lift if only person is not in lift */
                if(!waitingList[queueCounter].pi->isInLift)
                {
                    int liftNo = waitingList[queueCounter].idealLift;

                    /* Check for ideal lift match */
                    if(waitingList[queueCounter].idealLift == liftNo)
                    {
                        int distance = abs(lh->getLiftCurrentFloor(liftNo) -
                                           waitingList[queueCounter].pi->currentFloor) * bh->getMetrePerFloor();

                        /* Lift is already moving */
                        if(lh->getLiftIsMoving(liftNo))
                        {
                            /* Intecept lift */
                            if(lh->getLiftTravellingTo(liftNo) != waitingList[queueCounter].pi->currentFloor)
                            {
                                /* If lift current weight is into the 30kg buffer, lift will ignore the intercept */
                                if(lh->getLiftCurrentWeight(liftNo) <= (lh->getLiftWeight(liftNo)-30))
                                {
                                    /* Intercept lift if call is within lift travellingTo and lift currentFloor */
                                    distance -= lh->getLiftTotalDistance(liftNo);

                                    if(distance <= lh->getLiftDistanceLeft(liftNo) && distance >= 0)
                                    {
                                        /* UP */
                                        if(lh->getLiftDirection(liftNo) == +1
                                                && waitingList[queueCounter].pi->directionHeading == lh->getLiftDirection(liftNo)
                                                && waitingList[queueCounter].pi->currentFloor > lh->getLiftCurrentFloor(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        } /* DOWN */
                                        else if(lh->getLiftDirection(liftNo) == -1
                                                && waitingList[queueCounter].pi->directionHeading == lh->getLiftDirection(liftNo)
                                                && waitingList[queueCounter].pi->currentFloor < lh->getLiftCurrentFloor(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        } /* UP and PARKING */
                                        else if(lh->getLiftDirection(liftNo) == +1
                                                && waitingList[queueCounter].pi->directionHeading != lh->getLiftDirection(liftNo)
                                                && lh->getLiftPark(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);
                                            lh->setLiftPark(liftNo, false);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        } /* DOWN and PARKING */
                                        else if(lh->getLiftDirection(liftNo) == -1
                                                && waitingList[queueCounter].pi->directionHeading != lh->getLiftDirection(liftNo)
                                                && lh->getLiftPark(liftNo))
                                        {
                                            /* Calculate distance */
                                            lh->setLiftDistanceLeft(liftNo, distance);
                                            lh->setLiftTravellingTo(liftNo, waitingList[queueCounter].pi->currentFloor);
                                            lh->setLiftPark(liftNo, false);

                                            /* push to travelPath */
                                            travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));
                                        }
                                    }
                                }
                            }
                        }
                        else /* Lift is not moving */
                        {
                            /* Lift is at the same floor as caller */
                            if(lh->getLiftCurrentFloor(liftNo) == waitingList[queueCounter].pi->currentFloor)
                            {
                                /* Remove current floor from travelPath */
                                std::map<int,int>::iterator searchPathIter;
                                searchPathIter = travelPath[liftNo].find(waitingList[queueCounter].pi->currentFloor);
                                if(searchPathIter != travelPath[liftNo].end())
                                    travelPath[liftNo].erase(searchPathIter);

                                /* Calculate weight */
                                int weight = shh->getWeight(waitingList[queueCounter].pi->personNo, waitingList[queueCounter].pi->isResident);
                                int liftCurrentWeight = lh->getLiftCurrentWeight(liftNo);

                                /* If weight is in acceptable limit */
                                if((weight+liftCurrentWeight) <= lh->getLiftWeight(liftNo))
                                {
                                    /* Push to travelPath */
                                    travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->travellingTo, waitingList[queueCounter].pi->travellingTo));

                                    /* Get first value */
                                    int temp = travelPath[liftNo].begin()->first;

                                    /* temp < current floor = DOWN */
                                    if(temp < lh->getLiftCurrentFloor(liftNo))
                                    {
                                        lh->setLiftDirection(liftNo, -1);
                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - travelPath[liftNo].rbegin()->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, travelPath[liftNo].rbegin()->first);
                                    }
                                    else /* UP */
                                    {
                                        lh->setLiftDirection(liftNo, +1);
                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - travelPath[liftNo].begin()->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, travelPath[liftNo].begin()->first);
                                    }

                                    /* Push passenger in lift */
                                    waitingList[queueCounter].pi->isInLift = true;
                                    passengersInLift[liftNo].push_back(*waitingList[queueCounter].pi);
                                    lh->setLiftCurrentWeight(liftNo, (liftCurrentWeight+weight));

                                    /* Store passenger boarding time */
                                    waitingList[queueCounter].pi->liftBoardTime = seconds;

                                    /* Set lift move status */
                                    lh->setLiftMoveNextRound(liftNo, true);
                                    lh->setLiftIsMoving(liftNo,true);
                                }
                            }
                            else
                            {
                                /* If lift is not picking any passenger and a call is made, move lift to call */
                                if(!lh->getLiftPickPassenger(liftNo))
                                {
                                    /* Push to travelPath */
                                    travelPath[liftNo].insert(pair<int,int>(waitingList[queueCounter].pi->currentFloor, waitingList[queueCounter].pi->currentFloor));

                                    /* Determine next floor to travel */
                                    if(lh->getLiftDirection(liftNo) == +1)
                                    {
                                        map<int,int>::iterator it = travelPath[liftNo].upper_bound(lh->getLiftCurrentFloor(liftNo));

                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - it->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, it->first);
                                    }
                                    else if(lh->getLiftDirection(liftNo) == -1)
                                    {
                                        map<int,int>::iterator it = travelPath[liftNo].lower_bound(lh->getLiftCurrentFloor(liftNo));
                                        it--;

                                        distance = abs(lh->getLiftCurrentFloor(liftNo) - it->first) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftNo, distance);
                                        lh->setLiftTravellingTo(liftNo, it->first);
                                    }

                                    /* Determine direction based on travelling floor */
                                    if(lh->getLiftTravellingTo(liftNo) < lh->getLiftCurrentFloor(liftNo))
                                        lh->setLiftDirection(liftNo, -1);
                                    else
                                        lh->setLiftDirection(liftNo, +1);

                                    /* Allows lift to pick passenger */
                                    lh->setLiftPickPassenger(liftNo, true);

                                    /* Lift to start moving next seconds */
                                    lh->setLiftMoveNextRound(liftNo, true);

                                }
                            }
                        }
                    }
                }
                queueCounter++;
            }
        }

        /* Movement of all lifts */
        for(int i=0;i<noOfLifts;i++)
        {
            /* Count number of passengers waiting for current targetted lift */
            int found = count(waitingList.begin(), waitingList.end(), i);

            /* Park lift if lift has no passenger, and lift is empty and is not picking up any passenger */
            if(found <= 0 && passengersInLift[i].size() <= 0
                    && !lh->getLiftPickPassenger(i) && !lh->getLiftPark(i))
            {
                if(lh->getLiftCurrentFloor(i) != lh->getLiftDefaultFloor(i))
                {
                    /* Set direction to travel back to default floor */
                    if(lh->getLiftDefaultFloor(i) >= lh->getLiftCurrentFloor(i))
                        lh->setLiftDirection(i, +1);
                    else
                        lh->setLiftDirection(i, -1);

                    /* Calculate distance to default floor */
                    int dist = abs(lh->getLiftCurrentFloor(i) - lh->getLiftDefaultFloor(i)) * bh->getMetrePerFloor();
                    lh->setLiftDistanceLeft(i, dist);
                    lh->setLiftTravellingTo(i, lh->getLiftDefaultFloor(i));

                    lh->setLiftDistanceCount(i, 0);
                    lh->setLiftTotalDistance(i, 0);

                    /* Push to travelPath */
                    travelPath[i].insert(pair<int,int>(lh->getLiftDefaultFloor(i), lh->getLiftDefaultFloor(i)));

                    /* Set lift status */
                    lh->setLiftIsMoving(i, true);
                    lh->setLiftPark(i, true);
                }
            }
            /* Lift movement algorithm
             * Move lift only if there are passengers,
             * lift is parking or picking up passenger
             */
            if(passengersInLift[i].size() > 0 || lh->getLiftPark(i) || lh->getLiftPickPassenger(i))
            {
                /* Set lift moving status */
                lh->setLiftIsMoving(i,true);

                // If lift is moving and moveNextRound is false, calculate movements based on speed
                if(lh->getLiftIsMoving(i) && !lh->getLiftMoveNextRound(i))
                {
                    /* Lift movement variables */
                    int distance = lh->getLiftDistanceLeft(i);
                    int speed = lh->getLiftSpeed(i);
                    int metrePerFloor = bh->getMetrePerFloor();

                    /* Continue moving until speed or distance has been met */
                    while(lh->getLiftDistanceCount(i) <= speed && lh->getLiftDistanceLeft(i) >= 0)
                    {
                        /* increase distance by 1 metre per loop */
                        lh->increaseDistance(i, 1);
                        lh->setLiftDistanceLeft(i, lh->getLiftDistanceLeft(i)-1);

                        /* Check if lift has traveled one floor */
                        if(lh->getLiftTotalDistance(i)%metrePerFloor == 0)
                        {
                            /* Update current floor */
                            if(lh->getLiftDirection(i) == +1)
                            {
                                if(lh->getLiftCurrentFloor(i) < bh->getNoOfFloor())
                                    lh->setLiftCurrentFloor(i, lh->getLiftCurrentFloor(i)+1);
                                else
                                {
                                    lh->setLiftCurrentFloor(i, bh->getNoOfFloor());
                                    lh->setLiftDirection(i, -1);
                                }
                            }
                            else
                            {
                                if(lh->getLiftCurrentFloor(i) > 1)
                                    lh->setLiftCurrentFloor(i, lh->getLiftCurrentFloor(i)-1);
                                else
                                {
                                    lh->setLiftCurrentFloor(i, 1);
                                    lh->setLiftDirection(i, +1);
                                }
                            }

                            /* If lift is at the highest floor, lift direction is DOWN */
                            if(lh->getLiftCurrentFloor(i) == bh->getNoOfFloor())
                                lh->setLiftDirection(i, -1);

                        }

                        /* If lift distance left is 0 */
                        if(lh->getLiftDistanceLeft(i) <= 0)
                        {
                            /* Remove current floor from travelPath */
                            std::map<int,int>::iterator searchPathIter;
                            searchPathIter = travelPath[i].find(lh->getLiftCurrentFloor(i));
                            if(searchPathIter != travelPath[i].end())
                                travelPath[i].erase(searchPathIter);

                            /* Reset movement variable */
                            lh->setLiftDistanceCount(i, 0);
                            lh->setLiftTotalDistance(i, 0);
                            lh->setLiftDistanceLeft(i, 0);
                            lh->setLiftIsMoving(i, false);

                            /* If travelPath is not empty, find next floor to travel */
                            if(!travelPath[i].empty())
                            {
                                /* Get first value */
                                int temp = travelPath[i].begin()->first;

                                /* temp < current floor = DOWN */
                                if(temp < lh->getLiftCurrentFloor(i))
                                {
                                    lh->setLiftDirection(i, -1);
                                    distance = abs(lh->getLiftCurrentFloor(i) - travelPath[i].rbegin()->first) * bh->getMetrePerFloor();
                                    lh->setLiftDistanceLeft(i, distance);
                                    lh->setLiftTravellingTo(i, travelPath[i].rbegin()->first);

                                }
                                else /* UP */
                                {
                                    lh->setLiftDirection(i, +1);
                                    distance = abs(lh->getLiftCurrentFloor(i) - travelPath[i].begin()->first) * bh->getMetrePerFloor();
                                    lh->setLiftDistanceLeft(i, distance);
                                    lh->setLiftTravellingTo(i, travelPath[i].begin()->first);
                                }
                            }

                            /* Reset lift status if true*/
                            if(lh->getLiftPark(i))
                                lh->setLiftPark(i, false);

                            if(lh->getLiftPickPassenger(i))
                                lh->setLiftPickPassenger(i, false);

                            /* clear passengers that reached destination */
                            vector<PassengerInfo>::iterator iter = passengersInLift[i].begin();
                            for( ; iter != passengersInLift[i].end(); )
                            {
                                iter->isInLift = false;

                                /* find passengers from waitingList that matches the info
                                 * (isResident, personNo, travelNo, travellingTo)
                                 */
                                pair<PassengerInfo, int> searchPair;
                                searchPair = make_pair(*iter, lh->getLiftCurrentFloor(i));

                                /* Find and remove passenger from waitingList that matches the searchPair */
                                vector<WaitingStatus>::iterator itr = find(waitingList.begin(),waitingList.end(), searchPair);
                                if(itr != waitingList.end())
                                {
                                    int personIndex = itr->pi->personNo;
                                    bool isResident = itr->pi->isResident;

                                    /* Time person exit lift */
                                    itr->pi->liftExitTime = seconds+1;

                                    int weight = shh->getWeight(personIndex, isResident);
                                    int liftCurrentWeight = lh->getLiftCurrentWeight(i);
                                    lh->setLiftCurrentWeight(i, (liftCurrentWeight-weight));

                                    /* Erase */
                                    waitingList.erase(itr);
                                }

                                /* Remove passengers from passengersInLift that matches the info travellingTo */
                                if(iter->travellingTo == lh->getLiftCurrentFloor(i))
                                {
                                    /* Erase */
                                    passengersInLift[i].erase(iter);
                                }
                                else
                                {
                                    ++iter;
                                }
                            }
                            break;
                        }

                        /* If speed has been met, reset and break out of loop */
                        if(lh->getLiftDistanceCount(i) >= speed)
                        {
                            lh->setLiftDistanceCount(i,0);
                            break;
                        }
                    }
                }
                else
                {
                    lh->setLiftMoveNextRound(i, false);
                }
            }
            else
            {
                if(!lh->getLiftIsMoving(i))
                {
                    int time = liftIdleTime[i];
                    liftIdleTime[i] = (time+1);
                }
                lh->setLiftIsMoving(i,false);
            }
        }
        seconds++;
    }

    /* Get summary of simulation */
    QString summary = getSummary(passengerList);

    /* Get time related information <elapsedTime, <averageWaitingTime, averageTravelTime>> */
    pair<pair<double, vector<int> >, pair<int, int> > averageTime;
    averageTime = getTiming(convertToSeconds(timer.elapsed()), passengerList, liftIdleTime);

    /* Return information <Summary, <elapsedTime, <averageWaitingTime, averageTravelTime>> */
    pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > result = make_pair(summary, averageTime);

    /* Reset variables */
    lh->resetLift(noOfFloors);
    noOfLifts = 0;
    noOfFloors = 0;
    noOfResidents = 0;
    noOfVisitors = 0;

    /* Clear data and reset data */
    waitingList.clear();
    passengerList.clear();
    passengersInLift.clear();
    liftIdleTime.clear();
    liftSector.clear();

    /* Return result */
    return result;
}

double Algorithm::convertToSeconds(qint64 ms)
{
    return ms/1000.00;
}

/* Push all passenger into passengerList and sort by timing they are travelling */
vector<Algorithm::PassengerInfo> Algorithm::processPassenger(SimulatedHumanHandler *shh)
{
    vector<PassengerInfo> passengerList;

    PassengerInfo pi;

    /* Get data of all residents and push into passenger list */
    for(int i=0; i<shh->getNumberOfSimulatedHumanObject();i++)
    {
        for(int j=0;j<shh->getNoOfTimesTravel(i, true);j++)
        {

            pi.isResident = true;

            pi.travelTime = shh->getTravelTime(i,j,pi.isResident);
            pi.personNo = i;
            pi.travelNo = j;

            pi.liftPressTime = shh->getTravelTime(i,j,pi.isResident);
            pi.liftBoardTime = 0;
            pi.liftExitTime = 0;

            pi.travellingTo = shh->getTravelFloor(i,j,pi.isResident);

            if(pi.travellingTo != 1)
            {
                pi.currentFloor = 1;
                pi.directionHeading = +1;
            }
            else
            {
                pi.currentFloor = shh->getResident(i, pi.isResident);
                pi.directionHeading = -1;
            }

            pi.isInLift = false;

            passengerList.push_back(pi);
        }
    }

    /* Get data of all visitors and push into passenger list */
    for(int i=0; i<shh->getNumberOfVisitorObj();i++)
    {
        for(int j=0;j<shh->getNoOfTimesTravel(i, false);j++)
        {
            pi.isResident = false;

            pi.travelTime = shh->getTravelTime(i,j,pi.isResident);
            pi.personNo = i;
            pi.travelNo = j;

            pi.liftPressTime = shh->getTravelTime(i,j,pi.isResident);
            pi.liftBoardTime = 0;
            pi.liftExitTime = 0;

            pi.travellingTo = shh->getTravelFloor(i,j,pi.isResident);

            if(pi.travellingTo != 1)
            {
                pi.currentFloor = 1;
                pi.directionHeading = +1;
            }
            else
            {
                if(j != 0)
                    pi.currentFloor = shh->getTravelFloor(i,j-1,pi.isResident);

                pi.directionHeading = -1;
            }

            pi.isInLift = false;

            passengerList.push_back(pi);
        }
    }

    /* Sort passengerList based on travelTime (ascending) */
    std::sort(passengerList.begin(), passengerList.end());

    return passengerList;
}

/* Compute FS for individual passenger in the waitingList (parse by reference) */
void Algorithm::computeFS(vector<WaitingStatus>& waitingList, LiftHandler* lh)
{
    // for each individual people waiting for the lift
    for(size_t i=0;i<waitingList.size();i++)
    {
        if(!waitingList[i].pi->isInLift)
        {
            int highestFS = 0;

            //update the FS for each lift
            for(int j=0;j<noOfLifts;j++)
            {
                int FS = 0;

                int currentFloor = waitingList[i].pi->currentFloor;
                int callDirection = waitingList[i].pi->directionHeading;

                int d = currentFloor - lh->getLiftCurrentFloor(j);
                int N = noOfFloors-1;

                int liftDirection = lh->getLiftDirection(j);

                if(d / liftDirection >= 0)
                {
                    if(liftDirection  == callDirection)
                        FS = (N + 2) - abs(d);
                    else
                        FS = (N + 1) - abs(d);
                }
                else
                {
                    FS = 1;
                }

                // Store FS
                waitingList[i].FS[j] = FS;

                // Selecting most ideal lift
                if (FS > highestFS)
                {
                    highestFS = FS;
                    waitingList[i].idealLift = j;
                }
            }
        }
    }
}

/* Get lift by sector */
void Algorithm::getLiftBySector(vector<WaitingStatus>& waitingList)
{
    // for each individual people waiting for the lift
    for(size_t i=0;i<waitingList.size();i++)
    {
        if(!waitingList[i].pi->isInLift)
        {
            int currentFloor = waitingList[i].pi->currentFloor;
            int travellingTo = waitingList[i].pi->travellingTo;
            int callDirection = waitingList[i].pi->directionHeading;

            if(noOfFloors < 8)
                waitingList[i].idealLift = rand() % noOfLifts;
            else
            {
                /* UP */
                if(callDirection == +1)
                {
                    // check travellingTo matches lift range
                    for(size_t k=0;k<liftSector.size();k++)
                    {
                        if(travellingTo >= liftSector[k].first && travellingTo <= liftSector[k].second)
                        {
                            waitingList[i].idealLift = k;
                            break;
                        }
                    }
                }
                else if(callDirection == -1) /* UP */
                {
                    // checks currentfloor matches lift range
                    for(size_t k=0;k<liftSector.size();k++)
                    {
                        if(currentFloor >= liftSector[k].first && currentFloor <= liftSector[k].second)
                        {
                            waitingList[i].idealLift = k;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Algorithm::splitLiftBySector()
{
    if(noOfFloors < 8)
    {
        for(int i=0;i<noOfLifts;i++)
        {
            pair<int, int> sector;
            sector = make_pair(1, noOfFloors);
            liftSector.push_back(sector);
        }
    }
    else
    {
        int sectorRange = noOfFloors/noOfLifts;
        int startRange = 1;
        int endRange = noOfFloors/noOfLifts;

        for(int i=0;i<noOfLifts;i++)
        {
            pair<int, int> sector;
            sector = make_pair(startRange, endRange);
            liftSector.push_back(sector);

            startRange = endRange+1;

            if(i == noOfLifts-2)
                endRange = noOfFloors;
            else
                endRange = endRange+sectorRange;
        }
    }
}

/* Compute lift sector dynamically */
void Algorithm::computeDynamicSector(vector<WaitingStatus>& waitingList, LiftHandler* lh)
{
    liftSector.clear();

    int range = noOfFloors/noOfLifts;
    for(int i=0;i<noOfLifts;i++)
    {
        pair<int, int> sector;
        sector = make_pair(lh->getLiftCurrentFloor(i)-range, lh->getLiftCurrentFloor(i)+range);
        liftSector.push_back(sector);
    }

    // for each individual people waiting for the lift
    for(size_t i=0;i<waitingList.size();i++)
    {
        if(!waitingList[i].pi->isInLift)
        {
            int currentFloor = waitingList[i].pi->currentFloor;
            int travellingTo = waitingList[i].pi->travellingTo;
            int callDirection = waitingList[i].pi->directionHeading;

            /* UP */
            if(callDirection == +1)
            {
                // check travellingTo matches lift range
                for(size_t k=0;k<liftSector.size();k++)
                {
                    if(travellingTo >= liftSector[k].first && travellingTo <= liftSector[k].second)
                    {
                        waitingList[i].idealLift = k;

                        if(!lh->getLiftIsMoving(k))
                            break;
                    }
                }
            }
            else if(callDirection == -1) /* UP */
            {
                // checks currentfloor matches lift range
                for(int k=0;k<liftSector.size();k++)
                {
                    if(currentFloor >= liftSector[k].first && currentFloor <= liftSector[k].second)
                    {
                        waitingList[i].idealLift = k;

                        if(!lh->getLiftIsMoving(k))
                            break;
                    }
                }
            }
        }
    }
}

/* Get summary of simulation */
QString Algorithm::getSummary(vector<PassengerInfo> passengerList)
{
    QString summary;

    /* Create graph counter */
    vector<int> graph;
    graph.resize(24);
    fill(graph.begin(), graph.end(), 0);

    /* Get number of pressed for each hour */
    summary += "<b>Time travel graph:</b><br>";
    for(size_t i=0;i<passengerList.size();i++)
    {
        int pressTime = passengerList[i].liftPressTime;
        if(pressTime >= 0 && pressTime <= 3600)
            graph[0] += 1;
        else if(pressTime >= 3601 && pressTime <= 7200)
            graph[1] += 1;
        else if(pressTime >= 7201 && pressTime <= 10800)
            graph[2] += 1;
        else if(pressTime >= 10801 && pressTime <= 14400)
            graph[3] += 1;
        else if(pressTime >= 14401 && pressTime <= 18000)
            graph[4] += 1;
        else if(pressTime >= 18001 && pressTime <= 21600)
            graph[5] += 1;
        else if(pressTime >= 21601 && pressTime <= 25200)
            graph[6] += 1;
        else if(pressTime >= 25201 && pressTime <= 28800)
            graph[7] += 1;
        else if(pressTime >= 28801 && pressTime <= 32400)
            graph[8] += 1;
        else if(pressTime >= 32401 && pressTime <= 36000)
            graph[9] += 1;
        else if(pressTime >= 36001 && pressTime <= 39600)
            graph[10] += 1;
        else if(pressTime >= 39601 && pressTime <= 43200)
            graph[11] += 1;
        else if(pressTime >= 43201 && pressTime <= 46800)
            graph[12] += 1;
        else if(pressTime >= 46801 && pressTime <= 50400)
            graph[13] += 1;
        else if(pressTime >= 50401 && pressTime <= 54000)
            graph[14] += 1;
        else if(pressTime >= 54001 && pressTime <= 57600)
            graph[15] += 1;
        else if(pressTime >= 57601 && pressTime <= 61200)
            graph[16] += 1;
        else if(pressTime >= 61201 && pressTime <= 64800)
            graph[17] += 1;
        else if(pressTime >= 64801 && pressTime <= 68400)
            graph[18] += 1;
        else if(pressTime >= 68401 && pressTime <= 72000)
            graph[19] += 1;
        else if(pressTime >= 72001 && pressTime <= 75600)
            graph[20] += 1;
        else if(pressTime >= 75601 && pressTime <= 79200)
            graph[21] += 1;
        else if(pressTime >= 79201 && pressTime <= 82800)
            graph[22] += 1;
        else if(pressTime >= 82801 && pressTime <= 86400)
            graph[23] += 1;
    }

    /* Draw graph */
    for(size_t j=0; j<graph.size(); j++)
    {
        int number = graph[j]%20;
        if(number != 0)
        {
            if(j>12)
                summary += QString::number(j-12);
            else
                summary += QString::number(j);

            if(j<=11)
                summary += "am: ";
            else
                summary += "pm: ";

            for(int i=0;i<number; i++)
                summary += "*";

            summary += "<br>";
        }
    }

    /* Get travel summary */
    summary += "<br><b>Travel data:</b><br>";
    for(size_t i=0;i<passengerList.size();i++)
    {
        bool isResident = passengerList[i].isResident;
        if(isResident)
            summary += "Resident " + QString::number(passengerList[i].personNo+1);
        else
            summary += "Visitor " + QString::number(passengerList[i].personNo+1);

        summary += " to floor ";
        summary += QString::number(passengerList[i].travellingTo);
        summary += ":<br>";
        summary += "Time pressed lift: ";
        summary += QDateTime::fromTime_t(passengerList[i].liftPressTime).toUTC().toString("h:mm:ss ap");

        summary += "<br>Time board lift: ";
        if(passengerList[i].liftBoardTime == 0)
            summary += "-";
        else
            summary += QDateTime::fromTime_t(passengerList[i].liftBoardTime).toUTC().toString("h:mm:ss ap");

        summary += "<br>Time exit lift: ";
        if(passengerList[i].liftExitTime == 0)
            summary += "-";
        else
            summary += QDateTime::fromTime_t(passengerList[i].liftExitTime).toUTC().toString("h:mm:ss ap");

        summary += "<br><br>";
    }

    /* empty graph */
    graph.clear();

    return summary;
}

/* Get elapsed time and average waiting time */
pair<pair<double, vector<int> >, pair<int, int> > Algorithm::getTiming(double elapsedTime,
                                                   vector<PassengerInfo> passengerList,
                                                   vector<int> liftIdleTime)
{
    int averageWaitTime = 0;
    int averageTravelTime = 0;
    int countA = 0;
    int countB = 0;

    /* Calculate for all individual passengers */
    for(size_t i=0;i<passengerList.size();i++)
    {
        if(passengerList[i].liftBoardTime != 0 && passengerList[i].liftPressTime != 0)
        {
            averageWaitTime += (passengerList[i].liftBoardTime - passengerList[i].liftPressTime);
            countA++;
        }
        if(passengerList[i].liftBoardTime != 0 && passengerList[i].liftExitTime != 0)
        {
            averageTravelTime += (passengerList[i].liftExitTime - passengerList[i].liftBoardTime);
            countB++;
        }
    }

    /* Calculate average */
    averageWaitTime /= countA;
    averageTravelTime /= countB;

    /* Result */
    pair<int, int> averageTime;
    averageTime = make_pair(averageWaitTime, averageTravelTime);

    pair<double, vector<int> > elapsedLiftIdle;
    elapsedLiftIdle = make_pair(elapsedTime, liftIdleTime);

    pair<pair<double, vector<int> >, pair<int, int> > result;
    result = make_pair(elapsedLiftIdle, averageTime);

    return result;
}

/* Get priority floor */
int Algorithm::getFloorWithHighestPriority(map<int, int> travelPath, int currentFloor, int direction)
{
    int floorToGo = 1;
    int highestTimedPriority = 0;
    map<int,int>::iterator it = travelPath.begin();

    for(;it != travelPath.end();it++)
    {
        if(direction == -1)
        {
            if(it->first < currentFloor)
            {
                // Selecting priority floor
                if (it->second > highestTimedPriority)
                {
                    highestTimedPriority = it->second;
                    floorToGo = it->first;
                }
            }
        }
        else
        {
            if(it->first > currentFloor)
            {
                // Selecting priority floor
                if (it->second > highestTimedPriority)
                {
                    highestTimedPriority = it->second;
                    floorToGo = it->first;
                }
            }
        }
    }


    return floorToGo;
}
