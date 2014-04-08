int memory_init(struct sp* env){
	//do nothing since memory output just returns a buffer of decoded data
	callback.handle = NULL;
	return 0;
}

int memory_play(char* buf, long size){
	//adjust size of memory and copy pcm data there
	free(callback.handle); //okay since init sets it to NULL
	callback.handle = malloc(env->size); //make a new buffer

	int i = 0;
	do { (unsigned char*)callback.handle[i] = buf[i]; } while(++i<size); //copy
	return i;
}

int memory_deinit(){
	free(callback.handle);
}

memory(struct sp* env, enum sp_ops operation){
	switch(operation){
		case SPOP_DECODE:
			return memory_decode(env);
		case SPOP_INIT:
			return memory_init(env);
		case SPOP_DEINIT:
			return memory_deinit(env);
		default:
			return SP_NOCODE;
	}
	return SP_ABORT;
}
