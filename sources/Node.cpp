#include "Node.h"

std::ostream& operator<<(std::ostream& out, const InputData& inputData)
{
    out<<std::endl<<" map name="<<inputData.mapName<<std::endl<<" sourcesAndGoals="<<inputData.soucesGoales<<std::endl<<" scales="<<inputData.scales<<std::endl;
//    out<<" map name="<<inputData.mapName<<" sourcesAndGoals="<<inputData.soucesGoales<<" scales="<<inputData.scales;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Node& node)
{
    out<<"row: "<<node.row<<", column: "<<node.column<<", wigth: "<<node.traceWeight;
    return out;
}

//template<class T>
//std::ostream& operator<<(std::ostream& out, const boost::shared_ptr<T>& ptr)
//{
//    out<<*ptr;
//    return out;
//}
