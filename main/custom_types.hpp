#ifndef CUSTOM_TYPES_H // include guard
#define CUSTOM_TYPES_H

#include <cstdint>


#define NUM_RT_PARM 3

using id_t = std::uint32_t;//по идеее не меньше 2^32 ), можно и uint32_t
using weight_t = std::uint32_t;  


class weights_t{
protected:
	weight_t weights[NUM_RT_PARM];

public:
	weights_t();
	weights_t(weight_t* w);
	weights_t(const weights_t & ref);
	//тип weights_t::оpеrator[](int i);
	weight_t& operator[] (int i) { return this->weights[i]; };
	const weight_t& operator[](int i) const{ return this->weights[i];};
};

#define SET_WEIGHTS_WEIGHTS(a,b) ({ \
	for(std::size_t i = 0; i < NUM_RT_PARM; i++ ) \
		a[i] = b[i];})
#define SET_WEIGHTS_VAL(a,b) ({ \
	for (std::size_t i = 0; i < NUM_RT_PARM; i++) \
		a[i] = b;})
#define ADD_WEIGHTS(a,b,c) ({ \
	for (std::size_t i = 0; i < NUM_RT_PARM; i++ ) \
		a[i] = b[i] + c[i];})
//#define MAX(a, b) a >= b ? a : b

using criterion = bool (*)(const weights_t &x,const  weights_t &y); 
#endif /* CUSTOM_TYPES_H */