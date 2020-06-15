#ifndef ALGORITHMA_H_
#define ALGORITHMA_H_

#include <queue>
#include <boost/foreach.hpp>

#include "Algorithm.h"

typedef std::pair<Node, double> QueueItem;
struct EuclidesEstimation
{
    double operator()(const Node& source, const Node& goal)
    {
        double drow = source.row - goal.row;
        double dcolumn = source.column - goal.column;

//        std::cout<<"<------IN ESTIMATOR ------->"<<std::endl;
        return (sqrt(pow(drow,2)+ pow(dcolumn,2))/glo_scale*1.5);
        if(drow < 0)
            drow = -drow;
        if(dcolumn < 0)
            dcolumn = -dcolumn;
//        return (drow + dcolumn)/glo_scale*1.5;
    }
};

struct NodeFindPredicat
{
    NodeFindPredicat(Node node): m_node(node){}

    bool operator()(QueueItem item)
    {
        return m_node == item.first;
    }
    Node m_node;
};

struct ANodeComparaor
{
    bool operator()(const QueueItem& pair1, const  QueueItem& pair2)
    {
        return (pair1.second + pair1.first.traceWeight) > (pair2.second + pair2.first.traceWeight);
    }
};

template<class Estimator>
class AlgorithmA: public Algorithm
{
//    typedef std::priority_queue<std::pair<Node, unsigned int>, std::vector<std::pair<Node, unsigned int> >, ANodeComparaor > Queue;
    typedef std::vector<QueueItem> Queue;

public:
    AlgorithmA(QImage map, Node source, Node goal, const OperatorsList& opers):
          Algorithm(map, source, goal, opers),
          m_nodes(map.width(),std::vector<Node_ptr>(map.height()))
    {
    }

    AlgorithmA()
    {
    }


    virtual void execute()
    {
      execute(m_source, m_goal, m_opers);
    }

    std::string getName()
    {
        return std::string("A");
    }

    void execute(const Node& source, const Node& goal, const OperatorsList& opers)
    {
        boost::posix_time::ptime tic =  boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time();
        int numOfExplored = 0;
        m_queue.push_back(std::make_pair(source, m_estimationFunc(source, goal)) );

        while(!m_queue.empty())
        {
            Node node = takeSmalest(m_queue);

            if(isExplored(node))
                continue;

            markAsExplored(node);

            sig_explored(node);
            numOfExplored++;
            boost::this_thread::interruption_point();

            if( node == goal)
            {
                boost::posix_time::ptime toc =  boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time();
                Params result(true, node, numOfExplored, boost::posix_time::to_simple_string(toc-tic));
                sig_end(result);
                return;
            }

            BOOST_FOREACH(const Operator_ptr& oper,  opers)
            {
                Node childNode = (*oper)(node);

                if (isInvalid(childNode) || isExplored(childNode)  || getWeight(childNode) == wall )
                {
                    continue;
                }

                childNode.parent = Node_ptr(new Node(node));
                childNode.traceWeight = node.traceWeight + getWeight(childNode)*(oper->weightScaler());

                Queue::iterator it = std::find_if(m_queue.begin(), m_queue.end(), NodeFindPredicat(childNode));
                if( it != m_queue.end())
                {
                    if(it->first.traceWeight > childNode.traceWeight)
                    {
                        it->first.parent = Node_ptr(new Node(node));
                        it->first.traceWeight = childNode.traceWeight;
                        it->second = m_estimationFunc(it->first, goal);
                    }
                }
                else
                {
                    m_queue.push_back(std::make_pair(childNode, m_estimationFunc(childNode, goal)));
                }
            }
        }
        sig_end(Params(false));
        std::cout<<"KONIEC"<<std::endl;
    }
private:
    Node takeSmalest(Queue& queue)
    {
        if(queue.empty())
            throw 0;
        int i= 0;
        int j =0;
        Queue::value_type pairSmalest = queue[i];
        for(i = 1; i < queue.size(); i++ )
        {
            if((queue[i].first.traceWeight+queue[i].second) < (pairSmalest.first.traceWeight+pairSmalest.second))
            {
                j = i;
                pairSmalest = queue[i];
            }
        }
        queue.erase(queue.begin()+j);
        return pairSmalest.first;
    }
    Queue m_queue;
    Estimator m_estimationFunc;
    std::vector<std::vector<Node_ptr> > m_nodes;
};

#endif /* ALGORITHMA_H_ */
