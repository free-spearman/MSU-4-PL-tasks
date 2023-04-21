#include "BaseLogger.hpp"

BaseLogger::BaseLogger(){
	this->log_name = LOGNAME;
	this->class_name = "not set";
}
BaseLogger::BaseLogger(const char *class_name){
	this->log_name = LOGNAME;
	this->class_name = class_name;	
};
BaseLogger::BaseLogger(const char *class_name, const char *log_name){
	this->class_name = class_name;
	this->log_name = log_name;
};

int BaseLogger::log(const char *out_sring,const char *operation){
	std::ofstream out;
	out.open(this->log_name, std::ios::app);
	out<<this->class_name<<"::" <<operation<<":";
	std::time_t result = std::time(nullptr);
    out<<std::ctime(&result);	
	out<<out_sring<<std::endl;
	out.close();
	return 0;
};

int BaseLogger::log_const(const char *out_sring,const char *operation) const{
	std::ofstream out;
	out.open(this->log_name, std::ios::app);
	out<<this->class_name<<"::" <<operation<<":";
	std::time_t result = std::time(nullptr);
    out<<std::ctime(&result);	
	out<<out_sring<<std::endl;
	out.close();
	return 0;
};


ProcessLogger::ProcessLogger():BaseLogger(typeid(this).name()){};

ProcessLogger::ProcessLogger(const char *class_name):BaseLogger(class_name){};
ProcessLogger::ProcessLogger(const char *class_name, const char *log_name):BaseLogger(class_name, log_name){};

ProcessLogger::ProcessLogger(const ProcessLogger& ref){
	this->log_name = ref.log_name;
	this->class_name = ref.class_name;
	this->start_times = ref.start_times;
	this->call_stack = ref.call_stack;
};

ProcessLogger& ProcessLogger::operator= (const ProcessLogger& ref){
	this->log_name = ref.log_name;
	this->class_name = ref.class_name;
	this->start_times = ref.start_times;
	this->call_stack = ref.call_stack;
	return *this; 
};

int ProcessLogger::openLog(){
	//проверка на открытие
	this->out.open(this->log_name, std::ios::app);
	return 0;
};

int ProcessLogger::closeLog() {
	this->out.close();
	return 0;
};

void ProcessLogger::logTimeLabel() {
	std::time_t result = std::time(nullptr);
    this->out<<std::ctime(&result);
};

int ProcessLogger::log(const char *out_sring,const char *operation){
	this->openLog(); 
	this->out<<this->class_name<<"::" <<operation<<":";
	this->logTimeLabel();
	
	this->out<<out_sring<<std::endl;
	this->closeLog();
	return 0;
};


hrc_lock_t ProcessLogger::getTime(){
	return std::chrono::high_resolution_clock::now();
};


/*std::string timeToStr(hrc_lock_t& tm){
	char mbstr[50];
	
	std::strftime(mbstr, sizeof(mbstr), "%F %T.\n", std::localtime(&t_c));
	std::string stime = mbstr;
	return stim  
};
*/


int ProcessLogger::beginLogMess(){
	this->openLog(); 
	this->out<<"\n\n\n"<<"BEGIN LOG:";
	this->logTimeLabel();
	this->out<<std::endl;
	this->closeLog();
	return 0;
};

int ProcessLogger::startLog(const char *operation){
	std::string s = operation;
	this->call_stack.push(s);
	this->start_times.push(this->getTime());
	return 0;
};

int ProcessLogger::endLog(){
	
	this->openLog();
	struct rusage usage;

	if (this->start_times.empty()){
		this->out <<"start_times empty";
		this->logTimeLabel();
		this->out<<std::endl;
		this->closeLog();
		return 1;
	}

	auto start_time = this->start_times.top().time_since_epoch();
	this->start_times.pop();

	auto end_time = this->getTime().time_since_epoch();
	auto time = end_time - start_time; 
	double total_time = std::chrono::duration<double>(time).count();
	
	

	this->out<<this->class_name<<"::" <<this->call_stack.top()<<":";
	this->logTimeLabel();
	
	this->out<<"call:"<< this->call_stack.top()<<" time:"<<total_time;
	this->call_stack.pop();
	if (0 == getrusage(RUSAGE_SELF, &usage)){
        this->out <<" maxrss:"<<usage.ru_maxrss;
    }
    else{
		this->out <<" maxrss:" << '0';
    }
    this->out<<std::endl;
	this->closeLog();
	return 0; 
};
