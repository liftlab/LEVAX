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

    vector<PassengerInfo> passengerList = processPassenger(shh);
    vector<WaitingStatus> waitingList;

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
            for(int i=0;i<totalFound;i++)
            {
                WaitingStatus ws;
                ws.pi = *itr;
                for(int i=0;i<noOfLifts;i++)
                    ws.FS.push_back(make_pair<Lift*,int>(lh->getLiftObj(i),0));
                ws.idealLift = 0;   //default ideal lift is 0
                waitingList.push_back(ws);

                itr++;
            }
        }

        vector<vector<int> > liftTravelPath;

        // Calculate their respective FS if they are not empty
        if(!waitingList.empty())
        {
            // Compute FS for each person and the respective lift and choose an ideal lift
            computeFS(waitingList, lh);


            // Implement lift movement algo here
            // push the ideal path to lifttravelpath
        }

        waitingList.clear();    //Remove this line after implementing algorithm or program will crash(See ***)

        seconds++;
    }

    /* Clear data */
    passengerList.clear();

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
    // for each individual people waiting for the lift
    for(size_t i=0;i<waitingList.size();i++)
    {
        int highestFS = 0;

        //update the FS for each lift
        for(int j=0;j<noOfLifts;j++)
        {
            int FS = 0;

            int personIndex = waitingList[i].pi.personNo;
            bool isResident = waitingList[i].pi.isResident;
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
            waitingList[i].FS[j].second = FS;

            // Selecting most ideal lift
            if (FS > highestFS)
            {
                highestFS = FS;
                waitingList[i].idealLift = j;
            }

            QDebug deb = qDebug();
            if(isResident)
                deb << "Resident";
            else
                deb << "Visitor";

            deb << personIndex << "- lift" << j << "has FS of" << FS;
        }

        qDebug() << "Lift" << waitingList[i].idealLift << "most ideal";
    }
}
