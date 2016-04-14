/*
 * GoalToLinkMap.h
 *
 *  Created on: Mar 13, 2016
 *      Author: vagrant
 */

#ifndef ROUTE_PREDICTION_GOALTOLINKMAP_H_
#define ROUTE_PREDICTION_GOALTOLINKMAP_H_
#include "../map/GenericMap.h"
#include "GoalMapEntry.h"
#include "../driver_prediction/Link.h"
#include "Goal.h"



namespace PredictivePowertrain {

class GoalToLinkMap {
private:
	GenericMap<int,GoalMapEntry*> goalMap;
	GenericMap<int, int> linkHashCounts;
public:
	int maps_size;
	GoalToLinkMap();
	GoalToLinkMap(GoalToLinkMap &other);
	int linkTraversed(Link* link, Goal* goal);
	double probabilityOfGoalGivenLink(Link * link, Goal * goal, bool isSimilar);
	virtual ~GoalToLinkMap();
};

} /* namespace PredictivePowertrain */

#endif /* ROUTE_PREDICTION_GOALTOLINKMAP_H_ */
