#include "Params.h"
#include <boost/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

boost::mutex glo_mutex;

std::ostream& operator<<(std::ostream& out, const Params& rhp)
{
    boost::interprocess::scoped_lock<boost::mutex> lock(glo_mutex);

    if(rhp.goalAchieved == false)
    {
        out<<rhp.algName<<":";
        out<<" GOAL NOT FOUND";
    }
    else
    {
        out<<rhp.algName<<":";
        out<<"\n"<<rhp.numOfExplored;
        out<<"\n"<<rhp.nodesInPath;
        out<<"\n"<<rhp.endNode.traceWeight;
        out<<"\n"<<rhp.time;
    }
    return out;
}
