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

    pair<QString, pair<double, int> > nearestCar(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);                           /* Nearest Car algorithm */
    double fixedSectoringCommonSectorSystem(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);     /* Fixed Sectoring Common Sector System algorithm */
    double fixedSectoringPriorityTimedSystem(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);    /* Fixed Sectoring Priority Timed System algorithm */
    double dynamicSectoringSystem(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);               /* Dynamic Sectoring System algorithm */

    double convertToSeconds(qint64);                                        /* Convert time to seconds */
    vector<PassengerInfo> processPassenger(SimulatedHumanHandler*);         /* Push all passenger into passengerList and sort by timing they are travelling */
    void computeFS(vector<WaitingStatus>&, LiftHandler*);                   /* Compute FS for individual passenger in the waitingList (parse by reference) */

    QString getSummary(vector<PassengerInfo>);                              /* Get summary of simulation */
    pair<double, int> getTiming(double, vector<PassengerInfo>);             /* Get elapsed time and average waiting time */

private:
    int noOfLifts;
    int noOfFloors;
    int noOfResidents;
    int noOfVisitors;

    vector<vector<PassengerInfo> > passengersInLift;
};
#endif // ALGORITHM_H
