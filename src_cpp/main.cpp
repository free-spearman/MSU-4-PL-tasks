#include "custom_types.hpp"
#include "Graph.hpp"
#include "Flight.hpp"
//#include "DataReader.hpp"
#include "Susanin.hpp"
/*class Susanin
{test_input.txt
public:
	Susanin();
	int loop(){

	}
	~Susanin();
	
};
*/


int main(int argc, char const *argv[])
{	
	Susanin gui;
	//я пока не сделал обработку акс
	//так что файл по умолчанию для графа test_input.txt 
	gui.beginLogMess();
	gui.loop();
	/*Graph G = Graph();
	char name[] = "test_input.txt";
	handle_file_input(G, name);
	std::ofstream out;
	out.open("result.txt");
	G.printInFile(out);
	*/	
	return 0;
}