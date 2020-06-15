#ifndef ALGORITHMBEST_H_
#define ALGORITHMBEST_H_

#include <queue>

#include "Algorithm.h"

class AlgorithmBest: public Algorithm
{
    typedef std::priority_queue<Node, std::vector<Node>, NodeComparator > Queue;

    void printQueue(const Queue& queue)
    {
        Queue tmp = queue;
        while(!tmp.empty())
        {
            std::cout<<"["<<tmp.top()<<", trace: "<<tmp.top().traceWeight<<"] ,";
            tmp.pop();
        }
        std::cout<<std::endl;
    }

public:
    AlgorithmBest(QImage map, Node source, Node goal, const OperatorsList& opers):
        Algorithm(map, source, goal, opers)
    {
    }

    AlgorithmBest()
    {
    }

    virtual void execute()
    {
        execute(m_source, m_goal, m_opers);
    }

    std::string getName()
    {
        return std::string("BestSearch");
    }

    void execute(const Node& source, const Node& goal, const OperatorsList& opers);

private:
    Queue m_queue;
};

#endif /* ALGORITHMBEST_H_ */
