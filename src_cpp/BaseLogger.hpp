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

#define LOGNAME "log.txt"
/*

*/

using hrc_lock_t = std::chrono::time_point<std::chrono::high_resolution_clock>;


class BaseLogger{
protected:
	std::string log_name;
	std::string class_name;	
	//std::ofstream out;
public:
	BaseLogger();
	BaseLogger(const char *class_name);
	BaseLogger(const char *class_name,const char *log_name);
	//BaseLogger(const BaseLogger& ref);
	//BaseLogger& operator= (const BaseLogger& ref);
	
	//typeid(myint).name()
	int log(const char *out_sring,const char *operation);
	int log_const(const char *out_sring,const char *operation) const;
};

class ProcessLogger: public BaseLogger{
protected:
	//std::set<std::string> list_log;
	std::ofstream out;
	std::stack<hrc_lock_t> start_times; 
	std::stack<std::string> call_stack;
	
public:
	ProcessLogger();
	ProcessLogger(const char *class_name);
	ProcessLogger(const char *class_name,const char *log_name);
	ProcessLogger(const ProcessLogger& ref);

	ProcessLogger& operator= (const ProcessLogger& ref);

	int openLog();
	int closeLog();
	void logTimeLabel();


	hrc_lock_t getTime();
	
	int beginLogMess();
	
	int log(const char *out_sring,const char *operation);
	int startLog(const char * operation);
	int endLog();
	//int end();
};


#endif //loger