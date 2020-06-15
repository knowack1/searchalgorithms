#ifndef DIJKSTRAALGORITHM_H_
#define DIJKSTRAALGORITHM_H_

#include "Algorithm.h"
#include <boost/foreach.hpp>

struct DijkstraNodeComparator
{
    DijkstraNodeComparator(std::vector<std::vector<std::pair<double, Node_ptr> > > &dist):m_dist(dist){}

    bool operator()(const Node& node1, const Node& node2)
    {
        return m_dist[node1.row][node1.column].first > m_dist[node2.row][node2.column].first;
    }
private:
    std::vector<std::vector<std::pair<double, Node_ptr> > > &m_dist;
};

struct NodeEqualPredicat
{
    NodeEqualPredicat( Node_ptr node):m_node(node){}
    bool operator()(const Node_ptr& node)
    {
        return (node->row == m_node->row) && (node->column == m_node->column);
    }

private:
    Node_ptr    m_node;
};

class AlgorithmDijkstra: public Algorithm
{
    typedef std::list<Node> QType;
public:
    AlgorithmDijkstra(QImage map, Node source, Node goal, const OperatorsList& opers):
        Algorithm(map, source, goal, opers)
    {
    }

    AlgorithmDijkstra()
    {
    }

    virtual void execute()
    {
       execute(m_source, m_goal, m_opers);
    }

    std::string getName()
    {
        return std::string("Dijkstra");
    }

    void execute(const Node& source, const Node& goal, const OperatorsList& opers)
    {
        m_distPrev = std::vector<std::vector<std::pair<double ,Node_ptr> > >(m_map.height(),std::vector<std::pair<double, Node_ptr> >
            (m_map.width(), std::make_pair(std::numeric_limits<double>::max(), Node_ptr())));

        for(int r = 0; r < m_map.height(); r += glo_scale)
        {
            for(int c = 0 ; c < m_map.width(); c += glo_scale)
            {
                m_q.push_back( Node(r,c));
            }
        }

        boost::posix_time::ptime tic =  boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time();
        m_distPrev[source.row][source.column].first = 0;
        m_distPrev[source.row][source.column].second = Node_ptr();

        int numOfExplored = 0;
        //
        while(!m_q.empty())
        {
            Node smallest = extracetSmallest(m_q);
            sig_explored(smallest);
            numOfExplored++;
            boost::this_thread::interruption_point();

            if(m_distPrev[smallest.row][smallest.column].first == std::numeric_limits<double>::max())
            {
                break;
            }

            if( smallest == goal)
            {
                boost::posix_time::ptime toc =  boost::date_time::microsec_clock<boost::posix_time::ptime>::universal_time();
               smallest.parent =  m_distPrev[smallest.row][smallest.column].second;
               Node_ptr parent = smallest.parent;
               while(parent)
               {
                   parent->parent = m_distPrev[parent->row][parent->column].second;
                   parent = parent->parent;
               }
               Params result(true, smallest, numOfExplored, boost::posix_time::to_simple_string(toc-tic));
               sig_end(result);
               return;
            }
            BOOST_FOREACH(const Operator_ptr& oper,  opers)
            {
                Node childNode = (*oper)(smallest);

               if (isInvalid(childNode) || getWeight(childNode) == wall )
               {
                   continue;
               }

               double distance = getWeight(childNode)*(oper->weightScaler()) + m_distPrev[smallest.row][smallest.column].first;
               if(distance < m_distPrev[childNode.row][childNode.column].first)
               {
                   m_distPrev[childNode.row][childNode.column].first = distance;
                   m_distPrev[childNode.row][childNode.column].second = Node_ptr(new Node(smallest));
               }
            }
        }
        sig_end(Params(false));
    }
private:

    Node extracetSmallest(QType& q)
    {
        Node currentSmallest(-1,-1, std::numeric_limits<double>::max());
        int i = 0;
        int j = -1;
        QType::iterator it = q.begin();
        QType::iterator jt;
        BOOST_FOREACH(const Node& node, q)
        {
            if ( m_distPrev[node.row][node.column].first < currentSmallest.traceWeight)
            {
                currentSmallest = node;
                currentSmallest.traceWeight = m_distPrev[node.row][node.column].first;
                jt = it;
            }
            ++it;
        }
        q.erase(jt);
        return currentSmallest;
    }

    std::vector<std::vector<std::pair<double, Node_ptr> > > m_distPrev;
    QType m_q;
};

#endif /* DIJKSTRAALGORITHM_H_ */
