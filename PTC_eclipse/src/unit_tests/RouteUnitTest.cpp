/*
 * RouteUnitTest.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: vagrant
 */

#include "../driver_prediction/Link.h"
#include "../route_prediction/Goal.h"
#include "../route_prediction/Route.h"
#include <iostream>
#include <assert.h>
#include "UnitTests.h"

using namespace std;
using namespace DriverPrediction;
using namespace RoutePrediction;

void route_ut() {
	Link * link1 = new Link(1, 0);
	Link * link2 = new Link(2, 1);
	Link * link3 = new Link(3, 0);

	Link links1[3];
	links1[0] = *link1;
	links1[1] = *link2;
	links1[2] = *link3;
	Link links2[3];
	links2[0] = *link3;
	links2[1] = *link1;
	links2[2] = *link2;

	int bins[] = {1};
	Goal * goal1 = new Goal(1, bins);
	Goal * goal2 = new Goal(2, bins);

	Route * route1 = new Route(links1, goal1);
	Route * route2 = new Route(links1, goal2);
	Route * route3 = new Route(links1, goal1);
	Route * route4 = new Route(links2, goal1);
	Route * route5 = new Route(links2, goal2);


	// Test 1: addlink
	Link emptyLinks[0];
	Route * route = new Route(emptyLinks, goal1);
	(*route).addlink(link1);
	assert((*(*route).getLastLinkPtr()).isEqual(link1) && (*route).getLinkSize() == 1);
	(*route).addlink(link2);
	assert((*(*route).getLastLinkPtr()).isEqual(link2) && (*route).getLinkSize() == 2);
	(*route).addlink(link3);
	assert((*(*route).getLastLinkPtr()).isEqual(link3) && (*route).getLinkSize() == 3);

	// Test 2: iterator
	route = new Route(links1, goal1);
	int linksLength = sizeof(*links1)/sizeof(Link);
	for(int i = 0; i < linksLength; i++) {
		Link tempLink = ((*route).nextlink());
		assert(links1[i].isEqual(&tempLink));
	}
	Link lastLink = (*route).nextlink();
	assert(links1[linksLength - 1].isEqual(&lastLink));

	// Test 3: isequal to oneself
	assert((*route1).isequal(route1));

	// Test 4: isequal is reversible
	assert((*route1).isequal(route3));
	assert((*route3).isequal(route1));
	assert(!(*route1).isequal(route2));
	assert(!(*route2).isequal(route1));

	// Test 5: isequal with same links / goals (but still unequal)
	assert(!(*route1).isequal(route4));
	assert(!(*route1).isequal(route5));

	// Test 6: copy
	Route tempRoute = (*route1).copy();
	route2 = &tempRoute;
	assert((*route1).isequal(route2));
	Link * linksPtr = (*route2).getLinksPtr();
	linksPtr[0] = *link2;
	assert(!(*route1).isequal(route2));
}





