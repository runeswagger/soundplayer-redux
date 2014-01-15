#include "../includes/common.h"
#include <stdlib.h>
#include <unistd.h>
#include <neaacdec.h>

struct aac_data {
	struct NeAACDecFrameInfo hInfo;
	NeAACDecHandle hAac;
	//NeAACDecConfigurationPtr conf;
	//typedef suggests this is a pointer... so i probably don't need it to persist
};

struct cb callback;

int aac_init(struct sp* env){
	//initialize the aac specific structures
	struct aac_data* data = malloc(sizeof(struct aac_data)); //allocate private structures
	int buffer_size = FAAD_MIN_STREAMSIZE*8; //formerly used in the decode call
	//might not be necessary, it's the minimum amount of data needed to get the decoder to do something
	//hopefully faad acts like lame and internally buffers input until it can make a frame.
	//if not we'll need to internally buffer, by holding a buffer_size sized chunk of memory in aac_data
	//and then filling it
	int cap = NeAACDecGetCapabilities(); //we should do some bulletproofing here, but we won't now

	data->hAac = NeAACDecOpen();

	//Configure the decoder
	NeAACDecConfigurationPtr conf = NeAACDecGetCurrentConfiguration(data->hAac);
	conf->defObjectType = 2; //aac "MAIN"
	conf->defSampleRate = 44100;
	conf->outputFormat = 1;
	conf->downMatrix = 1;
	NeAACDecSetConfiguration(data->hAac, conf); //set the stuff
	// Initialise the library using one of the initialization functions
	unsigned long samplerate;
	unsigned char channels;
	int offset = NeAACDecInit(data->hAac, (unsigned char*)env->input, env->size, &samplerate, &channels);

	if (offset != 0) { //now how do i tell sp from here that audio data starts at index
		env->p.offset = offset; //communicate offset
		return SP_ABORT; //do something to notify of error
	}
	//shutup -Wunused-variable... i'll use them eventually
	(void)buffer_size;
	(void)cap;
	
	env->private_data = data;
	return SP_OK;
}

int aac_play(struct sp* env){
	struct aac_data* data = env->private_data;
	char* pcm = NeAACDecDecode(data->hAac, &data->hInfo, (unsigned char*)env->input, env->size);
	if ((data->hInfo.error == 0) && (data->hInfo.samples > 0)){
		callback.audio_play(pcm, data->hInfo.samples*2);
		//output of decoder is 16 bit, because i set it that way in aac_init;
	}
	else if (data->hInfo.error != 0) {
		;
	}
	return data->hInfo.bytesconsumed;
}

int aac_deinit(struct sp* env){
	//deallocate things
	struct aac_data *data = env->private_data;
	NeAACDecClose(data->hAac); //let faad cleanup
	free(data); //clean up my struct
	return SP_OK;
}
