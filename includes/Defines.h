#ifndef DEFINES_H_
#define DEFINES_H_

#include <vector>
#include <ostream>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <unistd.h>

#define msleep(ms) usleep(ms*1000)
#define INTINF 0xffffffffU

class Operator;
class Node;
class Algorithm;

typedef boost::shared_ptr<Operator> Operator_ptr;
typedef boost::shared_ptr<Node> Node_ptr;
typedef boost::shared_ptr<Algorithm> Algorithm_ptr;

typedef std::vector<Algorithm_ptr > AlgorithmsList;
typedef std::vector<Operator_ptr > OperatorsList;

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec)
{
    BOOST_FOREACH(const T& item, vec)
    {
        out<<" ["<<item<<"]";
    }
    return out;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& pair)
{
    out<<" pair("<<pair.first<<", "<<pair.second<<")";
    return out;
}

extern int glo_scale;

#endif /* DEFINES_H_ */
