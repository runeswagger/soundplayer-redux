#include <alsa/asoundlib.h>
#include "../includes/common.h"

struct alsa_data {
	snd_pcm_t* pcm; //the playback handle
};

int alsa_configure(sp_module_t *env){
	struct alsa_data* data = (struct alsa_data*) env->data;
	snd_pcm_set_params(data->pcm, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, env->p.chan, env->p.rate, 1, 500000);
	return SP_OK;
}

int alsa_init(sp_module_t *env){
	//initialize alsa
	struct alsa_data* data = malloc(sizeof(struct alsa_data));
	void* arg = env->data;
	if(arg == NULL){
		//the arg is a module specific initializer
		//for alsa it is the device to use
		arg = (void*)"default";
	}
	
	snd_pcm_t *pcm;
	
	snd_pcm_open(&pcm,(char*)arg, SND_PCM_STREAM_PLAYBACK, 0);
	if(pcm == NULL) return -1;

	data->pcm = pcm;
	env->data = data;
	env->p.rate = 44100;
	env->p.chan = 2;
	alsa_configure(env);
	return SP_OK;
}

int alsa_decode(sp_module_t *env){
	//play a buffer through alsa

	//adapting for sp_module_t
	char* in = env->input;
	long size = env->size;
	struct alsa_data* data = (struct alsa_data*) env->data;
	
	int rval;
	rval = snd_pcm_writei(data->pcm, in, snd_pcm_bytes_to_frames(data->pcm, size));
	if(rval == -EPIPE){
		snd_pcm_prepare(data->pcm);
		return SP_AGAIN; //hint that decoder module should resend it's data
			//for uninterrupted playback
	}
	return SP_OK;
}

int alsa_deinit(sp_module_t *env){
	//kill alsa
	struct alsa_data* data = (struct alsa_data*) env->data;
	snd_pcm_drain(data->pcm);
	snd_pcm_close(data->pcm);
	free(env->data);
	return SP_OK;
}

int alsa(sp_module_t *env, sp_operation_t operation){
	switch(operation){
		case SPOP_AUTO:
			return alsa_decode(env); //TODO: write alsa_auto();
		case SPOP_DECODE:
			return alsa_decode(env);
		case SPOP_DEINIT:
			return alsa_deinit(env);
		case SPOP_CONFIGURE:
			return alsa_configure(env);
		case SPOP_INIT:
			return alsa_init(env);
		default:
			return SP_NOCODE;
	}
	return SP_ABORT;
}
