#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <boost/signals2.hpp>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time.hpp>

#include <QImage>

#include "Defines.h"
#include "Operator.h"
#include "Node.h"
#include "Params.h"

struct NodeComparator
{
    bool operator()(const Node& node1, const Node& node2)
    {
        return node1.traceWeight > node2.traceWeight;
    }
};

class Algorithm
{
    typedef std::vector<std::vector<bool> > DiscoveringMapType;
public:
    static const double wall = -1;

    Algorithm(QImage map, Node source, Node goal, OperatorsList opers):
        m_dicoveringMap(map.width(),std::vector<bool>(map.height(), false)),
        m_map(map),
        m_source(source),
        m_goal(goal),
        m_opers(opers)
    {
    }

    Algorithm():
        m_source(-1,-1),
        m_goal(-1,-1)
    {
    }

    const Node& getSource()
    {
        return m_source;
    }
    const Node& getGoal()
    {
        return m_goal;
    }

    void setData(Node& source, Node& goal, OperatorsList& opers)
    {
        m_source = source;
        m_goal = goal;
        m_opers = opers;
    }
    virtual void execute()=0;
    virtual void execute(const Node& source, const Node& goal, const OperatorsList& opers)=0;

    virtual std::string getName()=0;

    virtual void setMap(const QImage& map)
    {
        m_map = map;
        m_dicoveringMap = DiscoveringMapType(map.width(),std::vector<bool>(map.height(), false));
//        m_dicoveringMap.resize(map.width());
//        BOOST_FOREACH(DiscoveringMapType::value_type& vec, m_dicoveringMap)
//        {
//            vec.resize(map.height());
//            DiscoveringMapType::value_type::iterator it;
//            for(it = vec.begin(); it != vec.end(); ++it)
//            {
//                (*it) = false;
//            }
//        }
    }

protected:
    bool isInvalid(const Node& node)
    {
        return !m_map.valid(node.column, node.row);
    }

    bool isExplored(const Node& node)
    {
        return m_dicoveringMap[node.column][node.row];
    }

    double getWeight(const Node& node)
    {
        unsigned int weight = (m_map.pixel(node.column, node.row) & 0xFFFFFF);
        if (weight > 0xf00000)
        {
//            std::cout<<std::hex<<"weight1: white"<<std::endl;
            return 1;
        }
        else if(weight < 0xff )
        {
            return wall;
        }
        else
        {
            weight = (weight % 100) + 2 ;
            return (double)weight;
//            std::cout<<"weight3: "<<weight<<std::endl;
        }

        return (double)weight;
    }

    void markAsExplored(const Node& node)
    {
        m_dicoveringMap[node.column][node.row] = true;
    }

private:
    DiscoveringMapType m_dicoveringMap;

protected:
    QImage m_map;
    Node m_source;
    Node m_goal;
    OperatorsList m_opers;

public:
//signals
    boost::signals2::signal<void (const Node&)> sig_explored;
    boost::signals2::signal<void (Params)> sig_end;


};

#endif /* ALGORITHM_H_ */
