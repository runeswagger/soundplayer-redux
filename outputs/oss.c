int output_oss_configure(int sample_rate, int channels, void* module_data){
	int a = 16;
	ioctl(fileno(callback.handle), SOUND_PCM_WRITE_BITS, &a);
	ioctl(fileno(callback.handle), SOUND_PCM_WRITE_CHANNELS, &channels);
	ioctl(fileno(callback.handle), SOUND_PCM_WRITE_RATE, &sample_rate);
	return 0;
}

int output_oss_init(void *arg){
	//start up the oss
	if(arg == NULL){
		//default to /dev/dsp
		arg = (void*)"/dev/dsp";
	}
	output_file_init(arg);
	output_oss_configure(44100, 2, NULL);
	return 0;
}

int output_oss_play(char *in, long size){
	//play a buffer
	return output_file_play(in,size);
}

int output_alsa_deinit(){
	//deallocate things
	return output_file_deinit();
}
