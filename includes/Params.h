#ifndef PARAMS_H_
#define PARAMS_H_

#include "Node.h"

struct Params
{
    Params(bool goalAchieved_, Node endNode_ = Node(-1,-1), int numOfExplored_ = 0, std::string time_ = std::string()):
        goalAchieved(goalAchieved_),
        endNode(endNode_),
        numOfExplored(numOfExplored_),
        nodesInPath(0),
        time(time_)

    {
    }

    bool goalAchieved; /* descrie whether goal was found*/
    Node endNode; /*has backtrace to source and road weigth*/
    int numOfExplored; /*number of explored nodes*/
    int nodesInPath;
    std::string algName;
    std::string time;
};

std::ostream& operator<<(std::ostream& out, const Params& rhp);

#endif /* PARAMS_H_ */
