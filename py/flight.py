from py.custom_types import DoublyLinkedList 
from py.custom_types import MAX_WEIGHT
from py.custom_types import weights_t
class Flight:
	def __init__(
		self,
		from_city = -1,
		to_city = -1,
		transport_type = -1, 
		cruise_time = MAX_WEIGHT, 
		cruise_fare = MAX_WEIGHT, 
		cruise_locals = MAX_WEIGHT
	):
		if from_city == -1 or to_city == -1 or transport_type == -1:
			self.__set_flag__ = False
		else:
			self.__set_flag__ = True
		self.from_city = from_city
		self.to_city = to_city
		self.transport_type = transport_type
		self.weights =  weights_t(cruise_time, cruise_fare, cruise_locals)
		pass
	def filled(self):
		self.__set_flag__ = True 
	def is_empty(self):
		if self.weights['locals'] == 0:
			return True
		return not self.__set_flag__
	def __str__(self):
		return f"from:{self.from_city}| to: {self.to_city}|{self.weights} |{self.transport_type}"

class Route(Flight):
	def __init__(self, to_city = -1):
		self.from_city = to_city
		self.to_city = to_city
		self.path = DoublyLinkedList()
		self.weights = weights_t()

	def push_front(self, fl):
		self.from_city = fl.from_city
		self.weights = self.weights + fl.weights
		self.path.prepend(fl)
		pass
	def push_back(self, fl):
		self.to_city = fl.to_city
		self.weights = self.weights + fl.weights
		self.path.append(fl)	
		pass