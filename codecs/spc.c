/* utilizes the blargg snes apu code to play spc files */

/* this is the include for the C interface */
#include "spc.h"
#include "../includes/common.h"
//include own header file, so that i can freely deinit if init doesn't go so well
#include "../includes/spc.h"
#include "../includes/splib.h"

struct spc_data {
	SNES_SPC* snes_spc;
	unsigned char* buffer;
};

int spc_init(struct sp* env){
	/* Create emulator and filter */
	SNES_SPC* snes_spc = spc_new();

	//since this only happens if you don't have enough ram to allocate, we'll error hard here
	if (!snes_spc) return SP_ABORT;

	/* store SNES_SPC data, if the next function fails we can easily clean up and try again */
	env->private_data = snes_spc;
	
	/* Load SPC data into emulator - if it isn't null something bad happend */
	if(spc_load_spc(snes_spc, env->input, env->size) != NULL){
		spc_deinit(env); //clean up so we can try again
		return SP_AGAIN; //we don't hold buffer yet, so you'll need to resend buffer + more data
	}

	/* Most SPC files have garbage data in the echo buffer, so clear that */
	spc_clear_echo(snes_spc);

	callback.audio_configure(32000, 2, NULL); //snes_spc works at 32khz
	return SP_OK;
}

int spc_play_sp(struct sp* env){
	SNES_SPC* snes_spc = env->private_data;
	short buf [2048];
	//play indefinitely because i don't have a limit built into struct sp
	while (1){
		spc_play(snes_spc, 2048, buf);
		sp_data_in(env->next, (char*)buf, 2048*sizeof(short));
	}
	return SP_OK;
}

int spc_deinit(struct sp* env){
	spc_delete((SNES_SPC*) env->private_data);
	return SP_OK;
}
