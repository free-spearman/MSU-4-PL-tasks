from py.graph import Graph
from py.flight import Flight
from py.logger import BaseLogger
from py.susanin import Susanin
import curses as crs
import argparse as arg

parser = arg.ArgumentParser()
parser.add_argument('file', help='path to file to be processed')

def main(stdscr):
	args = parser.parse_args()
	name = args.file.split('=')[1]
	input_name = name 
	#"input_small.txt"
	#input_name = "25_cubes.txt"#
	#graph = Graph()
	#logger = BaseLogger("log.txt","MAIN") 
	#logger.log("\n\nBEGIN\n\n", "main")
	s = Susanin(input_name, stdscr)
	s.loop()
	#logger.start_log("read_graph_ds")
	#read_graph_ds(input_name, graph)
	#logger.end_log()
	#print('begin')
	"""for city in graph.flights:
		for flt in graph.flights[city]:
			print(graph.flight_to_string(flt))
		print()
	""" 
if __name__ =="__main__":
	
	#print(name)
	crs.wrapper(main)
