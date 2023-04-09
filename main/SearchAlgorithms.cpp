#include "SearchAlgorithms.hpp"

//  функции для сравнения 
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



SearchAlgorithm::SearchAlgorithm(){
	this->lims = weights_t(std::numeric_limits<weight_t>::max());
}; 

SearchAlgorithm::~SearchAlgorithm(){};

SearchAlgorithm::SearchAlgorithm(Graph& g):SearchAlgorithm(){
	this->max_weight =  std::numeric_limits<weight_t>::max();
	this->graph = g;
};

bool SearchAlgorithm::addAvableTransport(id_t t_id){
	this->graph.findTransportById(t_id);
	this->avable_transport.insert(t_id);
	return true;
};


/* Блок проверок*/
bool SearchAlgorithm::checkTransportRestrictions(id_t t){
	//если пустой - то все, если не пустой, то ищем вхождение
	if (this->avable_transport.size() != 0 
		&& this->avable_transport.find(t) != this->avable_transport.end()){
		return false;
	}
	return true;
}


//проверка на тип трансопрта + ограничения на веса 
bool SearchAlgorithm::checkWeightRestrictions(weights_t& w){
	//по умолчанию в lims std::numeric_limits<weight_t>::max() 
	for(id_t i = 0; i < NUM_RT_PARM; i++)
		if (w[i] > this->lims[i])
			return false;
	return true;
}


/* Блок проверок*/

SearchAlgorithm::SearchAlgorithm(Graph& g, id_t to, id_t from):SearchAlgorithm(g){
	this->to = to;
	this->from = from;
};

Dijkstra::Dijkstra(): SearchAlgorithm(){};
Dijkstra::Dijkstra(Graph& g): SearchAlgorithm(g) {};


//нужно добавить location
std::vector<Flight> Dijkstra::calcOptRoute (id_t& from, id_t& to, criterion xlessy){
	// отчистить opt_route
	this->previous.clear(); // прошлого нет
	this->previous.resize(this->graph.size()); // не более чем количество вершин
    this->opt_distance.clear();
    	
    this->opt_distance.resize( this->graph.size());

    for (std::vector<weights_t>::iterator itr = this->opt_distance.begin(); itr != this->opt_distance.end(); itr++) {
    	SET_WEIGHTS_VAL((*itr), std::numeric_limits<weight_t>::max());
    		/*for (size_t v = 0; v < NUM_RT_PARM; v++ )
    			(*itr)[v] = numeric_limits<weight_t>::max();
    		*/
    	} 

  	
  	struct cust_compare {
		bool operator()(const std::pair<weights_t, id_t>& p1,
                    const std::pair<weights_t, id_t>& p2){
			if ( p1.second == p2.second)
				return x_fast_cheaper_y(p1.first, p2.first);  
			return p1.second < p2.second;
		}
	};

	std::set<std::pair<weights_t, id_t>, cust_compare> vertex_queue; // какая-то странная очередь :Б  
		
	//создать путь в самого себя
	SET_WEIGHTS_VAL(this->opt_distance[from], 0);
	/*for (size_t i = 0; i < NUM_RT_PARM; i++ )
		this->opt_distance[from][i] = 0;
	*/
	vertex_queue.insert( std::make_pair( this->opt_distance[from], from) ); // очередь путей? 

	while (!vertex_queue.empty()){ // пока есть элементы - работаем 
      	// получаем вершину
     	weights_t act_dist = vertex_queue.begin()->first;
     	//SET_WEIGHTS_WEIGHTS(act_dist, vertex_queue.begin()->first);
     	/*for (size_t i = 0; i < NUM_RT_PARM; i++ )
     		act_dist[i] = vertex_queue.begin()->first[i];
     	*/
     	id_t act_vertx = vertex_queue.begin()->second; //где мы сейчас 
     	
     	//Flight usng = vertex_queue.begin();
     	//убирает первый элемент
        vertex_queue.erase(vertex_queue.begin());

        	// Visit each edge exiting u
        	// тут может быть ошибка !EE
		location &neighbors = this->graph[act_vertx]; // получаю множество ребер вершины
        
        	for (location::iterator neighbor_iter = neighbors.begin(); // пробегаем по векторам 
          		neighbor_iter != neighbors.end();
          		neighbor_iter++){

          		id_t to_vertx = neighbor_iter->get_to(); // куда идем 
            		
            	weights_t to_weights;
            	neighbor_iter->get_weights(to_weights);        
          		weights_t distance_through_act; 
           		//сложение вектора весов 
          		ADD_WEIGHTS(distance_through_act, act_dist, to_weights);
          		/*for (size_t i = 0; i < NUM_RT_PARM; i++ )
            			distance_through_act[i] = act_dist[i] + to_weight[i];
				*/

	    		if (this->checkWeightRestrictions(distance_through_act)
	    			&& this->checkTransportRestrictions(neighbor_iter->get_transport())    
	    			&& xlessy(distance_through_act, this->opt_distance[to_vertx])
	    			) {

	    			vertex_queue.erase(std::make_pair(this->opt_distance[to_vertx],to_vertx));
	    			//(std::make_pair(min_distance[v], v));
	     			SET_WEIGHTS_WEIGHTS (this->opt_distance[to_vertx], distance_through_act);
					
					this->previous[to_vertx] = *neighbor_iter; //тут должно быть по идее ребро ыаыаыаыва
					
					vertex_queue.insert(std::make_pair(this->opt_distance[to_vertx], to_vertx));
				}
			}
		}
	this->setting_f = true; 
	return this->previous;
};
	
Route Dijkstra::restoreRoute(const id_t &to){
	if (this->previous.size() == 0){
		// пустой путь

		throw std::runtime_error("previous.size() == 0");
		}
	Route path = Route(to);
	id_t act = to;
	while( act != this->from ){
			if ( this->previous[act].isEmpty()){
				throw std::runtime_error("no previous element.");
			}
			path.push_front(this->previous[act]);
			act = this->previous[act].get_from();
		}
	return path;
}

Route Dijkstra::findFastRoute (id_t& from, id_t& to){
	//weights_t lims;
	//SET_WEIGHTS_VAL(lims, std::numeric_limits<weight_t>::max());
	//lims[CRUISE_TIME_P] = lim;
	if (from == to){
		throw std::domain_error("from = to");
	}
	this->calcOptRoute(from, to, x_faster_y);
	return this->restoreRoute(to);
	
};
Route Dijkstra::findCheapRoute (id_t& from, id_t& to){
	return Route(from);
};
Route Dijkstra::findShortRoute (id_t& from, id_t& to) {
	return Route(from);
};
