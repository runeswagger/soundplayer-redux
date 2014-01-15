int output_memory_init(void* arg){
	//do nothing since memory output just returns a buffer of decoded data
	callback.handle = NULL;
	return 0;
}

int output_memory_play(char* buf, long size){
	//adjust size of memory and copy pcm data there
	free(callback.handle); //okay since init sets it to NULL
	callback.handle = malloc(size); //make a new buffer

	int i = 0;
	do { (unsigned char*)callback.handle[i] = buf[i]; } while(++i<size); //copy
	return i;
}

output_memory_deinit(){
	free(callback.handle);
}
