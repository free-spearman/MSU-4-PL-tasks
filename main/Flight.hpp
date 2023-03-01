#ifndef FLIGHT_H // include guard
#define FLIGHT_H

#include <iostream>

#include <list>
#include <set>
#include <vector>
#include <stdexcept>
#include <limits>

#include "custom_types.hpp"
#include
//#include <stdint.h> 

#define CRUISE_TIME_P 0
#define CRUISE_FARE_P 1
#define NUM_LOCALS_P 2

using namespace std;

class Flight{
private:
	id_t transport_type;
protected:
	id_t from;
	id_t to;
	weights_t weights; 
public:
	Flight(id_t from);
	Flight(const Flight &ref);
	Flight(id_t from,
		id_t to, 
		id_t transport_type,
		weight_t cruise_time,
		weight_t cruise_fare,
		weight_t locals);
	
	~Flight();
	id_t get_to();
	id_t get_from();
	void get_weights(weights_t &tgt);
	weights_t get_weights();
};

using graph_t = vector<vector<Flight*>>;
using vertex_t = vector<Flight>;

class Route: public Flight {
protected:
	list<Flight*> flights;
public:
	Route(id_t from);
	void push_front(Flight* f);
	~Route();	
};




#endif /*FLIGHT */