/*
 * LinkToStateMap.h
 *
 *  Created on: Mar 13, 2016
 *      Author: vagrant
 */

#ifndef ROUTE_PREDICTION_LINKTOSTATEMAP_H_
#define ROUTE_PREDICTION_LINKTOSTATEMAP_H_
#include "../map/GenericMap.h"
#include "LinkStateMapEntry.h"
#include "Goal.h"
#include "../map/GenericEntry.h"

namespace PredictivePowertrain {

class LinkToStateMap {
private:
	GenericMap<int, LinkStateMapEntry*> linkMap;
	GenericMap<int, GoalMapEntry*> goalMap;

public:
	int incrementTransition(Link* link1, Goal* goal, Link* link2);
	double getProbability(Link* link1, Link* link2, Goal * goal, bool isSimilar);
	LinkToStateMap();
	LinkToStateMap(LinkToStateMap& other);
	virtual ~LinkToStateMap();
};

} /* namespace PredictivePowertrain */

#endif /* ROUTE_PREDICTION_LINKTOSTATEMAP_H_ */
