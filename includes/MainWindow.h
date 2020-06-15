#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QImage>
#include <QPoint>

#include "ui_MainWindow.h"
#include "ImageWidget.h"
#include "Operator.h"
#include "AlgorithmDepth.h"
#include "AlgorithmBest.h"
#include "AlgorithmA.h"
#include "AlgorithmDijkstra.h"
#include "Defines.h"

#include <boost/thread.hpp>
#include "boost/foreach.hpp"

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(std::string mapName):
        m_mapName(mapName)

    {
        ui_mainWindow.setupUi(this);
        ui_mainWindow.mdiArea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        QObject::connect(ui_mainWindow.startButton, SIGNAL(released ()),
                             this, SLOT(start()));

        QObject::connect(ui_mainWindow.scaleSpinBox, SIGNAL(valueChanged(int) ),
                                     this, SLOT(scaleChanged(int )));
        glo_scale = ui_mainWindow.scaleSpinBox->value();
//        start();
    }

public slots:

    void start()
    {
        Node source(ui_mainWindow.startRowSpinBox->value(),ui_mainWindow.startColumnSpinBox->value());
        Node goal(ui_mainWindow.stopRowSpinBox->value(),ui_mainWindow.stopColumnSpinBox->value());
//        Node source(50,50);
//        Node goal(200,200);
//        ui_mainWindow.aBox->setChecked(true);
//        glo_scale = 5;
        OperatorsList oper_list = getOperators();
        std::vector<AlgorithmWindow*> windows  = getAlgorithms(source, goal, oper_list);

        BOOST_FOREACH(AlgorithmWindow* window, windows)
        {
            ui_mainWindow.mdiArea->addSubWindow(window);
            window->show();
            window->start();
        }
    }

    void scaleChanged(int value)
    {
        glo_scale = value;
    }

private:

    struct PrepareOperComp
    {
        bool operator()(const std::pair<Operator_ptr, int> &pair1, const std::pair<Operator_ptr, int> &pair2)
        {
            return pair1.second > pair2.second;
        }
    };

    OperatorsList getOperators()
    {
        std::vector<std::pair<Operator_ptr, int> > opersAndPrio;
        if(int value = ui_mainWindow.upSpinBox->value())
        {
           opersAndPrio.push_back(std::make_pair(Operator_ptr(new OperatorUp), value));
        }
        if(int value = ui_mainWindow.downSpinBox->value())
        {
           opersAndPrio.push_back(std::make_pair(Operator_ptr(new OperatorDown), value));
        }
        if(int value = ui_mainWindow.leftSpinBox->value())
        {
           opersAndPrio.push_back(std::make_pair(Operator_ptr(new OperatorLeft), value));
        }
        if(int value = ui_mainWindow.rightSpinBox->value())
        {
           opersAndPrio.push_back(std::make_pair(Operator_ptr(new OperatorRight), value));
        }
        if(int value = ui_mainWindow.upLeftSpinBox->value())
        {
           opersAndPrio.push_back(std::make_pair(Operator_ptr((OperatorUp*)new OperatorUpLeft), value));
        }
        if(int value = ui_mainWindow.downLeftSpinBox->value())
        {
           opersAndPrio.push_back(std::make_pair(Operator_ptr((OperatorDown*)new OperatorDownLeft), value));
        }
        if(int value = ui_mainWindow.uprightSpinBox->value())
        {
           opersAndPrio.push_back(std::make_pair(Operator_ptr((OperatorUp*)new OperatorUpRight), value));
        }
        if(int value = ui_mainWindow.downRightSpinBox->value())
        {
           opersAndPrio.push_back(std::make_pair(Operator_ptr((OperatorDown*)new OperatorDownRight), value));
        }
        std::sort(opersAndPrio.begin(), opersAndPrio.end(), PrepareOperComp());

        OperatorsList opers;
        std::vector<std::pair<Operator_ptr, int> >::iterator it;
        for(it = opersAndPrio.begin(); it != opersAndPrio.end(); ++it)
        {
            opers.push_back((*it).first);
        }
        return opers;
    }

    std::vector<AlgorithmWindow*> getAlgorithms(const Node &source, const Node &goal, const OperatorsList &oper_list)
    {
        std::string mapName(m_mapName);
        std::vector<AlgorithmWindow*> windows;
        if(ui_mainWindow.depthBox->isChecked())
        {
            AlgorithmWindow* depthWindow  = new AlgorithmWindow();
            depthWindow->openImage(mapName.c_str());
            Algorithm_ptr depthAlg(new  AlgorithmDepth(depthWindow->getImage(), source, goal, oper_list));
            depthWindow->setAlgorithm(depthAlg);
            windows.push_back(depthWindow);
        }
        if(ui_mainWindow.bestBox->isChecked())
       {
           AlgorithmWindow* bestWindow  = new AlgorithmWindow();
           bestWindow->openImage(mapName.c_str());
           Algorithm_ptr bestAlg(new  AlgorithmBest(bestWindow->getImage(), source, goal, oper_list));
           bestWindow->setAlgorithm(bestAlg);
           windows.push_back(bestWindow);
       }
        if(ui_mainWindow.aBox->isChecked())
        {
            AlgorithmWindow* aWindow  = new AlgorithmWindow();
            aWindow->openImage(mapName.c_str());
            Algorithm_ptr aAlg(new  AlgorithmA<EuclidesEstimation>(aWindow->getImage(), source, goal, oper_list));
            aWindow->setAlgorithm(aAlg);
            windows.push_back(aWindow);
        }
        if(ui_mainWindow.dijkstraBox->isChecked())
        {
            AlgorithmWindow* dijkstraWindow  = new AlgorithmWindow();
            dijkstraWindow->openImage(mapName.c_str());
            Algorithm_ptr dijkstraAlg(new  AlgorithmDijkstra(dijkstraWindow->getImage(), source, goal, oper_list));
            dijkstraWindow->setAlgorithm(dijkstraAlg);
            windows.push_back(dijkstraWindow);
        }

        return windows;
    }

    Ui::MainWindow ui_mainWindow;
    std::string m_mapName;
};

#endif /* MAINWINDOW_H_ */
