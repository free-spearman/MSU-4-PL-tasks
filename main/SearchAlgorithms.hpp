#ifndef SEARCHALGORITHMS // include guard
#define SEARCHALGORITHMS

#include "custom_types.hpp"
#include "Graph.hpp"


class SearchAlgorithm { 
protected:
	/* *** Блок пути ***  */
	Route actl_rout;
	id_t to;
	id_t from;
	// добавил когда-то, пока не понятно зачем 
	weight_t max_weight = std::numeric_limits<weight_t>::max();
	// граф путей 
	Graph graph;
	/* *** Блок ограничений ***  */
	//цена, время, количество локакаций
	weights_t lims;
	//Доступный транспорт, если весь, то size = 0
	std::set<weight_t> avable_transport; 
	//флаг на готовность к поиску маршрута
	bool setting_f {false};

public:
	SearchAlgorithm();
	SearchAlgorithm(Graph& g);
	SearchAlgorithm(Graph& g, id_t to, id_t from);
	~SearchAlgorithm();


	bool addAvableTransport(id_t t_id);
	bool checkTransportRestrictions(id_t t);
	bool checkWeightRestrictions(weights_t& w);


	virtual std::vector<Flight> calcOptRoute (id_t& to, id_t& from, criterion xlessy) = 0;
	virtual Route findFastRoute (id_t& from, id_t& to) = 0;
	virtual Route findCheapRoute (id_t& from, id_t& to) = 0;
	virtual Route findShortRoute (id_t& from, id_t& to) = 0;
};

class Dijkstra: public SearchAlgorithm {
protected:
	std::vector<Flight> previous;
	std::vector<weights_t> opt_distance;  
public:
	Dijkstra();
	Dijkstra(Graph& g);
	//findOptRoute
	
	std::vector<Flight>  calcOptRoute (id_t& from, id_t& to,  criterion xlessy);	
	Route restoreRoute(const id_t &to);
	Route findFastRoute (id_t& from, id_t& to);
	Route findCheapRoute (id_t& from, id_t& to);
	Route findShortRoute (id_t& from, id_t& to);

};

#endif /* SEARCHALGORITHMS */
