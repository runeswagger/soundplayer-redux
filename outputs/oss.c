int oss_configure(sp_module_t *env){
	int bits = env->p.bits;
	int channels = env->p.chan;
	int sample_rate = env->p.rate;
	
	ioctl(fileno(env->input), SOUND_PCM_WRITE_BITS, &bits);
	ioctl(fileno(env->input), SOUND_PCM_WRITE_CHANNELS, &channels);
	ioctl(fileno(env->input), SOUND_PCM_WRITE_RATE, &sample_rate);
	
	return 0;
}

int output_oss_init(sp_module_t *env){
	//start up the oss
	if(env->data == NULL){
		//default to /dev/dsp
		env->data = (void*)"/dev/dsp";
	}
	output_file_init(env);
	return 0;
}

int output_oss_play(char *in, long size){
	//play a buffer
	return output_file_play(in,size);
}

int oss_deinit(sp_module_t *env){
	//deallocate things
	return file_deinit(env);
}

int oss(sp_module_t *module, sp_operation_t operation){
	switch(operation){
		case SPOP_AUTO:
		case SPOP_DECODE:
		case SPOP_OUTPUT:
			return oss_output(module);
		default:
			return SP_NOCODE;
	}
	return SP_ABORT;
}
