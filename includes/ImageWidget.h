#ifndef IMAGEWIDGET_H_
#define IMAGEWIDGET_H_

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QPainter>

#include <boost/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <vector>

#include "Defines.h"
#include "Algorithm.h"

 class AlgorithmWindow : public QWidget
 {
     Q_OBJECT

 public:
     AlgorithmWindow(bool drawDirectly = false);
     ~AlgorithmWindow();

     bool openImage(const QString &fileName);
     bool saveImage(const QString &fileName);
     void setPenColor(const QColor &newColor);
     void setPenWidth(int newWidth)
     {
         myPenWidth = newWidth;
     }

     void setAlgorithm(Algorithm_ptr alg)
     {
         m_alg = alg;
         Node source = m_alg->getSource();
         Node goal = m_alg->getGoal();
         setPenColor(Qt::blue);
         drawPointDirectly(QPoint(source.column, source.row));
         drawPointDirectly(QPoint(goal.column, goal.row));
         setPenColor(Qt::green);

         setWindowTitle(m_alg->getName().c_str());
         m_conExpored = m_alg->sig_explored.connect(boost::bind(&AlgorithmWindow::nodeExploredHandler,this, _1));
         m_conEnd = m_alg->sig_end.connect(boost::bind(&AlgorithmWindow::alggorithmEndHandler,this, _1));
     }

     void start()
     {
         m_thread = boost::thread(boost::bind(&Algorithm::execute, m_alg) );
     }

     void iterrupt()
     {
         m_thread.interrupt();
         m_thread.join();
         m_conExpored.disconnect();
         m_conEnd.disconnect();
     }

     void join()
     {
         m_thread.join();
         m_conExpored.disconnect();
         m_conEnd.disconnect();
     }

     void restoreDefaultPen();
     QColor penColor() const { return myPenColor; }
     int penWidth() const { return myPenWidth; }
     const QImage& getImage()
     {
         return image;
     }

     void drawPoint(const QPoint &point);
     void drawPointDirectly(const QPoint &point);

 protected:
     void paintEvent(QPaintEvent *event);
     void timerEvent(QTimerEvent* event);
     void resizeEvent(QResizeEvent *event);

 private:
     void nodeExploredHandler(const Node& node )
     {
         if( node == m_alg->getSource() || node == m_alg->getGoal())//dont draw this...
             return;
         if(drawDirectly)
         {
             drawPointDirectly(QPoint(node.column, node.row));
         }
         else
         {
              drawPoint(QPoint(node.column, node.row));
         }
     }
     void alggorithmEndHandler(Params result)
     {
        Node_ptr parent = result.endNode.parent;
        int numOfNodesInResultPath = 0;

        if(drawDirectly)
        {
            setPenColor(Qt::red);
            while(parent)
            {
                numOfNodesInResultPath++;
                drawPointDirectly(QPoint(parent->column, parent->row));
                parent = parent->parent;
                if(!parent->parent)
                    break;
            }
            result.nodesInPath = numOfNodesInResultPath;
            result.algName = m_alg->getName();

            std::fstream file("results.txt", std::fstream::app |std::fstream::out | std::fstream::in);
            std::string fileName = m_alg->getName();
            file<<"\n"<<m_fileName<<": "<<result<<std::endl;
            fileName = fileName+ m_fileName.substr(0, m_fileName.find(".")) +".jpg";
            saveImage(fileName.c_str());
        }
        else
        {
            while(!m_pointsToDraw.empty()){}
            setPenColor(Qt::red);
            while(parent)
            {
                numOfNodesInResultPath++;
                drawPoint(QPoint(parent->column, parent->row));
                parent = parent->parent;
                if(!parent->parent)
                      break;
            }
            while(!m_pointsToDraw.empty()){}
            result.nodesInPath = numOfNodesInResultPath;
            result.algName = m_alg->getName();

            std::cout<<result<<std::endl;
            std::string fileName = m_alg->getName();
            fileName += ".jpg";
            saveImage(fileName.c_str());
        }
     }

     void resizeImage(QImage *image, const QSize &newSize);

 protected slots:
     void mainLoopIsRunning()
     {
         drawDirectly = false;
     }

 private:
     int myPenWidth;
     QColor myPenColor;
     QImage image;
     QPainter m_painter;
     std::vector<QPoint> m_pointsToDraw;
     int m_timerId;
     boost::mutex m_mutex;
     Algorithm_ptr m_alg;
     boost::signals2::connection m_conExpored;
     boost::signals2::connection m_conEnd;
     boost::thread m_thread;
     bool drawDirectly;
     std::string m_fileName;
 };

#endif /* IMAGEWIDGET_H_ */
