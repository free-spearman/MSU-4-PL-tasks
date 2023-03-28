#include "SearchAlgorithms.hpp"

SearchAlgorithm::SearchAlgorithm(graph_t *g){
	this->max_weight =  std::numeric_limits<weight_t>::max();
	this->graph = g;
};

SearchAlgorithm::SearchAlgorithm(graph_t *g, id_t to, id_t from):SearchAlgorithm(g){
	this->to = to;
	this->from = from;
};

Dijkstra::Dijkstra(graph_t* g): SearchAlgorithm(g) {};

//нужно добавить location
vector<Flight*> Dijkstra::calcOptRoute (id_t& from, id_t& to, weights_t lims,  criterion xlessy){
	// отчистить opt_route
	this->previous.clear(); // прошлого нет
	this->previous.resize(this->graph->size(), NULL); // не более чем количество вершин
    	this->opt_distance.clear();
    	
    	this->opt_distance.resize( this->graph->size());
    	for (vector<weights_t>::iterator itr = this->opt_distance.begin(); itr != this->opt_distance.end(); itr++) {
    		SET_WEIGHTS_VAL((*itr), numeric_limits<weight_t>::max());
    		/*for (size_t v = 0; v < NUM_RT_PARM; v++ )
    			(*itr)[v] = numeric_limits<weight_t>::max();
    		*/
    	} 

  	
  	struct cust_compare {
		bool operator()(const pair<weights_t, id_t>& p1,
                    const pair<weights_t, id_t>& p2){
			if ( p1.second == p2.second)
				return x_fast_cheaper_y(p1.first, p2.first);  
			return p1.second < p2.second;
		}
	};

	set<pair<weights_t, id_t>, cust_compare> vertex_queue; // какая-то странная очередь :Б  
		
	//создать путь в самого себя
	SET_WEIGHTS_VAL(this->opt_distance[from], 0);
	/*for (size_t i = 0; i < NUM_RT_PARM; i++ )
		this->opt_distance[from][i] = 0;
	*/
	vertex_queue.insert( make_pair( this->opt_distance[from], from) ); // очередь путей? 

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
		const vector<Flight*> &neighbors = (*(this->graph))[act_vertx]; // получаю множество ребер вершины
        
        		for (vector<Flight*>::const_iterator neighbor_iter = neighbors.begin(); // пробегаем по векторам 
          	   neighbor_iter != neighbors.end();
          	   neighbor_iter++){

          		id_t to_vertx = (*neighbor_iter)->get_to(); // куда идем 
            		
            		weights_t to_weights;
            		(*neighbor_iter)->get_weights(to_weights);        
          		weights_t distance_through_act; 
           	//сложение вектора весов 
          		ADD_WEIGHTS(distance_through_act, act_dist, to_weights);
          		/*for (size_t i = 0; i < NUM_RT_PARM; i++ )
            			distance_through_act[i] = act_dist[i] + to_weight[i];
				*/
	    			if ( xlessy(distance_through_act, this->opt_distance[to_vertx]) ) {
	    				vertex_queue.erase(make_pair(this->opt_distance[to_vertx],to_vertx));
	    			//(std::make_pair(min_distance[v], v));
	     			SET_WEIGHTS_WEIGHTS (this->opt_distance[to_vertx], distance_through_act);
					
					this->previous[to_vertx] = *neighbor_iter; //тут должно быть по идее ребро ыаыаыаыва
					
					vertex_queue.insert(make_pair(this->opt_distance[to_vertx], to_vertx));
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
			if (this->previous[act] == NULL){
				throw std::runtime_error("no previous element.");
			}
			path->push_front( *(this->previous[act]));
			act = this->previous[act]->get_from();
		}
	return path;
}

Route* Dijkstra::findFastRoute (id_t& from, id_t& to, weight_t lim){
	weights_t lims;
	SET_WEIGHTS_VAL(lims, 0);
	lims[CRUISE_TIME_P] = lim;
	this->calcOptRoute(from, to, lims, x_faster_y);

	return this->restoreRoute(to);
};
Route* Dijkstra::findCheapRoute (id_t& from, id_t& to, weight_t lim){
	return new Route(from);
};
Route* Dijkstra::findShortRoute (id_t& from, id_t& to, weight_t lim) {
	return new Route(from);
};
