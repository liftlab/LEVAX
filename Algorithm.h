#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "BuildingHandler.h"
#include "LiftHandler.h"
#include "SimulatedHumanHandler.h"
#include <QDateTime>

class Algorithm
{
public:
    Algorithm();                                                                        /* Constructor */
    ~Algorithm();                                                                           /* Destructor */

    double nearestCar(BuildingHandler *,
                      LiftHandler *, SimulatedHumanHandler *);                                  /* Nearest Car algorithm */
    double fixedSectoringCommonSectorSystem(BuildingHandler *,
                                            LiftHandler *, SimulatedHumanHandler *);            /* Fixed Sectoring Common Sector System algorithm */
    double fixedSectoringPriorityTimedSystem(BuildingHandler *,
                                             LiftHandler *, SimulatedHumanHandler *);           /* Fixed Sectoring Priority Timed System algorithm */
    double dynamicSectoringSystem(BuildingHandler *,
                                  LiftHandler *, SimulatedHumanHandler *);                      /* Dynamic Sectoring System algorithm */

    double convertToSeconds(qint64);                                                            /* Convert time to seconds */

    multimap<int,pair<bool, pair<int, int> > > getDataSet(SimulatedHumanHandler*);   /* Get data set for simulation */

private:

    int noOfLifts;
    int noOfFloors;
    int noOfResidents;
    int noOfVisitors;
};

#endif // ALGORITHM_H
