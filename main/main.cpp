#include "custom_types.hpp"
#include "Graph.hpp"
#include "Flight.hpp"
#include "DataReader.hpp"

/*class Susanin
{
public:
	Susanin();
	int loop(){

	}
	~Susanin();
	
};
*/


int main(int argc, char const *argv[])
{	
	Graph G = Graph();
	char name[] = "test_input.txt";
	handle_file_input(G, name);
	std::ofstream out;
	out.open("result.txt");
	G.printInFile(out);	
	return 0;
}