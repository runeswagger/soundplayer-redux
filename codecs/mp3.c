#include <stdlib.h>
#include <unistd.h>
#include <lame/lame.h>
#include "../includes/common.h"

#define MP3_FRAME_SIZE 1152
//number of shorts in a frame
#define RAWSIZE MP3_FRAME_SIZE*2

struct mp3_data {
	lame_t lame;
	hip_t	l;
	short *b; //output buffer
};

struct cb callback;

void interleave(short* pcml, short* pcmr, short* stereo){
	//interleave the channels
	unsigned long x, counter;
	counter = 0;
	for (x = 0; x <= RAWSIZE; x += 2){
		stereo[x] = pcml[counter];
		stereo[x+1] = pcmr[counter];
		counter++;
	}
}

int mp3_init(struct sp* env){
	struct mp3_data *data = malloc(sizeof(struct mp3_data));

	data->lame = lame_init();
	data->l = hip_decode_init();
	lame_init_params(data->lame);
	lame_set_decode_only(data->lame, 1);
	data->b = malloc(RAWSIZE*sizeof(short)); //16bit stereo samples 4 bytes/frame

	env->p.offset = ((*(int*)env->input&0xffffff) == *(int*)"ID3") ? 10+((env->input[8]<<7)|(env->input[9])|(env->input[7]<<14)|(env->input[6]<<21)) : 0; //detect offset
	env->private_data = data;
	return SP_OK;
}

int mp3_play(struct sp* env){
	struct mp3_data *data = env->private_data;
	
	//making buffers
	short pcml[MP3_FRAME_SIZE];
	short pcmr[MP3_FRAME_SIZE];

	int status = 0, bytes_in_buffer = env->size;

	do {
		status = hip_decode1(data->l, (unsigned char*)(env->input + env->size - bytes_in_buffer), bytes_in_buffer, pcml, pcmr);
		bytes_in_buffer = 0; //we consumed a buffer... yay
		interleave(pcml, pcmr, data->b);
		callback.audio_play((char*)data->b, status*4);
	} while (status == MP3_FRAME_SIZE);
	return env->size; //decode function always consumes the full input
}

int mp3_deinit(struct sp* env){
	//clean up mp3 decoder
	struct mp3_data *data = env->private_data;
	hip_decode_exit(data->l); //exit hip decoder instance l
	lame_close(data->lame); //exit lame instance lame
	free(data->b);
	env->private_data = NULL;
	free(data);
	return SP_OK;
}
