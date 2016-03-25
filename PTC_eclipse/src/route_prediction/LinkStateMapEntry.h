/*
 * LinkStateMapEntry.h
 *
 *  Created on: Mar 13, 2016
 *      Author: vagrant
 */

#ifndef ROUTE_PREDICTION_LINKSTATEMAPENTRY_H_
#define ROUTE_PREDICTION_LINKSTATEMAPENTRY_H_
#include "GenericMap.h"
#include "../driver_prediction/Link.h"

using namespace DriverPrediction;

namespace RoutePrediction {

class LinkStateMapEntry {
private:
	GenericMap<int,int> entries;
public:
	LinkStateMapEntry();
	LinkStateMapEntry(LinkStateMapEntry &other);
	int add_entry(Link);
	int get_m(Link);
	int get_total_m();
	virtual ~LinkStateMapEntry();
	static int total_m(LinkStateMapEntry);
};

} /* namespace RoutePrediction */

#endif /* ROUTE_PREDICTION_LINKSTATEMAPENTRY_H_ */