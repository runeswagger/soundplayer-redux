/* Soundplayer Supplemental Functions */
#include "includes/common.h"

inline int sp_module_init(sp_module_t *module){
	//invoking modules is a little unsightly
	return module->dispatcher(module, SPOP_INIT);
}

inline int sp_module_deinit(sp_module_t *module){
	//another aesthetic whim
	return module->dispatcher(module, SPOP_DEINIT);
}

inline int sp_data_in(sp_module_t *module, char* buffer, unsigned long len){
	//passing data to a module has become
	//three times more lengthy since the api change
	module->input = buffer;
	module->size = len;
	return module->dispatcher(module, SPOP_AUTO);
}
	
