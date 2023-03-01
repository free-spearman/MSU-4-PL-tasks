#include "Graph.hpp" 

id_t Graph::AddCity(const std::string name){
	if (name.empty()){
		throw invalid_argument( "empty name");
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
	location v;
	this->flights.push_back(v);
	
	return nums;
};

id_t Graph::AddTransport(const std::string name){
	if (name.empty()){
		throw invalid_argument( "empty name");
	}
	// есть ключ в массиве
	try
	{
		return this->FindTransportByName(name);
	}
	catch(std::domain_error e){}
	
	id_t nums = this->GetNumTypeTransports();
	this->transports.insert(std::make_pair(name, nums));
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
		throw invalid_argument("there is no such city");
	}
	if (this->transports.size() < transport_type){
		throw invalid_argument("incorrect transport type");
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

