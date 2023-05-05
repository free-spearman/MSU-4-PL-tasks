import heapq
from py.custom_types import MAX_WEIGHT
from py.custom_types import weights_t
from py.graph import Graph
from py.flight import Flight
from py.flight import Route
from py.logger import BaseLogger

MAX_DEPTH_2 = 32

def x_fast_cheaper_y(w: weights_t):
    return w['time']<<MAX_DEPTH_2 | w['fare'] 
def x_fast_y(w: weights_t):
    return w['time']
def x_short_y(w: weights_t):
    return w['locals']
def x_cheap_y(w: weights_t):
    return w['fare']


class SearchAlgorithm:
    def __init__(self, graph: Graph, logger: BaseLogger):
        self.graph = graph
        self.lims = weights_t(MAX_WEIGHT, MAX_WEIGHT, MAX_WEIGHT)
        self.prohibited_transport = set()
        self.logger = logger
        pass
    def reset_restrictions_transport(self):
        """resetRestrictionsTransport"""
        self.prohibited_transport.clear()
        pass
    def get_prohibited_transport(self) -> str:
        """std::string SearchAlgorithm::getProhibitedTransport()"""
        if len(self.prohibited_transport) == 0:
            return "Нет ограничений"
        tr = []
        for t_id in self.prohibited_transport:
            tr.append(self.graph.find_transport_by_id(t_id))
        return ";".join(tr)
    def set_limits(self, limits: weights_t):
        self.lims = limits
        pass
    def find_fast_route(self,):
        pass 
    def find_cheap_route(self):
        pass
    def find_short_route(self):    
        pass
    def add_restrictions_transport(self, tr):
        """addRestrictionsTransport"""
        res = 0
        if type(tr) == str:
            res = self.graph.find_transport_by_name(tr)
            #self.logger.log(str(res), "type(tr) == str")
            #self.logger.log("\n\n\n", "\n\n\n")
            #self.logger.log(self.graph.transports_name_id.__str__()+'\n', 'transports_name_id list')
            if res == -1:
                return False
        self.prohibited_transport.add(res)
        return True 
    def check_transport_restrictions(self, t :int)-> bool:
        """checkTransportRestrictions"""
        if t in self.prohibited_transport:
            return False
        return True
        pass
    def check_weight_restrictions(self, dist, estimator):
        """bool checkWeightRestrictions(weights_t& w):"""
        """
        for lim_name in self.lims.__weights__:
            if w[lim_name] > self.lims[lim_name]:
                return False
        return True
        """
        lim = estimator(self.lims)
        if lim < dist:
        	return False
        else:
        	return True

class Dijkstra(SearchAlgorithm):
    def __init__(self, graph: Graph, logger):
        super().__init__(graph, logger)
    def calc_opt_route(self, from_city, to_city, estimator):

        max_dist = estimator(weights_t(MAX_WEIGHT,MAX_WEIGHT,MAX_WEIGHT))
        #id:Flight
        self.prev_vert = {}
        self.opt_distances = {vertex: max_dist for vertex in self.graph.flights}
        self.opt_distances[from_city] = 0
        pq = [(0, from_city)]
        while len(pq):
            current_distance, current_vertex = heapq.heappop(pq)
            if current_distance > self.opt_distances[current_vertex]:
                continue
            for fl in self.graph[current_vertex]:
                if not self.check_transport_restrictions(fl.transport_type):
                    continue
                neighbor, weight = fl.to_city, estimator(fl.weights)
                distance = current_distance + weight
                if distance < self.opt_distances[neighbor]:
                    self.opt_distances[neighbor] = distance
                    self.prev_vert[neighbor] = fl
                    heapq.heappush(pq, (distance, neighbor))
    def restore_route(self, from_city, to_city):
        if to_city not in self.prev_vert:
            return -1
        path = Route(to_city)
        act = to_city
        while act != from_city:
            if self.prev_vert[act].is_empty():
                return -1
            path.push_front(self.prev_vert[act])
            act = self.prev_vert[act].from_city
        path.filled()
        return path
    def find_fast_cheaper_route(self, from_city, to_city):
        self.calc_opt_route(from_city, to_city, x_fast_cheaper_y)
        return self.restore_route(from_city, to_city)
    def find_cheaper_route(self, from_city, to_city):
        self.calc_opt_route(from_city, to_city, x_cheap_y)
        return self.restore_route(from_city, to_city)
    def find_short_route(self, from_city, to_city):
        self.calc_opt_route(from_city, to_city, x_short_y)
        return self.restore_route(from_city, to_city)
    def find_set_cities_lim_time(self, from_city):
    	self.calc_opt_route(from_city, -1, x_fast_y)
    	achiev_cities = []
    	for vtx in self.prev_vert:
    		flt = self.prev_vert[vtx]
    		if flt.is_empty():
    			continue
    		if not self.check_weight_restrictions(self.opt_distances[flt.to_city], x_fast_y):
    			continue
    		temp = self.restore_route(from_city ,flt.to_city)
    		achiev_cities.append(temp)
    	return achiev_cities
    def find_set_cities_lim_cost(self, from_city):
    	self.calc_opt_route(from_city, -1, x_cheap_y)
    	achiev_cities = []
    	for vtx in self.prev_vert:
    		flt = self.prev_vert[vtx]
    		if flt.is_empty():
    			continue
    		if not self.check_weight_restrictions(self.opt_distances[flt.to_city], x_cheap_y):
    			continue
    		temp = self.restore_route(from_city ,flt.to_city)
    		achiev_cities.append(temp)
    	return achiev_cities