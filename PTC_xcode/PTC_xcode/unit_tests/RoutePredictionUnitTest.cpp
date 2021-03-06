/*
 * RoutePredictionUnitTest.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: vagrant
 */

#include "../driver_prediction/Link.h"
#include "../city/City.h"
#include "../city/Intersection.h"
#include "../route_prediction/Goal.h"
#include "../data_management/DataManagement.h"
#include "../route_prediction/RoutePrediction.h"
#include "../route_prediction/Route.h"

#include <math.h>
#include <iostream>
#include <assert.h>
#include <climits>
#include <algorithm>
#include <ctime>
#include <vector>

using namespace PredictivePowertrain;

Link* makeLinks(int numberOfLinks)
{
	Link* links = new Link[numberOfLinks];
	for(int i = 0; i < numberOfLinks; i++)
	{
		Link newLink(rand() > .5, i + 1);
		links[i] = newLink;
	}
    return links;
}

RoutePrediction* routePrediction_ut()
{
    // seed random generator
    std::srand(std::time(0));
    
    // create a square city grid to predict over
    const int gridSideDim = 4;
    const int numInters = gridSideDim * gridSideDim;
    Intersection inters[numInters];
    
    GenericMap<long int, Road*>* roads = new GenericMap<long int, Road*>();
    GenericMap<long int, Intersection*>* intersections = new GenericMap<long int, Intersection*>();
    
    // fence post addage of the first intersection
    inters[0].assignID(1);
    intersections->addEntry(inters[0].getIntersectionID(), &inters[0]);
    
    // make all grid connections
    long int horzCount = 0;
    long int vertCount = (gridSideDim - 1) * gridSideDim;
    for(int i = 1; i < numInters; i++)
    {
        int interID = i + 1;
        
        inters[i].assignID(interID);
        
        // make horizontal connections
        if(i % gridSideDim != 0)
        {
            horzCount++;
            
            Road* newHorzRoad = new Road();
            newHorzRoad->assignID(horzCount);
            
            inters[i].addRoad(newHorzRoad, 1);
            inters[i-1].addRoad(newHorzRoad, 0);
            
            roads->addEntry(horzCount, newHorzRoad);
        }
        
        // make vertical conections
        if(interID > gridSideDim)
        {
            vertCount++;
            
            Road* newVertRoad = new Road();
            newVertRoad->assignID(vertCount);
            
            inters[i].addRoad(newVertRoad, 1);
            inters[i - gridSideDim].addRoad(newVertRoad, 0);
            
            roads->addEntry(vertCount, newVertRoad);
        }
        
        intersections->addEntry(inters[i].getIntersectionID(), &inters[i]);
    }
    
    // build city
    City* city = new City(intersections, roads, new GenericMap<int, Bounds*>());
    Link* links = makeLinks(5);
    std::vector<float>* conditions = new std::vector<float>(2);
    conditions->at(0) = 1;
    conditions->at(1) = 2;
    Goal goal(1, conditions);
    
    // add routes to route prediction
    RoutePrediction* rp = new RoutePrediction(city);
    Link link = links[0];
    int routeLength = 10;
    Intersection* startIntersection = city->getIntersectionFromLink(&link, true);
    
    // generate random actual route
    Route startRoute;
    startRoute.addLink(&link);
    startRoute.assignGoal(&goal);
    Route* actualRoute = city->randomPath(startIntersection, &startRoute, routeLength, conditions);
    
    // add trainging iterations here (simulates driving over the route multiple times)
    rp->parseRoute(actualRoute);
//    rp->parseRoute(actualRoute);
//    rp->parseRoute(actualRoute);
//    rp->parseRoute(actualRoute);
//    rp->parseRoute(actualRoute);
//    rp->parseRoute(actualRoute);
    
    
    // create number of random routes to include in test set
    int num_rand_routes = 4;
    
    for(int i = 1; i <= num_rand_routes; i++)
    {
        // create random route
        Route* randomRoute = city->randomPath(startIntersection, &startRoute, std::ceil((float)std::rand() / RAND_MAX * routeLength), conditions);
        while(randomRoute->isEqual(actualRoute))
        {
            randomRoute = city->randomPath(startIntersection, &startRoute, std::ceil((float)std::rand() / RAND_MAX * routeLength), conditions);
        }
        
        // add random route to test set
        rp->parseRoute(randomRoute);
    }
    
    // predict actual route as it is 'driven' over

    int traversalNum = 14;
    int totPredIters = 0;
    
    for(int i = 1; i <= traversalNum; i++)
    {
        std::cout << "+++++++++++++++++++++++++++++++++++++++ Route Traversal: " << i << " +++++++++++++++++++++++++++++++++++++++" << std::endl;
        
        int predIter = 1;
        Route* actualRouteCopy = actualRoute->copy();
        Route* predRoute = rp->startPrediction(actualRouteCopy->getLinks()->getEntry(0), startIntersection, conditions);
        while(actualRouteCopy->getLinkSize() > 2)
        {
            std::cout << "--- route prediction iteration " << predIter << " ---" << std::endl;
            
            // update actual route as it's 'driven' over
            actualRouteCopy->removeFirstLink();
            
            // print actual route
            actualRouteCopy->printLinks();
            
            // print predicted route
            predRoute->printLinks();
            
            // check if predicted route is actual route
            if(actualRouteCopy->isEqual(predRoute))
            {
                std::cout << "predicted routed before reaching end destination!" << std::endl;
                std::cout << "Links traversed: " << predIter << std::endl;
                std::cout << "Links in route: " << actualRouteCopy->getLinkSize() << std::endl;
                break;
            }
            else
            {
                totPredIters++;
            }
            
            // predict route
            predRoute = rp->predict(actualRouteCopy->getLinks()->getEntry(0));
            
            predIter++;
        
        }
        
        rp->parseRoute(actualRoute);
    }
    
    std::cout << "prediction accuracy: " << 1.0 - (float)totPredIters / ((float)traversalNum * (actualRoute->getLinks()->getSize() - 1)) << std::endl;
    
    return rp;

}
