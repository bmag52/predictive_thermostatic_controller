/*
 * DriverPrediction.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: vagrant
 */

#include "../route_prediction/Goal.h"
#include "../route_prediction/Route.h"
#include "../driver_prediction/DriverPrediction.h"
#include "../route_prediction/RoutePrediction.h"
#include "../speed_prediction/SpeedPrediction.h"
#include "../data_management/DataManagement.h"
#include "../city/BuildCity.h"
#include "../city/City.h"
#include "UnitTests.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>

using namespace PredictivePowertrain;

void setRouteNeuralNetworkVals(Route* route, std::vector<float> spds, float spdDist, City* city)
{
    SpeedPrediction sp;
    int trainIters = 10;
    
    std::vector<float> spdsTemp(spds);
    
    // add neural net values to route prediction links
    GenericMap<long int, Link*>* links = route->getLinks();
    
    Eigen::MatrixXd spdIn(1, sp.getI() + 1);
    for(int i = 0; i < spdIn.cols(); i++)
    {
        spdIn.coeffRef(0, i) = 0;
    }
    
    Eigen::MatrixXd spdOut(1, sp.getO());
    Eigen::MatrixXd spdAct(1, sp.getO());
    
    links->initializeCounter();
    GenericEntry<long int, Link*>* nextLink = links->nextEntry();
    while(nextLink != NULL && !nextLink->value->isFinalLink())
    {
        // get necessary road data
        Road* road_i = city->getRoads()->getEntry(nextLink->value->getNumber());
        float roadDist_i = road_i->getSplineLength();
        int roadSpdIndices_i = roadDist_i / spdDist * spds.size();
        std::vector<float> roadSpds_i;
        
        // get road speeds from arbitrary trace of continuous speed values
        for(int i = 0; i < roadSpdIndices_i; i++)
        {
            if(spdsTemp.size() == 0)
            {
                for(int j = 0; j < spds.size(); j++)
                {
                    spdsTemp.push_back(spds.at(j));
                }
            }
            
            roadSpds_i.push_back(spdsTemp.at(0));
            spdsTemp.erase(spdsTemp.begin());
        }
        
        // set actual speed values
        for(int i = 0; i < spdAct.cols(); i++)
        {
            if(i < roadSpds_i.size())
            {
                spdAct.coeffRef(0, i) = roadSpds_i.at(i);
            }
            
            else
            {
                spdAct.coeffRef(0, i) = 0;
            }
        }
        
        // Format input data
        sp.formatInData(&spdIn);
        sp.scaleTrainingSpeed(&spdAct);
        
        
        // make speed prediction
        SpeedPrediction sp;
        for(int i = 0; i < trainIters; i++)
        {
            sp.predict(&spdIn, &spdOut);
            sp.train(&spdOut, &spdAct, &spdIn);
        }
        
        // format output data
        sp.formatOutData(&spdOut);
        sp.unscaleTrainingSpeed(&spdAct);
        
        // update link NN vals
        std::vector<std::vector<Eigen::MatrixXd*>*>* nnVals = sp.getVals();
        nextLink->value->setWeights(nnVals->at(0), nnVals->at(1), nnVals->at(2), nextLink->value->getDirection());
        
        // update speed input from road speed suv i values
        if(spdIn.cols() > roadSpdIndices_i)
        {
            // left shift spdIn
            int deltaLengths = spdIn.cols() - roadSpdIndices_i;
            for(int i = 0; i < deltaLengths; i++)
            {
                spdIn.coeffRef(0, i) = spdIn.coeffRef(0, i+deltaLengths);
            }
            
            // add road spd measurements to speed input
            for(int i = 0; i < roadSpds_i.size(); i++)
            {
                spdIn.coeffRef(0, i+deltaLengths) = roadSpds_i.at(i);
            }
        }
        
        else
        {
            for(int i = spdIn.cols() - 1; i >= 0; i--)
            {
                spdIn.coeffRef(0, i) = roadSpds_i.back();
                roadSpds_i.erase(roadSpds_i.end());
            }
        }
        
        nextLink = links->nextEntry();
    }
    delete(nextLink);
    
    // get distance of speed measurements
    
}

