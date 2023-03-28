#include "DataReader.hpp"

std::string format_string(const std::string &input)
{

    std::string result = "";
    for (const auto &c : input)
    {
        if (c != '"')
        {
            result += c;
        }
    }
    return result;
}

void handle_file_input(Graph &graph, const char *filename){
    std::string result;

    std::ifstream input(filename);

    while (getline(input, result)){
        std::string item;
        std::string cityPart;
        std::stringstream ss(result);
        std::size_t iter = 0;

        id_t from_city_id;
        id_t to_city_id;
        id_t transport;
        weight_t time;
        weight_t fare;

        while (getline(ss, item, ' ')){
            if (item != "\n"){

                if (item[0] == '"' && item.back() != '"'){
                    cityPart = item;
                    continue;
                }
                if (cityPart != ""){
                    item = cityPart + ' ' + item;
                    cityPart = "";
                }
                // const auto arr = item.c_str();
                // item.erase(remove(arr.begin(), arr.end(), '"'), arr.end());
                item = format_string(item);

                switch (iter){
                case 0:
                    from_city_id = graph.AddCity(item);
                    break;
                case 1:
                    to_city_id = graph.AddCity(item);
                    break;
                case 2:
                    transport = graph.AddTransport(item);
                    break;
                case 3:
                    time = stoi(item);
                    break; 
                case 4:
                    fare = stoi(item);
                    graph.AddFlight(from_city_id, to_city_id, transport, time, fare, 1);
                    break;
                }
                iter++;
            }
        }
    } 

    input.close();
}