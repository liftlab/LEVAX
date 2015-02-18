#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "BuildingHandler.h"
#include "LiftHandler.h"
#include "SimulatedHumanHandler.h"
#include <QDateTime>
#include <cmath>

class Algorithm
{
    struct PassengerInfo {
        int travelTime;

        bool isResident;
        int personNo;
        int travelNo;
        int currentFloor;
        int travellingTo;
        int directionHeading;
        bool isInLift;

        int liftPressTime;
        int liftBoardTime;
        int liftExitTime;

        /* Functor to assist sorting in ascending order */
        bool operator<(const PassengerInfo& a) const { return travelTime < a.travelTime; }

        /* Functor to find if m matched travelTime */
        bool operator==(const int& m) const { return (m == travelTime); }

        /* Functor to find if PassengerInfo matched */
        bool operator==(const PassengerInfo& a) const
        {
            if(a.isResident == isResident && a.personNo == personNo && a.travelNo == travelNo)
                return true;
            return false;
        }

        /* Functor to find if passenger matches the one in the queues*/
        bool operator==(const pair<PassengerInfo, int>& a) const
        {
            if(a.first.isResident == isResident && a.first.personNo == personNo && a.first.travelNo == travelNo && a.second == travellingTo)
               return true;
            return false;
        }
    };

    struct WaitingStatus {
        PassengerInfo *pi;
        vector<int> FS;

        int idealLift;

        /* Functor to find if passenger matches the one in the queues */
        bool operator==(const pair<PassengerInfo, int>& a) const
        {
            if(*pi == a.first && pi->travellingTo == a.second)
                return true;
            return false;
        }

        /* Functor to find if idealLift and liftID is a match */
        bool operator==(const int& m) const { return (m == idealLift); }
    };

public:
    Algorithm();                /* Constructor */
    ~Algorithm();               /* Destructor */

    /* Nearest Car algorithm */
    pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > nearestCar(BuildingHandler *,
                                                                                 LiftHandler *,
                                                                                 SimulatedHumanHandler *);
    /* Fixed Sectoring Common Sector System algorithm */
    pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > fixedSectoringCommonSectorSystem(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);

    /* Fixed Sectoring Priority Timed System algorithm */
    pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > fixedSectoringPriorityTimedSystem(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);

    /* Dynamic Sectoring System algorithm */
    pair<QString, pair<pair<double, vector<int> >, pair<int, int> > > dynamicSectoringSystem(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);

    /* Convert time to seconds */
    double convertToSeconds(qint64);

    /* Push all passenger into passengerList and sort by timing they are travelling */
    vector<PassengerInfo> processPassenger(SimulatedHumanHandler*);

    /* Compute FS for individual passenger in the waitingList (parse by reference) */
    void computeFS(vector<WaitingStatus>&, LiftHandler*);

    /* Get lift by sector */
    void getLiftBySector(vector<WaitingStatus>&);

    /* Split lift by sector */
    void splitLiftBySector();

    /* Compute lift sector dynamically */
    void computeDynamicSector(vector<WaitingStatus>&, LiftHandler*);

    /* Get summary of simulation */
    QString getSummary(vector<PassengerInfo>);

    /* Get elapsed time and average waiting time */
    pair<pair<double, vector<int> >, pair<int, int> > getTiming(double, vector<PassengerInfo>, vector<int>);

    /* Get priority floor */
    int getFloorWithHighestPriority(map<int, int>, int, int);

private:
    int noOfLifts;
    int noOfFloors;
    int noOfResidents;
    int noOfVisitors;

    vector<pair<int, int> > liftSector;
};
#endif // ALGORITHM_H
