#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "BuildingHandler.h"
#include "LiftHandler.h"
#include "SimulatedHumanHandler.h"
#include <QTimer>

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

    double convertToSeconds(qint64);
private:

    int noOfLifts;
    int noOfFloors;
    int noOfResidents;
    int noOfVisitors;
};

#endif // ALGORITHM_H
