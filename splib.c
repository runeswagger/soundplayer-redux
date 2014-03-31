/* Soundplayer Supplemental Functions */
#include <stdlib.h>
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

sp_module_t* sp_module_new(sp_type_t format){
	//allocate a module
	//TODO: set up the dispatcher callback
	sp_module_t *module = calloc(1, sizeof(sp_module_t));
	
	if (module == NULL){ return NULL; }
	(void)format;
	
	return module;
}

void sp_module_destroy(sp_module_t* module){
	//release all memory held by a module
	module->dispatcher(module, SPOP_DEINIT);
	
	free(module); //deallocate the struct
	
	module = NULL; //set it to NULL
}
