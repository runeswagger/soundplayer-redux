#include <alsa/asoundlib.h>
#include "../includes/common.h"

struct cb callback;

int output_alsa_configure(int sample_rate, int channels, void* module_data){
	(void)module_data;
	snd_pcm_set_params(callback.handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, channels, sample_rate, 1, 500000);
	return SP_OK;
}

int output_alsa_init(void* arg){
	//initialize alsa
	if(arg == NULL){
		//the arg is a module specific initializer
		//for alsa it is the device to use
		arg = (void*)"default";
	}
	
	snd_pcm_t *pcm;
	
	snd_pcm_open(&pcm,(char*)arg, SND_PCM_STREAM_PLAYBACK, 0);
	if(pcm == NULL) return -1;


	callback.handle = pcm;
	output_alsa_configure(44100, 2, NULL);
	return SP_OK;
}

int output_alsa_play(char* in, long size){
	//play a buffer through alsa
	int rval;
	rval = snd_pcm_writei(callback.handle, in, snd_pcm_bytes_to_frames(callback.handle, size));
	if(rval == -EPIPE){
		snd_pcm_prepare(callback.handle);
		return SP_AGAIN; //hint that decoder module should resend it's data
			//for uninterrupted playback
	}
	return SP_OK;
}

int output_alsa_deinit(){
	//kill alsa
	snd_pcm_t *pcm = callback.handle;
	snd_pcm_drain(pcm);
	snd_pcm_close(pcm);
	return SP_OK;
}
