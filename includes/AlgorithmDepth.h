#ifndef ALGORITHMDEPTH_H_
#define ALGORITHMDEPTH_H_

#include "Algorithm.h"
#include <stack>

class AlgorithmDepth: public Algorithm
{
    typedef std::stack<Node> StackType;
public:
    AlgorithmDepth(QImage map, Node source, Node goal, const OperatorsList& opers):
          Algorithm(map, source, goal, opers),
          m_nodes(map.width(),std::vector<Node_ptr>(map.height()))
    {
    }

    AlgorithmDepth()
    {
    }

    virtual void setMap(const QImage& map)
    {
        Algorithm::setMap(map);
        m_nodes = std::vector<std::vector<Node_ptr> >(map.width(),std::vector<Node_ptr>(map.height()));
    }

    virtual void execute()
    {
        execute(m_source, m_goal, m_opers);
    }

    std::string getName()
    {
       return std::string("DepthSearch");
    }

    void execute(const Node& source, const Node& goal, const OperatorsList& opers);

private:
    OperatorsList swapOperators(OperatorsList opers)
    {
        OperatorsList::iterator begin = opers.begin();
        OperatorsList::iterator end = opers.end()-1;
        while(begin < end)
        {
            std::swap(*begin, *end);
            ++begin;
            --end;
        }
        return opers;
    }

    StackType m_stack;
    std::vector<std::vector<Node_ptr> > m_nodes;
};

#endif /* ALGORITHMDEPTH_H_ */

