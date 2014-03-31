/* utilizes the blargg snes apu code to play spc files */

/* this is the include for the C interface */
#include <stdlib.h>
#include "spc.h"
#include "../includes/common.h"
#include "../includes/spc.h"
#include "../includes/splib.h"

struct spc_data {
	SNES_SPC* snes_spc;
	SPC_Filter* filter;
	unsigned char* buffer;
};

int spc_init(sp_module_t *env){
	/* allocate spc_data */
	struct spc_data *data = malloc(sizeof(struct spc_data));
	
	/* Create emulator and filter */
	data->snes_spc = spc_new();
	data->filter = spc_filter_new();
	
	//since this only happens if you don't have enough ram to allocate, we'll error hard here
	if (!data->snes_spc) return SP_ABORT;

	/* store SNES_SPC data, if the next function fails we can easily clean up and try again */
	env->data = data;
	
	/* Load SPC data into emulator - if it isn't null something bad happend */
	if(spc_load_spc(data->snes_spc, env->input, env->size) != NULL){
		spc_deinit(env); //clean up so we can try again
		return SP_AGAIN; //we don't hold buffer yet, so you'll need to resend buffer + more data
	}

	/* Most SPC files have garbage data in the echo buffer, so clear that */
	spc_clear_echo(data->snes_spc);

	env->next->p.rate = 32000;
	env->next->p.chan = 2;
	env->next->dispatcher(env->next, SPOP_CONFIGURE); //snes_spc works at 32khz
	return SP_OK;
}

int spc_decode(sp_module_t *env){
	struct spc_data* data = env->data;
	
	short buf [2048];
	//play indefinitely because i don't have a limit built into struct sp
	while (1){
		spc_play(data->snes_spc, 2048, buf);
		spc_filter_run(data->filter, buf, 2048);
		sp_data_in(env->next, (char*)buf, 2048*sizeof(short));
	}
	return SP_OK;
}

int spc_deinit(sp_module_t *env){
	struct spc_data* data = env->data;
	spc_filter_delete(data->filter);
	spc_delete(data->snes_spc);
	free(data);
	env->data = NULL;
	return SP_OK;
}

int spc(sp_module_t *module, sp_operation_t operation){
	switch(operation){
		case SPOP_AUTO:
			return spc_decode(module);
		case SPOP_DECODE:
			return spc_decode(module);
		case SPOP_INIT:
			return spc_init(module);
		case SPOP_DEINIT:
			return spc_deinit(module);
		default:
			return SP_NOCODE;
	}
	return SP_ABORT;
}
