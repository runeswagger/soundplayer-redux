#include <stdio.h>
#include "../includes/common.h"

struct cb callback;

int output_file_configure(int sample_rate, int channels, void* module_data){
	(void)sample_rate;
	(void)channels;
	(void)module_data;
	return 0;
}

int output_file_init(void *arg) {
	if(arg == NULL) {
		//default value is output.wav
		arg = (char*)"output.wav";
	}
	FILE* output;
	output = fopen((char *) arg, "w+");
	callback.handle = output;
	return 0;
}

int file_encode(sp_module_t *env){
	return fread(env->input, 1, env->size, env->data);
}

int file_decode(sp_module_t *env) {
	return fwrite(env->input, 1, env->size, env->data);
}

int output_file_deinit(sp_module_t *env) {
	fclose(NULL);
	return 0;
}
