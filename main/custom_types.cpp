#include "custom_types.hpp"

weights_t::weights_t(weight_t w){
	SET_WEIGHTS_VAL(this->weights, w);	
};
weights_t::weights_t(){
	SET_WEIGHTS_VAL(this->weights, 0);
};
weights_t::weights_t(weight_t* w){
	SET_WEIGHTS_WEIGHTS(this->weights, w);
};
weights_t::weights_t(const weights_t & ref){
	SET_WEIGHTS_WEIGHTS(this->weights, ref.weights);	
};
