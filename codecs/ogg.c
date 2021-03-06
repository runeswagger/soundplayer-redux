#include "../includes/common.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
//vorbisfile hack

struct ogg_data {
	OggVorbis_File vf;
	ov_callbacks handlers;
	int current_section;
	long status;
	long marker;
	long cursor;
};

struct cb callback;

void mcpy(void* dest, void* src, size_t n){
	//mysterious segfault in __memcpy_sse2_unaligned
	//to hell with __memcpy_sse2_unaligned
	char* there = (char*) dest;
	char* here = (char*) src;
	while(n--) { //this ought to do it
		n[there] = n[here];
	}
	0[there] = 0[here];
}

size_t ogg_read_callback(void *ptr, size_t size, size_t nmemb, void* datasource){
	//rewrite, because vorbisfile is hard
	//WAIT... fread never modifies ptr... I've been doing it wrong this whole time

	//retrieve our data 
	struct sp * env = datasource;
	struct ogg_data * data = env->private_data;

	int requested = size * nmemb; //number of bytes the user wants
	int available = env->size - data->cursor; //bytes left in buffer

	//logic it up
	if( requested <= available ){
		//we can meet that request
		memcpy(ptr, env->input + data->cursor, requested);
		data->cursor += requested;
		return requested;
	}
	else {
		//we can't meet that request, send what we have
		memcpy(ptr, env->input + data->cursor, available);
		data->cursor += available+1; //and +1 for stdiolikeness
		return available;
	}

	errno = EAGAIN; //something bad must have happened
	return 0;
}

int ogg_init(struct sp* env){
	struct ogg_data *data = calloc(sizeof(struct ogg_data),1);
	env->private_data = data; //forgot this, wondered why everything went bonkers
	data->handlers.read_func = ogg_read_callback;
	data->handlers.seek_func = NULL;
	data->handlers.close_func = NULL;
	data->handlers.tell_func = NULL;
	data->cursor = 0;
	int rv = 0; //return value for ogg
	
	if (env->input == NULL) return SP_AGAIN; //must have input to successfully open
	//ov_open will pass it's first argument to it's callbacks as the datasource argument
	rv = ov_open_callbacks(env, &data->vf, NULL, 0, data->handlers);

	if (rv == OV_EREAD) {
		//reading input failure... probably needs more data
		return SP_AGAIN;
	}
	if (rv < 0){
		//not vorbis, corruption, invalid header... bad juju in general
		return SP_ABORT;
	}
	
	return 0;
}

int ogg_play(struct sp* env){
	char buffer[256];
	struct ogg_data *data = env->private_data;
	int bytes_in_buffer = env->size;
	int samples;
	data->cursor = 0;
	while(bytes_in_buffer){
		samples = ov_read(&data->vf, buffer, 256, 0, 2, 1, &data->current_section);
		if(samples > 0) { //no error occured
			callback.audio_play(buffer, samples);
		}
		if(samples == 0) break;
	}

	//well I can't detect partially used buffers yet
	//so I'll assume we used it all
	return env->size;
}

int ogg_deinit(struct sp* env){
	ov_clear(&((struct ogg_data*)env->private_data)->vf);
	return 0;
}
