#include "Flight.hpp"

//class Flight;
//using id_t = uint32_t;//по идеее не меньше 2^32 ), можно и uint32_t
//using weight_t = uint32_t;  
//using weights_t = weight_t[NUM_RT_PARM]; 
//using graph_t = vector<vector<Flight*>>;
//using criterion =  bool (*)(weights_t&  x, weights_t& y); 
//using vertex_t = vector<Flight>
//по throw https://isocpp.org/wiki/faq/exceptions#ctors-can-throw
/* Что хочу концептуально? 
map город : id 
map город_id: рейсы 
** поправка (зачем, если можно сделать вектор, где id-шкини -позиции в векторе )
** note: std::unordered_map vs std::vector 
https://stackoverflow.com/questions/22429719/stdvector-or-stdlist-for-stdunordered-map-buckets 

даст уникальность id-шников + избавит от возьни с переводом тудым-сюдым


** хочется хранить пути и сразу с ними работать, ибо звучит удобно с точки зрения прибавления, но вопрос памяти 
** если в пути хранить не копии, а указатели на рейсы, то вроде как проблема становится менее актуальной
** например если 2^32 городов и для каждого оптимальный путь 2^32-1 

** короче, храню просто рейсы в паре со стоимостью 

** я овощ, просто массив из 3-х элементов и рейс  
*/
Flight::Flight(){
	this->set_flag = false;
};
Flight::~Flight(){};

Flight::Flight(id_t from){
	this->from = from;
	this->to = from;
	SET_WEIGHTS_VAL(this->weights, 0);
	this->set_flag = false;
};

Flight::Flight(id_t from,
		id_t to, 
		id_t transport_type,
		id_t cruise_time,
		id_t cruise_fare,
		id_t locals
		): from(from), to(to), transport_type(transport_type){ 
		//this->from = from;
		//this->to = to;
		//this->transport_type = transport_type;
		/*if (cruise_time < 0 || cruise_fare < 0 )
			throw invalid_argument( "received negative value" );
		*/ 	
		this->weights[CRUISE_TIME_P] = cruise_time;
		this->weights[CRUISE_FARE_P] = cruise_fare;
		this->weights[NUM_LOCALS_P] = locals;
		this->set_flag = true;
};

Flight::Flight(const Flight &ref){
	this->from = ref.from;
	this->to = ref.to;
	this->weights = ref.weights;
	this->transport_type = ref.transport_type;
	this->set_flag = true;
}
id_t  Flight::get_to() const{
	return this->to;
};
id_t  Flight::get_from() const{
	return this->from;
};
void Flight::get_weights(weights_t &tgt) const{
	SET_WEIGHTS_WEIGHTS(tgt, this->weights); 
};

id_t Flight::get_transport() const{
	return this->transport_type; 
};

weights_t Flight::get_weights() const{
	return this->weights; 
};
///
std::string Flight::toString() const{
	std::string s = std::to_string(this->get_to())+ " -> " + std::to_string(this->get_from());
	s +=" CRUISE_TIME:" + std::to_string(this->weights[CRUISE_TIME_P]);
	s +=" CRUISE_FARE:" + std::to_string(this->weights[CRUISE_FARE_P]);

	return s; 
};

bool Flight::isEmpty(){
	return !this->set_flag; 
};

Route::Route(id_t from): Flight(from){
	/*this->from = from;
	this->to = from;
	SET_WEIGHTS_VAL(this->weights, 0);
	//this->flights.push_front(NULL);
	*/
};
Route::Route(const Route &ref){
	this->flights = ref.flights;
	this->from = ref.from; 
	this->to = ref.to;
	this->weights = ref.weights; 
};

Route::Route():Flight::Flight(){
};

Route::~Route(){};

void Route::push_front(const Flight &f){
	/*if (f == NULL)
		throw invalid_argument( "received empty Flight" );
	*/
	//if (this->from = this->to && this->flights.size() == 0)
	
	this->flights.push_front(f);
	this->from =f.get_from();

	weights_t w;
	f.get_weights(w);
	ADD_WEIGHTS(this->weights, this->weights, w);   
};

std::string Route::toString() const{
	std::string s = Flight::toString();
	s+= " NUM_LOCALS:" + std::to_string(this->weights[NUM_LOCALS_P]);
	return s;
};

std::string Route::routeToString() const{
	std::string s;
	for(auto ticket :this->flights){
		s+= ticket.toString() + "\n";
	}
	return s;
};
/*
class Route: public Flight{
protected:
	list<Flight>
public:
	Route();
	~Route();
	
}
*/