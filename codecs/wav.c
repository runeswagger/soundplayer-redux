#include "../includes/common.h"

struct wav_data {
	int empty; //empty for now
};

int wav_init(sp_module_t *env) {
	env->p.offset = 44; //jump past RIFF header
	//add riff header parsing code, so i can configure the output module to match
	return SP_OK;
}

int wav_deinit(sp_module_t *env) {
	(void) env;
	return SP_OK;
}

int wav_decode(sp_module_t *env) {
	int bytes_in_buffer = env->size;
	int rv = 0; //return value for detecting errors from audio_play
	do {
		//end_of_buffer-bytes_in_buffer is current start of new data
		//play for bytes remaining in the buffer
		//pass through to next module
		sp_data_in(env->next, env->input + env->size - bytes_in_buffer, bytes_in_buffer);
		if(rv > 0){
			bytes_in_buffer -= rv;
		}
		else {
			break;
		}
	} while (bytes_in_buffer > 0);

	return env->size;
}

int wav(sp_module_t *env, sp_operation_t operation){
	//dispatcher for wav
	switch(operation){
		case SPOP_AUTO:
			return wav_decode(env); //TODO: write wav_auto();
		case SPOP_DECODE:
			return wav_decode(env);
		case SPOP_INIT:
			return wav_init(env);
		case SPOP_DEINIT:
			return wav_deinit(env);
		default:
			return SP_NOCODE;
	}
	return SP_ABORT;
}
