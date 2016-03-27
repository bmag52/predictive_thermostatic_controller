/*
 * Route.cpp
 *
 *  Created on: Mar 6, 2016
 *      Author: vagrant
 */

#include "Route.h"
#include "Goal.h"
#include "../driver_prediction/Link.h"

using namespace DriverPrediction;

namespace RoutePrediction {

Route::Route() {
	this->counter = 0;
}

<<<<<<< HEAD
Route::Route(Link links[], Goal& goal) {
	this->counter = 0;
=======
Route::Route(Link* links, Goal* goal) {
>>>>>>> master
	this->links = links;
	this->goal = goal;
}

// adds new link to end of route
<<<<<<< HEAD
void Route::addlink(Link link) {
	int length = sizeof(this->links) / sizeof(Link);
	Link temp[] = new Link[length];
	for(int i = 0; i < length; i++) {
		temp[i] = links[i];
	}
	temp[length] = link;
	this->links = temp;
=======
void Route::addlink(Link* link) {
//	int length = sizeof(this->links) / sizeof(Link);
//	Link temp[] = new Link[length];
//	for(int i = 0; i < length; i++) {
//		temp[i] = links[i];
//	}
//	temp[length] = link;
//	this->links = temp;
>>>>>>> master
}

// checks if the route is equal to the route passed in
bool Route::isequal(Route& other) {
	int mylength = sizeof(this->links) / sizeof(Link);
	int otherlength = sizeof(other.links) / sizeof(Link);
	if(!(mylength == otherlength) || !((this->goal).isequal(other.goal))) {
		return false;
	}
	for(int i = 0; i < mylength; i++) {
		if(!(this->links[i].isEqual(other.links[i]))) {
			return false;
		}
	}
	return true;
}

//returns copy of current route
Route Route::copy() {
	Goal goal = Goal(this->goal.destination, this->goal.bins, this->goal.size);
	int length = sizeof(this->links) / sizeof(Link);
	links = Link[length];
	for(int i = 0; i < length; i++) {
		int number = this->links[i].getNumber();
		int direction = this->links[i].getDirection();
		links[i] = this->links[i].copy(direction, number);
	}
	return Route(links, goal);
}

void initiateiterator() {
	this->counter = 0;
}

int Route::getGoalHash() {
	return (*this->goal).getHash();
}

Goal* Route::getGoalPtr() {
	return this->goal;
}

int Route::getLinkSize() {
	return sizeof(*this->links)/sizeof(Link);
}

Link* Route::getLinksPtr() {
	return this->links;
}

//returns next link in the iterator, returns 0 if there are no more links left
Link Route::nextlink() {
	int length = sizeof(this->links) / sizeof(Link);
	if(this->counter > length) {
		return this->links[length-1]; // return last link
	} else {
		Link link = this->links[this->counter];
		this->counter++;
		return link;
	}
}

}


