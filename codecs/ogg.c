#include "../includes/common.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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

size_t ogg_read_callback(void *ptr, size_t size, size_t nmemb, void* datasource){
	//0 == end of data
	//short reads are allowed
	struct sp *data = datasource;
	struct ogg_data *odata = data->private_data;

	int requested_bytes = size * nmemb; //number of bytes requested
	int bytes_available = data->size - odata->cursor;

	if(bytes_available == -1) return 0; //end of buffer

	//expose buffer input+cursor if the buffer is filled
	ptr = data->input + odata->cursor;
	(void)ptr;
	if(bytes_available > requested_bytes) {
		odata->cursor += requested_bytes;
	} else {
		//odata->cursor += bytes_available;
		odata->cursor = data->size+1;
	}
	return bytes_available>requested_bytes ? requested_bytes : bytes_available;
}

int ogg_init(struct sp* env){
	struct ogg_data *data = calloc(sizeof(struct ogg_data),1);
	env->private_data = data; //forgot this, wondered why everything went bonkers
	data->handlers.read_func = ogg_read_callback;
	data->handlers.seek_func = NULL;
	data->handlers.close_func = NULL;
	data->handlers.tell_func = NULL;
	data->cursor = 0;
	
	if (env->input == NULL) return -1; //must have input to successfully open
	//ov_open will pass it's first argument to it's callbacks as the datasource argument
	//this always fails
	if (ov_open_callbacks(env, &data->vf, NULL, 0, data->handlers) < 0) return -1;
	return 0;
}

int ogg_play(struct sp* env){
	char buffer[256];
	struct ogg_data *data = env->private_data;
	int bytes_in_buffer = env->size;
	int samples;
	while(bytes_in_buffer){
		samples = ov_read(&data->vf, buffer, 256, 0, 2, 1, &data->current_section);
		if(samples > 0) { //no error occured
			callback.audio_play(buffer, samples/4);
		}
		if(samples == 0) break;
	}
	data->cursor = 0;
	return 0;
}

int ogg_deinit(struct sp* env){
	ov_clear(&((struct ogg_data*)env->private_data)->vf);
	return 0;
}
