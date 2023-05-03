from math import fabs
from py.flight import Flight

#ИМЕНА
#location = list[]std::vector<Flight>;
MAX_ID = 2**32
MAX_WEIGHT = 2**32
'''/*class Location{
protected:
	id_t id;
	vector<Flight> flights; //нужен лю using?
public:
 
	Location(id_t id);
	~Location();
};
*/
'''

class Graph:
	def __init__(self):
		# [string, id]
		self.citys = {}
		#<id_t, std::string>
		self.citys_names = {}
		#<std::string, id_t>
		self.transports_name_id = {}
		#<id_t, std::string>
		self.transports_id_name = {}
		#vector<Flight>
		self.flights = {}
		self.num_citys = 0
		self.num_type_transports = 0
	@property
	def num_citys_upd(self):
		""" id_t getNumCitys() const;"""
		self.num_citys = len(self.citys)
		return self.num_citys  
		pass	
	@property
	def num_type_transports_upd(self):
		self.num_type_transports = len(self.transports_name_id)
		return self.num_type_transports  
	
	def size(self):
		"""id_t size() const;"""
		return self.num_citys_upd
	def __getitem__(self, index: int):
		"""location& operator[] (int i)"""
		if fabs(index) > self.num_citys:
			raise ValueError( f"index:{index} > MAX_ID:{MAX_ID}")
		poss = index if index >= 0 else (self.num_citys + index)   
		return self.flights[poss]
		pass
	def add_city(self, name: str):	
		"""id_t addCity(const std::string name);"""
		if not name: 
			raise ValueError( "empty name")
		if name in self.citys_names:
			return self.citys_names[name]
		nums  = self.num_citys
		self.citys[nums] = name
		self.citys_names[name] = nums 
		self.flights[nums] = []
		self.num_citys = self.num_citys + 1
		return nums
	def add_transport(self, name: str):
		"""id_t addTransport(const std::string name);"""
		if not name: 
			raise ValueError( "empty name")
		if name in self.transports_name_id:
			return self.transports_name_id[name]
		nums = self.num_type_transports
		self.transports_name_id[name] = nums
		self.transports_id_name[nums] = name 
		self.num_type_transports = self.num_type_transports + 1 
		return nums

	def add_flight(self, 
		from_city: int, to_city: int,
		transport_type: int, 
		time = MAX_WEIGHT, 
		fare = MAX_WEIGHT, 
		locals_w = 1
		):
		"""	void addFlight(id_t from,
		id_t to, 
		id_t transport_type, 
		weight_t time, 
		weight_t fare, 
		weight_t locals
		);
		"""
		if self.num_citys < from_city or self.num_citys < to_city: 
			raise ValueError("there is no such city")
		if self.num_type_transports < transport_type:
			raise ValueError("incorrect transport type")
		self.flights[from_city].append(Flight(from_city, to_city, transport_type, time, fare, locals_w))

	def find_city_by_name(self, name: str):
		"""id_t find_city_by_name(const std::string name);"""
		if name in self.citys_names:
			return self.citys_names[name]
		return -1
		raise ValueError("there is no such city")
	def find_transport_by_name(self, name: str):
		"""id_t findTransportByName(const std::string name);"""
		if name in self.transports_name_id:
			return self.transports_name_id[name]
		return -1
		raise ValueError("there is no such transport")
	def find_city_by_id(self, id_city: int):
		"""std::string findCityById(const id_t id);"""
		if id_city < self.num_citys:
			return self.citys[id_city]
		return -1
		raise ValueError("there is no such city")
		pass
	def find_transport_by_id(self, id_tr: int):
		"""std::string findTransportById(const id_t id);"""
		if id_tr < self.num_type_transports:
			return self.transports_id_name[id_tr]
		raise ValueError("there is no such Transport By Id")
		pass
	def flight_to_string(self, flight: Flight):
		"""std::string flightToString(const Flight& flight);"""
		return f"{self.find_city_by_id(flight.from_city)} -> {self.find_city_by_id(flight.to_city)}\
		{self.find_transport_by_id(flight.transport_type)} {flight.weights}"
		pass
	def printInFile(self, out):
		"""void printInFile(std::ofstream& out);"""
		pass
	



#endif /* GRAPH_H */
