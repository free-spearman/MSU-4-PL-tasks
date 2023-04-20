#ifndef BASELOGGER_H // include guard
#define BASELOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/resource.h>
#include <ctime>
#include <chrono>

#include <string>
#include <stack>
#include <set>

#include <typeinfo>


/*

*/
using hrc_lock_t = std::chrono::time_point<std::chrono::high_resolution_clock>;


class BaseLogger{
protected:
	std::string log_name;
	std::string class_name;	
	std::ofstream out;
	std::set<std::string> list_log;
	std::stack<hrc_lock_t> start_times; 
	std::stack<std::string> call_stack;
	
public:
	BaseLogger();
	BaseLogger(const char *class_name);
	BaseLogger(const char *class_name,const char *log_name);
	
	
	int openLog();
	int closeLog();
	
	hrc_lock_t getTime();
	//	std::string timeToStr(time_t& tm);
	void logTimeLabel();
	//typeid(myint).name()
	int beginLogMess();
	int log(const char *out_sring,const char *operation);
	int startLog(const char * operation);
	int endLog();
	//int end();
};

#endif //loger