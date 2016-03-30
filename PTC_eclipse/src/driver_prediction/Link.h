/*
 * Link.h
 *
 *  Created on: Jan 5, 2016
 *      Author: Amanda
 */

#ifndef LINK_H_
#define LINK_H_

//#include "../city/City.h"

namespace PredictivePowertrain {

class Link {
public:
	int link_number;
	int link_direction;

	Link();
	Link(int, int);
	virtual ~Link();
	Link* copy(int, int);
	int getHash();
	bool isEqual(Link* other);
	int getNumber();
	int getDirection();
	static Link* newLinkFromHash(int);
	static Link* finalLink();
};

} /* namespace PredictivePowertrain */

#endif /* LINK_H_ */
