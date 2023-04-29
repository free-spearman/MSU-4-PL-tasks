from py.graph import Graph
from py.flight import Flight
from py.logger import BaseLogger
from py.susanin import read_graph_ds

def main():
	input_name = "big_test"
	graph = Graph()
	logger = BaseLogger("log.txt","MAIN")
	#logger.log("\n\nBEGIN\n\n", "main")
	logger.start_log("read_graph_ds")
	read_graph_ds(input_name, graph)
	logger.end_log()
	print('begin')
	for city in graph.flights:
		for flt in graph.flights[city]:
			print(graph.flight_to_string(flt))
		print() 
if __name__ =="__main__":
	main()
