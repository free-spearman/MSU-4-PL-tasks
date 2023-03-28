#ifndef DATAREADER_H // include guard DataReader 
#define DATAREADER_H

#include <sstream>
#include <fstream>
#include "Graph.hpp"

std::string format_string(const std::string &input);
void handle_file_input(Graph &graph, const char *filename);

#endif //DATAREADER_H