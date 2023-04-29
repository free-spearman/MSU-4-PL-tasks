#ifndef FLIGHT_H // include guard
#define FLIGHT_H

#include <iostream>

#include <list>
#include <set>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>

#include "custom_types.hpp"
#include "BaseLogger.hpp"
//#include
//#include <stdint.h> 

#define CRUISE_TIME_P 0
#define CRUISE_FARE_P 1
#define NUM_LOCALS_P 2

//using namespace std;

class Flight: public BaseLogger{
private:
	id_t transport_type;
protected:
	bool set_flag;
	id_t from;
	id_t to;
	weights_t weights; 
public:
	Flight();
	Flight(id_t from);
	Flight(const Flight &ref);
	Flight(id_t from,
		id_t to, 
		id_t transport_type,
		weight_t cruise_time,
		weight_t cruise_fare,
		weight_t locals);
	Flight& operator= (const Flight& ref);
	~Flight();
	id_t get_to() const;
	id_t get_from() const;
	void get_weights(weights_t &tgt) const;
	weights_t get_weights() const;
	std::string toString() const;
	id_t get_transport() const;
	void filled();
	bool isEmpty()const;
};

//using graph_t = vector<vector<Flight*>>;
//using vertex_t = vector<Flight>;

class Route: public Flight {
protected:
	std::list<Flight> flights;
public:
	Route();
	Route(id_t from);
	Route(const Route& ref);

	weight_t size() const;
	Flight& operator[] (int i);
	const Flight& operator[](int i) const;

	std::list<Flight>::const_iterator getFlightIter() const;
	Route operator + (const Route& ref) const;
	std::string toString() const;
	std::string routeToString() const; 
	void push_front(const Flight& f);
	void push_back(const Flight &f);
	~Route();	
};


#endif /*FLIGHT */
