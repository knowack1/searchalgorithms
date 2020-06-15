#ifndef NODE_H_
#define NODE_H_

#include "Defines.h"

struct Node
{
    Node(int row_, int column_, double traceWeight_ = 0):
        row(row_),
        column(column_),
        traceWeight(traceWeight_)
    {}

    bool operator==(const Node& rhp) const
    {
        return (rhp.row == row && rhp.column == column);
    }

    int row;
    int column;

    Node_ptr parent;
    double traceWeight;
};

struct InputData
{
    std::string mapName;
    std::vector<std::pair<Node, Node> > soucesGoales;
    std::vector<int> scales;
};

std::ostream& operator<<(std::ostream& out, const InputData& inputData);
std::ostream& operator<<(std::ostream& out, const Node& node);
//template<class T>
//std::ostream& operator<<(std::ostream& out, const boost::shared_ptr<T>& ptr);
template<class T>
std::ostream& operator<<(std::ostream& out, const boost::shared_ptr<T>& ptr)
{
    if(ptr)
        out<<*ptr;
    return out;
}

#endif /* NODE_H_ */
