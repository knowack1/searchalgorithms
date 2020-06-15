#include "AlgorithmDepth.h"
#include "boost/foreach.hpp"

void AlgorithmDepth::execute(const Node& source, const Node& goal, const OperatorsList& opers)
{
    OperatorsList m_opers= swapOperators(opers);
    m_nodes[source.column][source.row] = Node_ptr( new Node(source));
    boost::posix_time::ptime tic =  boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time();

    int numOfExplored = 0;
    m_stack.push(source);

    while(!m_stack.empty())
    {
        StackType::value_type node = m_stack.top();
        m_stack.pop();

        if(isExplored(node))
            continue;

        markAsExplored(node);

        sig_explored(node);
        numOfExplored++;
        boost::this_thread::interruption_point();

//        std::cout<<"Node: "<<node<<std::endl;
        double bestWeight = std::numeric_limits<double>::max();
        BOOST_FOREACH(const Operator_ptr& oper,  m_opers)
        {
            Node childNode = (*oper)(node);
            if (isInvalid(childNode) || getWeight(childNode) == wall )
            {
                continue;
            }
            if(isExplored(childNode)) //then childnode really is node parent
            {
                Node_ptr parent = m_nodes[childNode.column][childNode.row];
                if(parent->traceWeight+getWeight(node)*(oper->weightScaler()) < bestWeight)
                {
                    bestWeight = parent->traceWeight+getWeight(node)*(oper->weightScaler());
                    node.traceWeight= bestWeight;
                    node.parent = parent;
                }
                continue;
            }
            childNode.traceWeight = node.traceWeight + getWeight(childNode)*(oper->weightScaler());
            childNode.parent.reset(new Node(node));
            m_nodes[node.column][node.row] = childNode.parent;
            m_stack.push(childNode);
        }
        if( node == goal)
        {
            boost::posix_time::ptime toc =  boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time();
            Params result(true, node, numOfExplored, boost::posix_time::to_simple_string(toc-tic));
            sig_end(result);
            return;
        }
    }

    sig_end(Params(false));
    std::cout<<"KONIEC"<<std::endl;
}
