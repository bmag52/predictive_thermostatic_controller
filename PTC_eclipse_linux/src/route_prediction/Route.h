/*
 * Route.h
 *
 *  Created on: Mar 6, 2016
 *      Author: vagrant
 */

#ifndef ROUTE_H_
#define ROUTE_H_

#include "Goal.h"
#include "../driver_prediction/Link.h"
#include "../city/Intersection.h"

namespace PredictivePowertrain {

class Route {
private:
	GenericMap<int, Link*>* links;
	Goal* goal;
	Link* error;
	Intersection* intersection;
	bool goalIsIntersection;
	int linkCount;
public:
	Route();
	Route(GenericMap<int, Link*>* links, Goal* goal);
	void addlink(Link* link);
	bool isequal(Route* other);
	int getGoalHash();
	int getLinkSize();
	Link* getEntry(int index);
	void setToIntersection(Intersection* other);
	bool isIntersection();
	bool isEmpty();
	Goal* getGoalPtr();
	GenericMap<int, Link*>* getLinksPtr();
	Intersection* getIntersectionPtr();
	Link* getLastLinkPtr();;
	Route* copy();
	void removeFirstLink();
};

}

#endif /* ROUTE_H_ *//*
 * Route.h
 *
 *  Created on: Mar 6, 2016
 *      Author: vagrant
 */

#ifndef ROUTE_H_
#define ROUTE_H_

#include "Goal.h"
#include "../driver_prediction/Link.h"
#include "../city/Intersection.h"

namespace PredictivePowertrain {

class Route {
private:
	Link* links;
	Goal* goal;
	Link* error;
	Intersection* intersection;
	bool goalIsIntersection;
	int counter;
public:
	Route();
	Route(Link* links, Goal* goal);
	void addlink(Link* link);
	bool isequal(Route* other);
	int getGoalHash();
	int getLinkSize();
	Link* getEntry(int index);
	void setToIntersection(Intersection* other);
	bool isIntersection();
	bool isEmpty();
	Goal* getGoalPtr();
	Link* getLinksPtr();
	Intersection* getIntersectionPtr();
	Link* getLastLinkPtr();;
	Route* copy();
	void removeFirstLink();
	Link* nextlink();
};

}

#endif /* ROUTE_H_ */
