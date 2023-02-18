#include <stdexcept>
#define NUM_RT_PARM 3

using id_t = size_t;//по идеее не меньше 2^32 ), можно и uint32_t
using weight_t = double [NUM_RT_PARM]; 
using graph_t = vector<vector<Flight*>>;
using criterion =  bool (*)(weight_t&  x, weight_t& y); 
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



class Flight{
protected:
	id_t from {0};
	id_t to {0};
	id_t transport_type {0};

	double cruise_time {0.0};
	double cruise_fare {0.0};
public:
	Flight(size_t from,
		size_t to, 
		size_t transport_type,
		double cruise_time,
		double cruise_fare): from(from), to(to), transport_type(transport_type){ 
		//this->from = from;
		//this->to = to;
		//this->transport_type = transport_type;
		if (cruise_time < 0 || cruise_fare < 0 )
			throw invalid_argument( "received negative value" );
		this->cruise_time = cruise_time;
		this->cruise_fare = cruise_fare;
	}
	~Flight();
	
};

class Route: public SearchAlgorithm{
protected:
	list<Flight>
public:
	Route();
	~Route();
	
};

class Location {
protected:
	string name;
	const id_t id;
	vector<Flight*> flights; //нужен лю using?
public:
	Location(string name, id_t id ): name(name), id(id){};
	~Location();

};


class SearchAlgorithm {
protected:
	Route actl_rout;

	id_t to;
	id_t from;
	graph_t graph;
	bool setting_f {false};
	const double max_weight;
public:
	SearchAlgorithm(graph_t& g): graph(g){
		this->max_weight =  numeric_limits<double>::infinity();

	};
	SearchAlgorithm(graph_t& g, id_t to, id_t from): to(to), from(from), SearchAlgorithm(g){};
	~SearchAlgorithm();
	virtual Route findOptRoute (id_t& to, id_t& from, criterion xlessy) = 0;
	virtual Route findFastRoute (id_t& to, id_t& from, double lim) = 0;
	virtual Route findCheapRoute (id_t& to, id_t& from, double lim) = 0;
	virtual Route findShortRoute (id_t& to, id_t& from, double lim) = 0;
};


class Dijkstra: public SearchAlgorithm {
protected:
	vector<Flight*> previous;
	vector<weight_t> opt_route;  
public:
	
	Dijkstra(graph_t& g): SearchAlgorithm(g) {};
	Route findOptRoute (id_t& to, id_t& from, criterion xlessy){
		//std::set<std::pair<weight_t, vertex_t> > vertex_queue; // какая-то странная очередь
		set<Flight> vertex_queue; // какая-то странная очередь :Б  
		//создать путь в самого себя 
		Flight zero;
		vertex_queue.insert(zero); // очередь путей? 

		while (!vertex_queue.empty()){ // пока есть элементы - работаем 
        // получаем вершину 
       	weight_t dist = vertex_queue.begin().get_weight(); 
        id_t usng = vertex_queue.begin().get_to; //где мы сейчас 
        //Flight usng = vertex_queue.begin();
        //убирает первый элемент
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
	const std::vector<Flight> &neighbors = graph[usng]; // получаю множество ребер вершины
        for (std::vector<Flight>::const_iterator neighbor_iter = neighbors.begin(); // пробегаем по векторам 
             neighbor_iter != neighbors.end();
             neighbor_iter++){

            id_t v = neighbor_iter.get_to(); // куда идем 
            
            weight_t weight = neighbor_iter.get_weight();
            weight_t distance_through_u; 
            //сложение вектора весов 
            for (size_t i = 0; i < NUM_RT_PARM; i++ )
            	distance_through_u[i] = dist[i] + weight[i];

	    if ( xlessy(distance_through_u, opt_distance[v]) ) {
	        
	        vertex_queue.erase(neighbor_iter)
	        //(std::make_pair(min_distance[v], v));

	        min_distance[v] = distance_through_u;
	        previous[v] = u; //тут должно быть по идее ребро ыаыаыаыва
	        vertex_queue.insert(std::make_pair(min_distance[v], v));

	    }

        }
    }

	}  

};