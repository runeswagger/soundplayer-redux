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


int output_file_play(char *in, long size) {
	return fwrite(in, 1, size,callback.handle);
}

int output_file_deinit() {
	fclose(callback.handle);
	return 0;
}
