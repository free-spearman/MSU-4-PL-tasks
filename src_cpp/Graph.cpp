#include "Graph.hpp" 

Graph::Graph(){};
Graph::~Graph(){};

Graph::Graph( const Graph& ref){
	this->citys = ref.citys;
	this->citys_names = ref.citys_names;
	this->transports = ref.transports;
	this->transports_names = ref.transports_names;
	this->flights = this->flights;
};

id_t Graph::addCity(const std::string name){
	if (name.empty()){
		throw std::invalid_argument( "empty name");
	}
	// есть ключ в массиве, если нет, то FindCityByName кидает exp
	try
	{
		return this->findCityByName(name);
	}
	catch(std::domain_error e)
	{}
	//std::unordered_map<std::string, id_t>::iterator itr = this->citys.find(name);
	//if (itr != this->citys.end()){
	//	return *itr;
	//}
	//размер = свободное место	
	id_t nums = this->getNumCitys();

	this->citys.insert(std::make_pair(name, nums));
	this->citys_names.insert(std::make_pair(nums, name));
	location v;
	this->flights.push_back(v);
	
	return nums;
};

id_t Graph::addTransport(const std::string name){
	if (name.empty()){
		throw std::invalid_argument( "empty name");
	}
	// есть ключ в массиве
	try
	{
		return this->findTransportByName(name);
	}
	catch(std::domain_error e){}
	
	id_t nums = this->getNumTypeTransports();
	this->transports.insert(std::make_pair(name, nums));
	this->transports_names.insert(std::make_pair(nums, name));
	return nums;
};

//(id_t from, id_t to, id_t transport_type, id_t cruise_time, id_t cruise_fare,id_t locals
void Graph::addFlight(id_t from,
	id_t to, 
	id_t transport_type, 
	weight_t time, 
	weight_t fare, 
	weight_t locals
	){
	id_t nums =this->getNumCitys();
	if (nums < from || nums < to) {
		throw std::invalid_argument("there is no such city");
	}
	if (this->transports.size() < transport_type){
		throw std::invalid_argument("incorrect transport type");
	} 

	this->flights[from].push_back(Flight(from, to, transport_type, time, fare, locals));
};

// ищет город, если такого нет, то кидает исслючение
id_t Graph::findCityByName(const std::string name) {
	auto itr = this->citys.find(name);
	if (itr == this->citys.end()){
		throw std::domain_error("There is no such name."); 
	}
	return this->citys[name];
};
std::string Graph::findCityById(const id_t id){
	//std::unordered_map<std::string, id_t>::iterator itr = this->citys_names.find(id);
	auto itr = this->citys_names.find(id);
	if (itr == this->citys_names.end()){
		throw std::domain_error("There is no such id."); 
	}
	return this->citys_names[id];
};

id_t Graph::findTransportByName(const std::string name) {
	std::unordered_map<std::string, id_t>::iterator itr = this->transports.find(name);
	if (itr == this->transports.end()){
		throw std::domain_error("there is no such name."); 
	}
	return this->transports[name];	
};

std::string Graph::findTransportById(const id_t id) {
	//std::unordered_map<std::string, id_t>::iterator itr = this->transports_names.find(id);
	auto itr = this->transports_names.find(id);
	if (itr == this->transports_names.end()){
		throw std::domain_error("There is no such id."); 
	}
	return this->transports_names[id];
};

id_t Graph::getNumCitys() const{
	return this->flights.size();
};

id_t Graph::getNumTypeTransports() const{
	return this->transports.size(); 	
};
 
std::string Graph::flightsToString(const Flight& flight){ 
	std::string from = this->citys_names[ flight.get_from()];
	std::string to = this->citys_names[flight.get_to()];
	std::string transport = this->transports_names[flight.get_transport()];
	weights_t w = flight.get_weights();
	std::string result = from + " -> " + to + " тр-рт:" + transport + " время:" + std::to_string(w[CRUISE_TIME_P]) + " цена:" + std::to_string(w[CRUISE_FARE_P]);  
	//std::format("{} -> {} тр-рт{} время:{} цена:{} \n", from, to, transport, w[CRUISE_TIME_P], w[CRUISE_FARE_P]);
	return result + "\n";
};
//вывод в файл, надо сделать оператор <<
void Graph::printInFile(std::ofstream& out){
	if ( ! out.is_open()){
		throw std::runtime_error("Файл не открыт\n");
	}
	for( location loc :this->flights ){
		for(Flight f :loc){
			out<<(this->flightsToString(f)); 
		}
	}
	out<<std::endl;
};
id_t Graph::size() const{
	return this->getNumCitys();
};
