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
double Algorithm::nearestCar(BuildingHandler *bh, LiftHandler *lh, SimulatedHumanHandler *shh)
{
    /* Start timer */
    QElapsedTimer timer;
    timer.start();

    noOfLifts = lh->getNumberOfLiftsObject();
    noOfFloors = bh->getNoOfFloor();
    noOfResidents = shh->getNumberOfSimulatedHumanObject();
    noOfVisitors = shh->getNumberOfVisitorObj();

    passengersInLift.resize(noOfLifts);

    vector<PassengerInfo> passengerList = processPassenger(shh);
    vector<WaitingStatus> waitingList;

    vector<map<int, int> > travelPath;
    travelPath.resize(noOfLifts);

    // A day consist of 86400 seconds
    // 0 = 00:00:00
    // 86399 = 23:59:59
    int seconds = 0;
    while(seconds < 86399)
    {
        // Search for passenger that matches seconds, push them into waitingList
        vector<PassengerInfo>::iterator itr = find(passengerList.begin(),passengerList.end(), seconds);
        if(itr != passengerList.end())
        {
            int totalFound = count(passengerList.begin(), passengerList.end(), seconds);
            qDebug() << "\n" << totalFound << "passengers found pressing lift on" << seconds;
            for(int i=0;i<totalFound;i++)
            {
                WaitingStatus ws;
                ws.pi = *itr;
                for(int i=0;i<noOfLifts;i++)
                    ws.FS.push_back(0);
                ws.idealLift = 0;   //default ideal lift is 0
                waitingList.push_back(ws);

                itr++;
            }
        }

        // Calculate their respective FS if they are not empty
        if(!waitingList.empty())
        {
            qDebug() << "\nComputing FS";
            // Compute FS for each person and the respective lift and choose an ideal lift
            computeFS(waitingList, lh);

            qDebug() << "Attempting to push passenger in queue";
            // Push passenger in queue
            int queueCounter = 0;
            while(queueCounter < waitingList.size())
            {
                if(!waitingList[queueCounter].pi.isInLift)
                {
                    qDebug() << "Person is not in lift";
                    int liftCounter = 0;
                    while(liftCounter < noOfLifts)
                    {
                        if(waitingList[queueCounter].idealLift == liftCounter)
                        {
                            qDebug() << "Ideal lift found";
                            int distance = abs(lh->getLiftCurrentFloor(liftCounter) - waitingList[queueCounter].pi.currentFloor) * bh->getMetrePerFloor();

                            if(lh->getLiftIsMoving(liftCounter))
                            {
                                // if lift is travelling to fetch, do nothing
                                if(lh->getLiftTravellingTo(liftCounter) == waitingList[queueCounter].pi.currentFloor)
                                {
                                    qDebug() << "Lift is already travelling to call!";
                                }
                                else
                                {
                                    // if within lift target and lift current and moving, intercept.
                                    distance -= lh->getLiftTotalDistance(liftCounter);

                                    if(distance <= lh->getLiftDistanceLeft(liftCounter) && distance >= 0)
                                    {
                                        int personDirection;
                                        if(waitingList[queueCounter].pi.travellingTo < waitingList[queueCounter].pi.currentFloor)
                                            personDirection = -1;
                                        else if(waitingList[queueCounter].pi.travellingTo > waitingList[queueCounter].pi.currentFloor)
                                            personDirection = +1;

                                        // if going up
                                        if(lh->getLiftDirection(liftCounter) == +1 && personDirection == lh->getLiftDirection(liftCounter))
                                        {
                                            qDebug() << "intercept up";
                                            // push and sort ascending
                                        }
                                        else if(lh->getLiftDirection(liftCounter) == -1 && personDirection == lh->getLiftDirection(liftCounter))
                                        {
                                            // find travelPath and delete
                                            //std::map<int,int>::iterator it;
                                            //it = travelPath[liftCounter].find(waitingList[queueCounter].pi.currentFloor);

                                            qDebug() << "intercept down";

                                            // Calculate distance
                                            lh->setLiftDistanceLeft(liftCounter, distance);
                                            lh->setLiftTravellingTo(liftCounter, waitingList[queueCounter].pi.currentFloor);

                                        }
                                        else
                                        {
                                            qDebug() << "Unable to intercept this lift as different it is heading to different direction";
                                        }
                                    }
                                }
                            }
                            else
                            {
                                // if lift is at the same floor as the person
                                if(lh->getLiftCurrentFloor(liftCounter) == waitingList[queueCounter].pi.currentFloor)
                                {
                                    /* Remove from travelPath */
                                    std::map<int,int>::iterator searchPathIter;
                                    searchPathIter = travelPath[liftCounter].find(waitingList[queueCounter].pi.currentFloor);
                                    if(searchPathIter != travelPath[liftCounter].end())
                                        travelPath[liftCounter].erase (searchPathIter);

                                    // Calculate distance
                                    distance = abs(lh->getLiftCurrentFloor(liftCounter) - waitingList[queueCounter].pi.travellingTo) * bh->getMetrePerFloor();
                                    lh->setLiftDistanceLeft(liftCounter, distance);
                                    lh->setLiftTravellingTo(liftCounter, waitingList[queueCounter].pi.travellingTo);

                                    // Push passenger into lift
                                    waitingList[queueCounter].pi.isInLift = true;
                                    passengersInLift[liftCounter].push_back(waitingList[queueCounter].pi);

                                    // push to travelPath
                                    travelPath[liftCounter].insert({waitingList[queueCounter].pi.travellingTo, waitingList[queueCounter].pi.travellingTo});

                                    if(waitingList[queueCounter].pi.travellingTo < lh->getLiftCurrentFloor(liftCounter))
                                        lh->setLiftDirection(liftCounter, -1);
                                    else
                                        lh->setLiftDirection(liftCounter, +1);

                                    // lift to start move next second
                                    lh->setLiftMoveNextRound(liftCounter, true);

                                    qDebug() << "Lift is on the same floor as caller, push in waitingList";
                                    qDebug() << "Pushed successfully";
                                    qDebug() << "Time person board:" << QString::number(seconds);

                                }
                                else
                                {
                                    // Move lift if lift is not picking any passenger
                                    if(!lh->getLiftPickPassenger(liftCounter))
                                    {
                                        qDebug() << "Moving lift" << liftCounter;

                                        // Calculate distance
                                        distance = abs(lh->getLiftCurrentFloor(liftCounter) - waitingList[queueCounter].pi.currentFloor) * bh->getMetrePerFloor();
                                        lh->setLiftDistanceLeft(liftCounter, distance);
                                        lh->setLiftTravellingTo(liftCounter, waitingList[queueCounter].pi.currentFloor);

                                        // push to travelPath
                                        travelPath[liftCounter].insert({waitingList[queueCounter].pi.currentFloor, waitingList[queueCounter].pi.currentFloor});

                                        if(waitingList[queueCounter].pi.currentFloor < lh->getLiftCurrentFloor(liftCounter))
                                            lh->setLiftDirection(liftCounter, -1);
                                        else
                                            lh->setLiftDirection(liftCounter, +1);

                                        // Allows travel to pick passenger
                                        lh->setLiftPickPassenger(liftCounter, true);

                                        // lift to start move next second
                                        lh->setLiftMoveNextRound(liftCounter, true);

                                        qDebug() << "Lift" << liftCounter << "moving towards" << lh->getLiftTravellingTo(liftCounter);
                                    }
                                }
                            }
                            break;
                        }
                        liftCounter++;
                    }
                }
                queueCounter++;
            }
        }

        // Update all moving lifts
        for(int i=0;i<noOfLifts;i++)
        {
            // If there are passenger for lift i
            int found = count(waitingList.begin(), waitingList.end(), i);
            if(found <= 0 && passengersInLift[i].size() <= 0 && !lh->getLiftPickPassenger(i) && !lh->getLiftPark(i))
            {
                if(lh->getLiftCurrentFloor(i) != lh->getLiftDefaultFloor(i))
                {
                    // Set direction to travel back to default floor
                    if(lh->getLiftDefaultFloor(i) >= lh->getLiftCurrentFloor(i))
                        lh->setLiftDirection(i, +1);
                    else
                        lh->setLiftDirection(i, -1);

                    //lh->setLiftMoveNextRound(i, false);
                    int dist = abs(lh->getLiftCurrentFloor(i) - lh->getLiftDefaultFloor(i)) * bh->getMetrePerFloor();
                    lh->setLiftDistanceLeft(i, dist);
                    lh->setLiftTravellingTo(i, lh->getLiftDefaultFloor(i));

                    lh->setLiftDistanceCount(i,0);
                    lh->setLiftTotalDistance(i, 0);

                    lh->setLiftIsMoving(i, true);
                    lh->setLiftPark(i, true); // park lift
                    qDebug() << "No people boarding lift" << i << ". Going back default floor" << lh->getLiftDefaultFloor(i);
                }
            }
            if(passengersInLift[i].size() > 0 || lh->getLiftPark(i) || lh->getLiftPickPassenger(i))
            {
                // Set lift moving state
                lh->setLiftIsMoving(i,true);

                // If lift is moving and moveNextRound is false, calculate movements based on speed
                if(lh->getLiftIsMoving(i) && !lh->getLiftMoveNextRound(i))
                {
                    qDebug() << "Lift" << i << "moving... Time now is:" << seconds;
                    int distance = lh->getLiftDistanceLeft(i);
                    int speed = lh->getLiftSpeed(i);
                    int metrePerFloor = bh->getMetrePerFloor();

                    qDebug() << distance << "metre more to reach destination";
                    while(lh->getLiftDistanceCount(i) <= speed && lh->getLiftDistanceLeft(i) >= 0)
                    {
                        //increase distance by 1 per loop
                        lh->increaseDistance(i, 1);
                        lh->setLiftDistanceLeft(i, lh->getLiftDistanceLeft(i)-1);

                        //qDebug() << lh->getLiftDistanceCount(i) << "metre moved";
                        qDebug() << lh->getLiftTotalDistance(i) << "metre moved";

                        // Check if lift has traveled one floor
                        if(lh->getLiftTotalDistance(i)%metrePerFloor == 0)
                        {
                            // Update current floor
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

                            if(lh->getLiftCurrentFloor(i) == bh->getNoOfFloor())
                            {
                                lh->setLiftDirection(i, -1);
                            }
                            qDebug() << "Floor" << lh->getLiftCurrentFloor(i) << "reached";
                        }

                        if(lh->getLiftDistanceLeft(i) <= 0)
                        {
                            qDebug() << "Distance completed, Destination reached";
                            lh->setLiftDistanceCount(i, 0);
                            lh->setLiftTotalDistance(i, 0);
                            lh->setLiftDistanceLeft(i, 0);

                            lh->setLiftIsMoving(i, false);

                            if(lh->getLiftPark(i))
                                lh->setLiftPark(i, false);

                            if(lh->getLiftPickPassenger(i))
                                lh->setLiftPickPassenger(i, false);

                            // clear passengers that reached destination
                            vector<PassengerInfo>::iterator iter = passengersInLift[i].begin();
                            for( ; iter != passengersInLift[i].end(); )
                            {
                                iter->isInLift = false;

                                // remove passengers from waitingList that matches the info (isResident, personNo, travelNo, travellingTo)
                                pair<PassengerInfo, int> searchPair;
                                searchPair = make_pair(*iter, lh->getLiftCurrentFloor(i));

                                // Clear waitingList that matches the searchPair
                                vector<WaitingStatus>::iterator itr = find(waitingList.begin(),waitingList.end(), searchPair);
                                if(itr != waitingList.end())
                                {
                                    int personIndex = itr->pi.personNo;
                                    bool isResident = itr->pi.isResident;

                                    if(isResident)
                                        qDebug() << "Resident" << personIndex << "board on" << itr->pi.travelTime << "travelled on" << itr->pi.travelTime+1 << "removed from waitingList on" << seconds+1;
                                    else
                                        qDebug() << "Visitor" << personIndex << "board on" << itr->pi.travelTime << "travelled on" << itr->pi.travelTime+1 << "removed from waitingList on" << seconds+1;

                                    waitingList.erase(itr);
                                }

                                // remove passengers from passengersInLift that matches the info travellingTo
                                if(iter->travellingTo == lh->getLiftCurrentFloor(i))
                                {
                                    int personIndex = iter->personNo;
                                    bool isResident = iter->isResident;

                                    if(isResident)
                                        qDebug() << "Resident" << personIndex << "board on" << iter->travelTime << "travelled on" << iter->travelTime+1 << "removed from passengersInLift on" << seconds+1;
                                    else
                                        qDebug() << "Visitor" << personIndex << "board on" << iter->travelTime << "travelled on" << iter->travelTime+1 << "removed from passengersInLift on" << seconds+1;

                                    passengersInLift[i].erase(iter);
                                }
                                else
                                {
                                    ++iter;
                                }
                            }

                            // Park lift if idealLift in waitingList does not match lift number
//                            int check = count(waitingList.begin(), waitingList.end(), i);
//                            qDebug() << "Lift" << i << "Check is" << check;
//                            if(check == 0)
//                            {
//                                if(lh->getLiftCurrentFloor(i) != lh->getLiftDefaultFloor(i))
//                                {
//                                    // Set direction to travel back to default floor
//                                    if(lh->getLiftDefaultFloor(i) >= lh->getLiftCurrentFloor(i))
//                                        lh->setLiftDirection(i, +1);
//                                    else
//                                        lh->setLiftDirection(i, -1);

//                                    //lh->setLiftMoveNextRound(i, false);
//                                    int dist = abs(lh->getLiftCurrentFloor(i) - lh->getLiftDefaultFloor(i)) * bh->getMetrePerFloor();
//                                    lh->setLiftDistanceLeft(i, dist);
//                                    lh->setLiftTravellingTo(i, lh->getLiftDefaultFloor(i));

//                                    lh->setLiftDistanceCount(i,0);
//                                    lh->setLiftTotalDistance(i, 0);

//                                    lh->setLiftIsMoving(i, true);
//                                    lh->setLiftPark(i, true); // park lift
//                                    qDebug() << "No people board, going back default floor" << lh->getLiftDefaultFloor(i);// << lh->getLiftTravellingTo(i) << lh->getLiftDistanceLeft(i) << lh->getLiftDirection(i);
//                                }
//                            }
                            break;
                        }

                        if(lh->getLiftDistanceCount(i) >= speed)
                        {
                            qDebug() << "Speed of" << speed << "metre per second reached. Continue in next second(loop)...";
                            //qDebug() <<lh->getLiftDistanceCount(i) << lh->getLiftTotalDistance(i) << lh->getLiftDistanceLeft(i);
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
                lh->setLiftIsMoving(i,false);
            }
        }
        seconds++;

        /* The code below is for debugging purposes
         * enable it to break out of the loop so that
         * the output will not be flooded with text
         */
        if(seconds >= 49999)
            break;
    }

    /* Clear data and reset data */
    passengerList.clear();
    waitingList.clear();

    lh->resetLift(noOfFloors);
    noOfLifts = 0;
    noOfFloors = 0;
    noOfResidents = 0;
    noOfVisitors = 0;

    /* return timer */
    return convertToSeconds(timer.elapsed());
}

/* Fixed Sectoring Common Sector System algorithm */
double Algorithm::fixedSectoringCommonSectorSystem(BuildingHandler *bh, LiftHandler *lh, SimulatedHumanHandler *shh)
{
    QElapsedTimer timer;
    timer.start();

    noOfLifts = lh->getNumberOfLiftsObject();
    noOfFloors = bh->getNoOfFloor();
    noOfResidents = shh->getNumberOfSimulatedHumanObject();
    noOfVisitors = shh->getNumberOfVisitorObj();

    return convertToSeconds(timer.elapsed());
}

/* Fixed Sectoring Priority Timed System algorithm */
double Algorithm::fixedSectoringPriorityTimedSystem(BuildingHandler *bh, LiftHandler *lh, SimulatedHumanHandler *shh)
{
    QElapsedTimer timer;
    timer.start();

    noOfLifts = lh->getNumberOfLiftsObject();
    noOfFloors = bh->getNoOfFloor();
    noOfResidents = shh->getNumberOfSimulatedHumanObject();
    noOfVisitors = shh->getNumberOfVisitorObj();

    return convertToSeconds(timer.elapsed());
}

/* Dynamic Sectoring System algorithm */
double Algorithm::dynamicSectoringSystem(BuildingHandler *bh, LiftHandler *lh, SimulatedHumanHandler *shh)
{
    QElapsedTimer timer;
    timer.start();

    noOfLifts = lh->getNumberOfLiftsObject();
    noOfFloors = bh->getNoOfFloor();
    noOfResidents = shh->getNumberOfSimulatedHumanObject();
    noOfVisitors = shh->getNumberOfVisitorObj();

    return convertToSeconds(timer.elapsed());
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
    int personIndex = 0;
    bool isResident = true;
    // for each individual people waiting for the lift
    for(size_t i=0;i<waitingList.size();i++)
    {
        if(!waitingList[i].pi.isInLift)
        {
            int highestFS = 0;

            //update the FS for each lift
            for(int j=0;j<noOfLifts;j++)
            {
                int FS = 0;

                personIndex = waitingList[i].pi.personNo;
                isResident = waitingList[i].pi.isResident;
                int currentFloor = waitingList[i].pi.currentFloor;
                int callDirection = waitingList[i].pi.directionHeading;

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

                qDebug() << (isResident?"Resident":"Visitor") << personIndex << "- lift" << j << "has FS of" << FS;
            }
            qDebug() << "Lift" << waitingList[i].idealLift << "most ideal for" << (isResident?"Resident":"Visitor") << personIndex;
        }
    }
}
