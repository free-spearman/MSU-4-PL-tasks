#ifndef SEARCHALGORITHMS // include guard
#define SEARCHALGORITHMS

#include "custom_types.hpp"
#include "Graph.hpp"

// функции для сравнения 
// bool (*)(weights_t&  x, weights_t& y);

bool x_faster_y(const weights_t &x, const weights_t &y){
	return x[CRUISE_TIME_P] < y[CRUISE_TIME_P];
}

bool x_short_y(const weights_t&  x, const weights_t& y){
	return x[NUM_LOCALS_P] < y[NUM_LOCALS_P];
}

bool x_cheaper_y(const weights_t&  x,const weights_t& y){
	return x[CRUISE_FARE_P] < y[CRUISE_FARE_P];
}

/*bool x_cheaper_short_y(weights_t&  x, weights_t& y){
	return x[CRUISE_FARE_P] < y[CRUISE_FARE_P];
}*/

bool x_cheaper_fast_y(const weights_t&  x, const weights_t& y){
	if ( x[CRUISE_FARE_P] < y[CRUISE_FARE_P])
		return true;
	else if (x[CRUISE_FARE_P] > y[CRUISE_FARE_P])
		return false;
	return x[CRUISE_TIME_P] < y[CRUISE_TIME_P];
}

bool x_fast_cheaper_y(const weights_t&  x, const weights_t& y){
	if (x[CRUISE_TIME_P] < y[CRUISE_TIME_P]){
		return true;
	}
	else if (x[CRUISE_TIME_P] > y[CRUISE_TIME_P]){
		return false;
	}
	return x[CRUISE_FARE_P] < y[CRUISE_FARE_P];
}

class SearchAlgorithm {
protected:
	Route *actl_rout;
	id_t to;
	id_t from;
	graph_t *graph;
	bool setting_f {false};
	weight_t max_weight = numeric_limits<weight_t>::max();
public:
	SearchAlgorithm(graph_t *g);
	SearchAlgorithm(graph_t *g, id_t to, id_t from);
	~SearchAlgorithm();
	virtual vector<Flight*> calcOptRoute (id_t& to, id_t& from, weights_t lim, criterion xlessy) = 0;
	virtual Route* findFastRoute (id_t& from, id_t& to, weight_t lim) = 0;
	virtual Route* findCheapRoute (id_t& from, id_t& to, weight_t lim) = 0;
	virtual Route* findShortRoute (id_t& from, id_t& to, weight_t lim) = 0;
};

class Dijkstra: public SearchAlgorithm {
protected:
	std::vector<Flight*> previous;
	std::vector<weights_t> opt_distance;  
public:
	
	Dijkstra(graph_t* g);
	//findOptRoute
	
	vector<Flight*>  calcOptRoute (id_t& from, id_t& to, weights_t lim,  criterion xlessy);	
	Route* restoreRoute(const id_t &to);
	Route* findFastRoute (id_t& from, id_t& to, weight_t lim);
	Route* findCheapRoute (id_t& from, id_t& to, weight_t lim);
	Route* findShortRoute (id_t& from, id_t& to, weight_t lim);

};

#endif /* SEARCHALGORITHMS */