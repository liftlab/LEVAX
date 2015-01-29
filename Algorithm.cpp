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

    // multimap stores time path for all people, data will be processed linearly.
    // multimap <timeTravel, pair<isResident, pair<index, travelIndex> > >
    std::multimap<int, pair<bool, pair<int, int> > > dataSet = getDataSet(shh);
    std::multimap<int, pair<bool, pair<int, int> > >::iterator itr;

    // lift waiting list
    // <vector<pair<vectorliftID, figureOfSuitability> >, pair<isResident, pair<index, travelIndex> > >
    vector<pair<vector<pair<int, int> >, pair<bool, pair<int, int> > > > waitingList;

    // A day consist of 86400 seconds
    // 0 = 00:00:00
    // 86399 = 23:59:59
    int seconds = 0;
    while(seconds < 86399)
    {
        // check if timing (seconds) exist in dataSet, if exist, add in queue
        itr = dataSet.find(seconds);
        if(itr != dataSet.end())
        {
            int count = dataSet.count(seconds);
            for(int i=0;i<count;i++)
            {
                vector<pair<int, int> > liftInfo;
                for(int j=0;j<noOfLifts;j++)
                {
                    pair<int, int> liftIDFS = make_pair<int, int>(j, 0);
                    liftInfo.push_back(liftIDFS);
                }

                pair<int, int> a = make_pair<int, int>(itr->second.second.first, itr->second.second.second);
                pair<bool, pair<int, int> > b = make_pair<bool, pair<int, int> >(itr->second.first, a);
                waitingList.push_back(make_pair<vector<pair<int, int> >, pair<bool, pair<int, int> > >(liftInfo, b));

                itr++;
            }
        }

        // Update FS only if waitingList is not empty
        if(!waitingList.empty())
        {
            // for each individual people waiting for the lift
            for(int i=0;i<waitingList.size();i++)
            {
                //update the FS for each lift
                for(int j=0;j<noOfLifts;j++)
                {
                    int FS = 0;

                    int travelIndex = waitingList[i].second.second.second;
                    int personIndex = waitingList[i].second.second.first;
                    bool isResident = waitingList[i].second.first;
                    int travelFloor = shh->getTravelFloor(personIndex, travelIndex, isResident);
                    int personCurrentFloor;

                    if(travelFloor != 1)
                        personCurrentFloor = 1;
                    else
                        personCurrentFloor = shh->getResident(personIndex, isResident);

                    int d = personCurrentFloor - lh->getLiftCurrentFloor(j); // 4
                    int direction; // 0
                    int N = noOfFloors-1; // 5

                    if(d / lh->getLiftDirection(j) >= 0)
                    {
                        if(travelFloor != 1)
                            direction = +1;
                        else
                            direction = -1;

                        if(lh->getLiftDirection(j)  == direction)
                        {
                            FS = (N + 2) - abs(d);
                        }
                        else
                        {
                            FS = (N + 1) - abs(d);
                        }
                    }
                    else
                    {
                        FS = 1;
                    }

                    // Store FS
                    waitingList[i].first[j].second = FS;
                }
            }
        }

        // Do algorithm ***
        // Move lift, fetch person, remove from waiting list
        if(!waitingList.empty())
        {
            //...
        }

        waitingList.clear();    //Remove this line after implementing algorithm or program will crash(See ***)


        seconds++;
    }

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

multimap<int,pair<bool, pair<int, int> > > Algorithm::getDataSet(SimulatedHumanHandler *shh)
{
    multimap<int,pair<bool, pair<int, int> > > dataSet;

    for(int i=0; i<shh->getNumberOfSimulatedHumanObject();i++)
    {
        for(int j=0;j<shh->getNoOfTimesTravel(i, true);j++)
        {
            pair<int, int> a = make_pair<int, int>(i, j);
            pair<bool, pair<int, int> > b = make_pair<bool, pair<int, int> >(true, a);
            dataSet.insert(std::multimap<int,pair<bool, pair<int, int> > >::value_type(shh->getTravelTime(i,j,true), b));
        }
    }

    for(int i=0; i<shh->getNumberOfVisitorObj();i++)
    {
        for(int j=0;j<shh->getNoOfTimesTravel(i, false);j++)
        {
            pair<int, int> a = make_pair<int, int>(i, j);
            pair<bool, pair<int, int> > b = make_pair<bool, pair<int, int> >(false, a);
            dataSet.insert(std::multimap<int,pair<bool, pair<int, int> > >::value_type(shh->getTravelTime(i,j,false), b));
        }
    }
    return dataSet;
}
