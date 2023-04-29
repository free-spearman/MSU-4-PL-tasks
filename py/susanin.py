from py.graph import Graph
from py.flight import Flight
from py.logger import BaseLogger

def read_graph_ds(filename: str, graph: Graph) -> None:
    ds_f = open(filename, 'r')

    for line in ds_f:
        iter = 0
        cityPart = ''
        from_city_id = 0
        to_city_id = 0
        transport_id = 0
        time = 0
        fare = 0

        for item in line.split(' '):
            if item != '\n':
                if item[0] == '"' and item[-1] != '"':
                    cityPart = item
                    continue

                if cityPart != "":
                    item = cityPart + ' ' + item
                    cityPart = ""

                item = item.replace('"', '')

                if iter == 0:
                    from_city_id = graph.add_city(item)

                elif iter == 1:
                    to_city_id = graph.add_city(item)

                elif iter == 2:
                    transport_id = graph.add_transport(item)

                elif iter == 3:
                    time = int(item)

                elif iter == 4:
                    fare = int(item)
                    graph.add_flight(from_city_id, to_city_id, transport_id, time, fare)
                iter += 1

