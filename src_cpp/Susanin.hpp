#ifndef SUSANIN_H // include guard
#define SUSANIN_H

#include <sys/time.h>
#include <sys/resource.h>
#include <curses.h>
#include <ncurses.h>
#include <locale>
#include <chrono>
#include <sstream>

#include "BaseLogger.hpp"
#include "custom_types.hpp"

#include "Graph.hpp"

#include "SearchAlgorithms.hpp"

#define ctrl(x) (x & 0x1F)
#define EXIT_APP "EXIT_APP"
#define TITLE_STEP 2

float estimate_time(const std::chrono::steady_clock::duration &start_time_duration);

class Susanin: public BaseLogger 
{
protected:
	Graph graph;
	std::string ds_file_name;

	int modeInputHandler();
	std::string intInputhandler();
	std::string stringInputHandler(int row);
	int finishAppHandler();
	bool addRestrictedTransportHandler( SearchAlgorithm& s);
	id_t getCityHandler(Graph &graph, bool to_city);
	std::string formatString(const std::string &input);
	void beginLoop();
	void endLoop();
	void clear_n_lines(int start, int end){
    	for (int i = start; i <= end; i++){
        	move(i, 0);
        	clrtoeol();
    	}
	};
	void wait_for_enter(int start){
	    curs_set(0);
	    attron(COLOR_PAIR(3));
	    mvprintw(start, 0, "Нажмите ENTER чтобы продолжить");
	    attroff(COLOR_PAIR(3));
	    move(start + 1, 0);
	    char c;
	    while ((c = getch()) != 10){}
	}


public:
	Susanin();
	void readGraph(Graph &graph);

	int loop();
	~Susanin();
};

#endif /* SUSANIN_H */