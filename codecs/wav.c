#include "../includes/common.h"

struct wav_data {
	int empty; //empty for now
};

int wav_init(struct sp *env) {
	env->p.offset = 44; //jump past RIFF header
	//add riff header parsing code, so i can configure the output module to match
	return SP_OK;
}

int wav_deinit(struct sp *env) {
	(void) env;
	return SP_OK;
}

int wav_decode(struct sp *env) {
	int bytes_in_buffer = env->size;
	int rv = 0; //return value for detecting errors from audio_play
	do {
		//end_of_buffer-bytes_in_buffer is current start of new data
		//play for bytes remaining in the buffer
		env->next->input = env->input + env->size - bytes_in_buffer;
		env->next->size = bytes_in_buffer;
		rv = env->next->dispatcher(env->next, SPOP_DECODE); //pass through to next module
		if(rv > 0){
			bytes_in_buffer -= rv;
		}
		else {
			break;
		}
	} while (bytes_in_buffer > 0);

	return env->size;
}

int wav(struct sp* env, enum sp_ops operation){
	//dispatcher for wav
	switch(operation){
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
