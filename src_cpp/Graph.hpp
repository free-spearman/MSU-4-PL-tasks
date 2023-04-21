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
	Graph(const Graph& ref);
	~Graph();
	id_t addCity(const std::string name);
	id_t addTransport(const std::string name);
	void addFlight(id_t from,
		id_t to, 
		id_t transport_type, 
		weight_t time, 
		weight_t fare, 
		weight_t locals
	);
	id_t findCityByName(const std::string name) ;
	id_t findTransportByName(const std::string name);
	
	std::string findCityById(const id_t id) ;
	std::string findTransportById(const id_t id);

	id_t getNumCitys() const;
	id_t getNumTypeTransports() const;
	std::string flightToString(const Flight& flight);
	std::string routeToString(const Route& path);
	void printInFile(std::ofstream& out);
	id_t size() const;
	location& operator[] (int i) { return this->flights[i]; };
	const location& operator[](int i) const{ return this->flights[i]; };



protected:
	std::unordered_map<std::string, id_t> citys;
	std::unordered_map<id_t, std::string> citys_names;

	std::unordered_map<std::string, id_t> transports;
	std::unordered_map<id_t, std::string> transports_names;
	std::vector<location> flights;
};



#endif /* GRAPH_H */
