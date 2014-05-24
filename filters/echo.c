#include <stdlib.h>
#include "../includes/common.h"
#include "../includes/splib.h"
 
struct echo_data {
	//persistant data structures for echo filter
	short *buffer; //current input
	short *previous; //average this data in
	unsigned int sample_window; //number of samples to echo
	unsigned int limit; //maximum chunksize
};

int echo_configure(sp_module_t *env){
	//start configureing the module
	(void) env;
	return SP_OK;
}

int echo_auto(sp_module_t *env){
	//do the auto stuff
	if(!sp_flag_isset(env, SP_AUTO))
		//must be in auto mode
		return SP_ABORT;
	if(!sp_flag_isset(env, SP_INITIALIZED)){
		//initialize the module
		if(sp_module_init(env)){
			sp_flag_set(env, SP_INITIALIZED);
		}
	}
	
	return SP_OK;
}
		
int echo_init(sp_module_t *env){
	struct echo_data *data = malloc(sizeof(struct echo_data));
	//allocate some memory, we can't trust that env->size is sane right now
	data->sample_window = 4096;
	data->buffer = malloc(4096);
	data->previous = malloc(data->sample_window);

	if(data->buffer == NULL || data->previous == NULL) return SP_ABORT;

	env->data = data;
	return SP_OK;
}

int echo_decode(sp_module_t *env){
	//average the previous buffer with this one cumulatively
	struct echo_data *data = env->data;
	int bytes_consumed = 0; //since filter can only be applied in sample window_increments
	
	//adjust buffers as needed
	//we need to be smart here, at the very least we can't
	//realloc when env->size is zero, since it will free(data->buffer)
	if(env->size != 0){		
		data->buffer = realloc(data->buffer, env->size);
	}
	//sample window will eventually be runtime configurable
	//but for now this statement is ineffective
	//data->previous = realloc(data->previous, data->sample_window);
	int x;
	//FIXME: env->size is larger than data->sample_window ...i'm reading bad memory
	for(x=env->size; x>=0 ; x--) {
		data->buffer[x] = (data->previous[x]>>1) + (data->buffer[x]>>1);
		data->previous[x] = data->buffer[x];
	}
	sp_data_in(env->next, (char*)data->buffer, env->size);
	
	return env->size;
}

int echo_deinit(sp_module_t *env){
	//destroy buffers
	struct echo_data *data = env->data;
	free(data->buffer);
	data->buffer = NULL;
	free(data->previous);
	data->previous = NULL;
	free(data);
	return SP_OK;
}

int echo(sp_module_t *module, sp_operation_t operation){
	switch(operation){
		case SPOP_AUTO:
			return echo_decode(module);
		case SPOP_DECODE:
			return echo_decode(module);
		case SPOP_INIT:
			return echo_init(module);
		case SPOP_DEINIT:
			return echo_deinit(module);
		case SPOP_CONFIGURE:
			return echo_configure(module);
		default:
			return SP_NOCODE;
	}
	return SP_ABORT;
}