void driverPrediction_ut()
{
    // -----------------------------------
    // BUILD SMALL CITY SECTION IN SPOKANE
    // -----------------------------------
    
    DataManagement dm;
    bool getCityFromDM = true;
    
    City* rpCity;
    
    if(!getCityFromDM)
    {
        // make trip log
        GenericMap<long int, std::pair<double, double>*>* latLon = new GenericMap<long int, std::pair<double, double>*>();
        
        // spokane trip log small
        latLon->addEntry(1, new std::pair<double, double>(47.634, -117.396));
        latLon->addEntry(2, new std::pair<double, double>(47.635, -117.397));
        latLon->addEntry(3, new std::pair<double, double>(47.636, -117.398));
        latLon->addEntry(4, new std::pair<double, double>(47.637, -117.399));
        latLon->addEntry(5, new std::pair<double, double>(47.638, -117.400));
        latLon->addEntry(6, new std::pair<double, double>(47.639, -117.401));
        latLon->addEntry(7, new std::pair<double, double>(47.640, -117.402));
        latLon->addEntry(8, new std::pair<double, double>(47.645, -117.406));
        
        // jsonify trip log -> delete existing jsons
        dm.addTripData(latLon);
        
        BuildCity bc;
        bc.updateGridDataXMLSpline();
        bc.printNewIntersectionsAndRoads();

        GenericMap<long int, Intersection*>* intersections = bc.getNewIntersections();
        GenericMap<long int, Road*>* roads = bc.getNewRoads();
        GenericMap<int, Bounds*> bounds;
        bounds.addEntry(0, bc.getNewBounds());

        rpCity = new City(intersections, roads, &bounds);
        dm.addCityData(rpCity);
    }
    else
    {
        rpCity = dm.getCityData();
    }
    
    // ----------------------
    // SETUP SPEED PREDICTION
    // ----------------------
    
    SpeedPrediction sp;
    
    std::ifstream input("/Users/Brian/Desktop/the_goods/git/predictive_thermo_controller/data/spd.csv");
    std::string num;
    
    std::vector<float> routeSpds;
    for (int i = 0; i<10000; i++){
        std::getline(input, num, ',');
        std::stringstream fs(num);
        float f = 0.0;
        fs >> f;
        routeSpds.push_back(f);
    }
    
    float routeSpdsDist = 0.0;
    
    float prevSpd = routeSpds.at(0);
    for(int i = 1; i < routeSpds.size(); i++)
    {
        float currSpd = routeSpds.at(i);
        float accel_i = (currSpd - prevSpd) / sp.getDT();
        float dist_i = prevSpd + 0.5 * accel_i * std::pow(sp.getDT(),2);
        
        routeSpdsDist += dist_i;
        
        prevSpd = currSpd;
    }
    
    // ----------------------
    // SETUP ROUTE PREDICTION
    // ----------------------
    
    // set route length
    int routeLength = 10;
    
    // set conditions
    std::vector<float>* conditions = new std::vector<float>(2);
    conditions->at(0) = 1;
    conditions->at(1) = 2;
    
    Intersection* startIntersection = rpCity->getIntersections()->getFirstEntry();
    Intersection* endIntersection = rpCity->getIntersections()->getLastEntry();
    Link* firstLink = new Link(1, startIntersection->getRoads()->getFirstEntry()->getRoadID());

    RoutePrediction rp(rpCity);
    
    // generate random actual route
    Route startRoute;
    startRoute.addLink(firstLink);
    startRoute.assignGoal(new Goal(endIntersection->getIntersectionID()));
    Route* actualRoute = rpCity->randomPath(startIntersection, &startRoute, routeLength, conditions);
    
    // add NN vals
    setRouteNeuralNetworkVals(actualRoute, routeSpds, routeSpdsDist, rpCity);
    
    // add trainging iterations here (simulates driving over the route multiple times)
    rp.parseRoute(actualRoute);
    rp.parseRoute(actualRoute);
    rp.parseRoute(actualRoute);
    rp.parseRoute(actualRoute);
    rp.parseRoute(actualRoute);
    
    // create number of random routes to include in test set
    int num_rand_routes = 4;
    bool foundIncorrectTestRoute = false;
    Route* incorrectTestRoute;
    
    for(int i = 1; i <= num_rand_routes; i++)
    {
        // create random route
        Route* randomRoute = rpCity->randomPath(startIntersection, &startRoute, std::ceil((float)std::rand() / RAND_MAX * routeLength), conditions);
        while(randomRoute->isEqual(actualRoute))
        {
            randomRoute = rpCity->randomPath(startIntersection, &startRoute, std::ceil((float)std::rand() / RAND_MAX * routeLength), conditions);
        }
        
        if(!foundIncorrectTestRoute)
        {
            incorrectTestRoute = randomRoute;
            foundIncorrectTestRoute = true;
        }
        
        // add NN vals
        setRouteNeuralNetworkVals(randomRoute, routeSpds, routeSpdsDist, rpCity);
        
        // add random route to test set
        rp.parseRoute(randomRoute);
    }
    
    // -----------------------
    // BEGIN DRIVER PREDICTION
    // -----------------------
    
    DriverPrediction dp(&rp);
    DriverPrediction::PredData predData;
    
    std::vector<float> routeSpdsTmp(routeSpds);
    
    float currSpd = 0.0;
    bool isFirstLink = true;
    
    actualRoute->getLinks()->initializeCounter();
    GenericEntry<long int, Link*>* nextLink = actualRoute->getLinks()->nextEntry();
    while(nextLink != NULL)
    {
        if(nextLink->value->isFinalLink())
        {
            std::cout << "Driver Prediction Unit Test Complete" << std::endl;
            break;
        }
        
        Road* road = rpCity->getRoads()->getEntry(nextLink->value->getNumber());
        float roadDist = road->getSplineLength();
        float distAlongLink = 0.0;
        
        if(isFirstLink)
        {
            distAlongLink = roadDist / 2;
            
            float removeBeforeIndex = (float) roadDist / routeSpdsDist * routeSpds.size();
            for(int i = 0; i < removeBeforeIndex; i++)
            {
                prevSpd = routeSpdsTmp.at(0);
                routeSpdsTmp.erase(routeSpdsTmp.begin());
            }
            
            currSpd = routeSpdsTmp.at(0);
            predData = dp.startPrediction(nextLink->value, currSpd, conditions, distAlongLink);
            
            isFirstLink = false;
        }
        
        while(distAlongLink < roadDist)
        {
            float accel_i = (currSpd - prevSpd) / sp.getDT();
            float dist_i = prevSpd + 0.5 * accel_i * std::pow(sp.getDT(),2);
            distAlongLink += dist_i;
            
            currSpd = routeSpdsTmp.at(0);
            routeSpdsTmp.erase(routeSpdsTmp.begin());
            
            prevSpd = currSpd;
            
            if(routeSpdsTmp.size() == 0)
            {
                for(int j = 0; j < routeSpds.size(); j++)
                {
                    routeSpdsTmp.push_back(routeSpds.at(j));
                }
            }
            
            predData = dp.nextPrediction(nextLink->value, currSpd, distAlongLink);
        }
        
        nextLink = actualRoute->getLinks()->nextEntry();
    }
    delete(nextLink);
    
}

