/* This module will use the espeak library to render text input as speech
 * espeak uses formant synthesis, a type of procudural generation of human vocal sounds
 * based off of the physical properties of the human vocal cords
 * it should be fairly lightweight to link in unlike a diphonic tts engine like festival
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "espeak/speak_lib.h"
#include "../includes/common.h"

struct tts_data {
	espeak_VOICE voice; //not sure it needs persistance but whatever
	int rate; //speech rate... we set this to 120 default, a little slower than espeak default
	int wordgap; //we leave this alone, espeak selects a reasonable default
	int pitch; //I'll have to play with this
	unsigned int flags; //the flags we use when calling Synth
};

struct cb callback;

int tts_speech_callback(short* wav, int nsamp, espeak_EVENT* events){
	//Handle speech samples generated by espeak
	
	if(wav == NULL) {
		return 0; //NULL means synth complete... I'm not sure what the impact is though
	}
	
	//play the incoming data
	callback.audio_play((char*)wav, nsamp*sizeof(short)); //number of shorts is half buffer size
	(void)events; //i could respond to events like parsed xml commands or do something at each sentence

	return 0; // 1 || 0 == continue || abort synthesis
}

int tts_init(struct sp* env){
	struct tts_data *data = malloc(sizeof(struct tts_data)); //make a struct
	memset(data, 0, sizeof(struct tts_data)); //initialize to zero

	espeak_SetSynthCallback(&tts_speech_callback);
	//setting up espeak to pass no more than 32k buffers to speech_callback
	//having espeak search for data dir in default location and setting flags == 0
	int hz = espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 32768, NULL, 0);
	if(hz != EE_INTERNAL_ERROR){
		//return value is sample rate
		callback.audio_configure(hz, 1, NULL);
	}
	else {
		return SP_ABORT; //return an error
	}
	
	//configure all the variables
	
	data->voice.name = "english";
	data->voice.languages = "en-us"; //english
	data->voice.gender = 2; //1 == male | 2 == female
	data->voice.age = 24; //set to 0 for default
	data->rate = 120; //a little slower than default
	data->wordgap = espeak_GetParameter(espeakWORDGAP, 1); //current wordgap
	data->pitch = espeak_GetParameter(espeakPITCH, 1); //default pitch
	data->flags |= espeakCHARS_UTF8; //assume UTF8, perhaps autodetect later

	//apply the properties
	espeak_SetVoiceByProperties(&data->voice);
	espeak_SetParameter(espeakRATE, data->rate, 0);
	espeak_SetParameter(espeakWORDGAP, data->wordgap, 0);
	espeak_SetParameter(espeakPITCH, data->pitch, 0);
	
	env->private_data = data; //export module data
	return SP_OK;
}

int tts_play(struct sp* env){
	//convert text to speech
	struct tts_data* data = env->private_data;
	unsigned int id;
	espeak_Synth(env->input, env->size, 0, POS_CHARACTER, env->size/*end pos*/, data->flags, &id, data);
	return env->size;
}

int tts_deinit(struct sp* env){
	//clean up espeak structures
	espeak_Terminate();
	free(env->private_data);
	return SP_OK;
}
