#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "BuildingHandler.h"
#include "LiftHandler.h"
#include "SimulatedHumanHandler.h"

class Algorithm
{
public:
    Algorithm();                                        /* Constructor */
    ~Algorithm();                                       /* Destructor */

    Algorithm(BuildingHandler bh, LiftHandler lh, SimulatedHumanHandler shh);      /* Constructor taking 3 parameters as arguments */

    double nearestCar();                                /* Nearest Car algorithm */
    double fixedSectoringCommonSectorSystem();          /* Fixed Sectoring Common Sector System algorithm */
    double fixedSectoringPriorityTimedSystem();         /* Fixed Sectoring Priority Timed System algorithm */
    double dynamicSectoringSystem();                    /* Dynamic Sectoring System algorithm */

private:
    BuildingHandler bh;                                 /* Building handler object */
    LiftHandler lh;                                     /* Lift handler object */
    SimulatedHumanHandler shh;                          /* SimulatedHuman handler object */

};

#endif // ALGORITHM_H
