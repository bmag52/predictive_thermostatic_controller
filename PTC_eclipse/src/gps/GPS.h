/*
 * GPS.h
 *
 *  Created on: Apr 17, 2016
 *      Author: vagrant
 */

#ifndef GPS_GPS_H_
#define GPS_GPS_H_

#include <utility>
#include "../city/Road.h"

namespace PredictivePowertrain {

class GPS {
private:
	GenericMap<double, double> tripLog;

public:
	GPS();
	static std::pair<double, double>* getLatLon();
	static bool isOnRoad(Road* road);
	void updateTripLog();
	GenericMap<double, double>* getTripLog();
};

}



#endif /* GPS_GPS_H_ */