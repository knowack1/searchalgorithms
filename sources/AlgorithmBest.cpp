#include <boost/foreach.hpp>

#include "AlgorithmBest.h"

void AlgorithmBest::execute(const Node& source, const Node& goal, const OperatorsList& opers)
{
    boost::posix_time::ptime tic =  boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time();
    int numOfExplored = 0;
    m_queue.push(source);

    while(!m_queue.empty())
    {
        Queue::value_type node = m_queue.top();
        m_queue.pop();

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
            m_queue.push(childNode);
        }
    }
    sig_end(Params(false));
    std::cout<<"KONIEC"<<std::endl;
}
