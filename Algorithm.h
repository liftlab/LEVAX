#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "BuildingHandler.h"
#include "LiftHandler.h"
#include "SimulatedHumanHandler.h"
#include <QDateTime>

class Algorithm
{
    struct PassengerInfo {
        int travelTime;

        int isResident;
        int personNo;
        int travelNo;
        int currentFloor;
        int travellingTo;
        int directionHeading;
        bool isInLift;

        bool operator<(const PassengerInfo& a) const { return travelTime < a.travelTime; }
        bool operator==(const int& m) const { return (m == travelTime); }
    };

    struct WaitingStatus {
        PassengerInfo pi;
        vector<pair<Lift*, int> > FS;

        int idealLift;
    };

public:
    Algorithm();                /* Constructor */
    ~Algorithm();               /* Destructor */

    double nearestCar(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);                           /* Nearest Car algorithm */
    double fixedSectoringCommonSectorSystem(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);     /* Fixed Sectoring Common Sector System algorithm */
    double fixedSectoringPriorityTimedSystem(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);    /* Fixed Sectoring Priority Timed System algorithm */
    double dynamicSectoringSystem(BuildingHandler *, LiftHandler *, SimulatedHumanHandler *);               /* Dynamic Sectoring System algorithm */

    double convertToSeconds(qint64);                                        /* Convert time to seconds */
    vector<PassengerInfo> processPassenger(SimulatedHumanHandler*);         /* Push all passenger into passengerList and sort by timing they are travelling */
    void computeFS(vector<WaitingStatus>&, LiftHandler*);                   /* Compute FS for individual passenger in the waitingList (parse by reference) */

private:
    int noOfLifts;
    int noOfFloors;
    int noOfResidents;
    int noOfVisitors;

};
#endif // ALGORITHM_H
