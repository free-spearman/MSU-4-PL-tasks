#ifndef GRAPH_H // include guard
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <fstream>
#include <stdexcept>

#include "custom_types.hpp"
#include "Flight.hpp"

using location = std::vector<Flight>;

/*class Location{
protected:
	id_t id;
	vector<Flight> flights; //нужен лю using?
public:

	Location(id_t id);
	~Location();
};
*/

class Graph {
public:
	Graph();
	~Graph();
	id_t AddCity(const std::string name);
	id_t AddTransport(const std::string name);
	void AddFlight(id_t from,
		id_t to, 
		id_t transport_type, 
		weight_t time, 
		weight_t fare, 
		weight_t locals
	);
	id_t FindCityByName(const std::string name);
	id_t FindTransportByName(const std::string name);
	id_t GetNumCitys();
	id_t GetNumTypeTransports();
	std::string flightsToString(const Flight& flight);
	void printInFile(std::ofstream& out);


protected:
	std::unordered_map<std::string, id_t> citys;
	std::unordered_map<id_t, std::string> citys_names;

	std::unordered_map<std::string, id_t> transports;
	std::unordered_map<id_t, std::string> transports_names;
	std::vector<location> flights;
};



#endif /* GRAPH_H */