#include <QImage>
#include <QApplication>
#include "MainWindow.h"
#include <Defines.h>
#include <fstream>

std::vector<InputData> parseConfig(std::string configFile);

int main(int argc, char **argv) {

        QApplication app( argc, argv);
    if( argc !=  2)
    {
        std::cout<<"At least one argument"<<std::endl;
        return 0;
    }

    std::string fileName(argv[1]);

    if(fileName.find(".txt") == std::string::npos)
    {
        MainWindow mainWindow(fileName);
        mainWindow.show();
        return app.exec();
    }
    else
    {
        std::vector<InputData> inputDataVec = parseConfig(std::string(argv[1]));
        BOOST_FOREACH(InputData& inputData, inputDataVec) //for all input data
        {

            BOOST_FOREACH(int scale, inputData.scales) //for all defined scales
            {
                glo_scale = scale;
                typedef std::pair<Node, Node> Pair;
                BOOST_FOREACH(Pair& pair, inputData.soucesGoales )//for all start goal points
                {
                    Node start = pair.first;
                    Node goal = pair.second;
                    AlgorithmsList algorithmsList;
                    algorithmsList.push_back(Algorithm_ptr(new AlgorithmDepth));
                    algorithmsList.push_back(Algorithm_ptr(new AlgorithmBest));
                    algorithmsList.push_back(Algorithm_ptr(new AlgorithmA<EuclidesEstimation>));
                    algorithmsList.push_back(Algorithm_ptr(new AlgorithmDijkstra));

                    OperatorsList operators;
                    operators.push_back(Operator_ptr(new OperatorUp));
                    operators.push_back(Operator_ptr(new OperatorDown));
                    operators.push_back(Operator_ptr(new OperatorLeft));
                    operators.push_back(Operator_ptr(new OperatorRight));
                    operators.push_back(Operator_ptr((OperatorUp*)new OperatorUpLeft));
                    operators.push_back(Operator_ptr((OperatorDown*)new OperatorDownLeft));
                    operators.push_back(Operator_ptr((OperatorUp*)new OperatorUpRight));
                    operators.push_back(Operator_ptr((OperatorDown*)new OperatorDownRight));

                    BOOST_FOREACH(Algorithm_ptr& algPtr, algorithmsList) //make all algorithms
                    {

                        boost::shared_ptr<AlgorithmWindow> algWindow (new AlgorithmWindow(true));
                        if(algWindow->openImage(inputData.mapName.c_str()) == false)
                        {
                           std::cerr<<"Open image "<<inputData.mapName<<" error"<<std::endl;
                           continue;
                        }
                        algWindow->setPenWidth(scale);
                        algPtr->setMap(algWindow->getImage());
                        algPtr->setData(start, goal, operators);
                        algWindow->setAlgorithm(algPtr);
                        algWindow->start();
                        algWindow->join();
                    }
                }
            }
        }
    }
}

std::vector<InputData> parseConfig(std::string configFile)
{
    std::fstream file(configFile.c_str());
    std::string line;

    std::vector<InputData> inputDataVec;
    InputData inputData;

    while(!std::getline(file, line, '\n').eof())
    {
        if(line.find("map") != std::string::npos)
        {
            inputData.mapName = line.substr(line.find(":")+1);
        }
        else if(line.find("sources") != std::string::npos)
        {
            while(!line.empty())
            {
                line = line.substr(line.find(":")+1);
                std::size_t pos = line.find(";");
                std::string src =  line.substr(0, pos);
                line = line.substr(line.find(";")+1);

                Node source(atoi(src.substr(0, src.find(",")).c_str()), atoi(src.substr(src.find(",")+1).c_str()));
                inputData.soucesGoales.push_back(std::make_pair(source, Node(-1, -1)));
            }
        }
        else if(line.find("goals") != std::string::npos)
        {
            int i = 0;
            while(!line.empty())
            {
                line = line.substr(line.find(":")+1);
                std::size_t pos = line.find(";");
                std::string src =  line.substr(0, pos);
                line = line.substr(line.find(";")+1);

                Node goal(atoi(src.substr(0, src.find(",")).c_str()), atoi(src.substr(src.find(",")+1).c_str()));
                inputData.soucesGoales[i++].second = goal;
            }
        }
        else if(line.find("scales") != std::string::npos)
        {
            while(!line.empty())
            {
                line = line.substr(line.find(":")+1);
                std::size_t pos = line.find(";");
                std::string scl = line.substr(0, pos);
                line = line.substr(line.find(";")+1);
                inputData.scales.push_back(atoi(scl.c_str()));
            }
        }
        else if(line.find("#") != std::string::npos)
        {
            inputDataVec.push_back(inputData);
            inputData = InputData();
        }
    }
    std::cout<<inputDataVec<<std::endl;
    return inputDataVec;
}
