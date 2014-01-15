#include "../includes/common.h"

struct cb callback;

int wav_init(struct sp *env) {
	env->p.offset = 44; //jump past RIFF header
	//add riff header parsing code, so i can configure the output module to match
	return SP_OK;
}

int wav_deinit(struct sp *env) {
	(void) env;
	return SP_OK;
}

int wav_play(struct sp *env) {
	int bytes_in_buffer = env->size;
	int rv = 0; //return value for detecting errors from audio_play
	do {
		//end_of_buffer-bytes_in_buffer is current start of new data
		//play for bytes remaining in the buffer
		rv = callback.audio_play(env->input + env->size - bytes_in_buffer, bytes_in_buffer);
		if(rv > 0){
			bytes_in_buffer -= rv;
		}
		else {
			break;
		}
	} while (bytes_in_buffer > 0);

	return env->size;
}
