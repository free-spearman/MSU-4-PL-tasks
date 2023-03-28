#include "Graph.hpp" 

Graph::Graph(){};
Graph::~Graph(){};
id_t Graph::AddCity(const std::string name){
	if (name.empty()){
		throw std::invalid_argument( "empty name");
	}
	// есть ключ в массиве, если нет, то FindCityByName кидает exp
	try
	{
		return this->FindCityByName(name);
	}
	catch(std::domain_error e)
	{}
	//std::unordered_map<std::string, id_t>::iterator itr = this->citys.find(name);
	//if (itr != this->citys.end()){
	//	return *itr;
	//}
	//размер = свободное место	
	id_t nums = this->GetNumCitys();

	this->citys.insert(std::make_pair(name, nums));
	this->citys_names.insert(std::make_pair(nums, name));
	location v;
	this->flights.push_back(v);
	
	return nums;
};

id_t Graph::AddTransport(const std::string name){
	if (name.empty()){
		throw std::invalid_argument( "empty name");
	}
	// есть ключ в массиве
	try
	{
		return this->FindTransportByName(name);
	}
	catch(std::domain_error e){}
	
	id_t nums = this->GetNumTypeTransports();
	this->transports.insert(std::make_pair(name, nums));
	this->transports_names.insert(std::make_pair(nums, name));
	return nums;
};

//(id_t from, id_t to, id_t transport_type, id_t cruise_time, id_t cruise_fare,id_t locals
void Graph::AddFlight(id_t from,
	id_t to, 
	id_t transport_type, 
	weight_t time, 
	weight_t fare, 
	weight_t locals
	){
	id_t nums =this->GetNumCitys();
	if (nums < from || nums < to) {
		throw std::invalid_argument("there is no such city");
	}
	if (this->transports.size() < transport_type){
		throw std::invalid_argument("incorrect transport type");
	} 

	this->flights[from].push_back(Flight(from, to, transport_type, time, fare, locals));
};

// ищет город, если такого нет, то кидает исслючение
id_t Graph::FindCityByName(const std::string name){
	std::unordered_map<std::string, id_t>::iterator itr = this->citys.find(name);
	if (itr == this->citys.end()){
		throw std::domain_error("There is no such name."); 
	}
	return this->citys[name];
};


id_t Graph::FindTransportByName(const std::string name){
	std::unordered_map<std::string, id_t>::iterator itr = this->transports.find(name);
	if (itr == this->transports.end()){
		throw std::domain_error("there is no such name."); 
	}
	return this->transports[name];	
};

id_t Graph::GetNumCitys(){
	return this->flights.size();
};

id_t Graph::GetNumTypeTransports(){
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
