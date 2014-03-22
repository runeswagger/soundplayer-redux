#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

//config header, includes prototypes for various functions
#include "config.h"
#include "includes/common.h"
#include "includes/decoders.h"
#include "includes/outputs.h"

#define BUF_SIZE 4096
#define NUM_FUNC 3

void detect_filetype_from_filename(char* filename);
int arg_parse(int, char**);
int err();
enum sp_type fmt;

void progress(long cur, long end){
	printf("%li/%li\r",cur,end);
	fflush(stdout);
}

int main(int argc, char **argv) {
	if(argc < 2) return -1;

	struct sp callback = { .dispatcher = alsa, .data = "default" }; //we have an alsa callback
	struct sp soundp = { .next = &callback }; //create the link

	arg_parse(argc, argv);

	if(fmt == UNIMPLEMENTED){
		//user did not specify a -f
		detect_filetype_from_filename(argv[argc-1]);
	}

	soundp.format = fmt;

	int fd = open(argv[argc-1], O_RDONLY);
	if (fd == -1){
		perror("open");
		return 1;
	}

	struct stat fileinfo;
	fstat(fd, &fileinfo);
	//map file, and round up to the next page boundary
	soundp.input = mmap(NULL, ((fileinfo.st_size/4096+1)*4096), PROT_READ, MAP_PRIVATE, fd, 0 );
	soundp.size = fileinfo.st_size;

	int (*handlers[LEN_MODULES])(struct sp *arg, enum sp_ops operation) = {
		[WAV] = wav,
		//[AAC] = {aac_init,aac_play,aac_deinit},
		//[MP3] = {mp3_init,mp3_play,mp3_deinit},
		//[OGG] = {ogg_init,ogg_play,ogg_deinit},
		//[FLAC] = {err,err,err},
		//[SPC] = {spc_init,spc_play_sp,spc_deinit},
		//[TXT] = {tts_init,tts_play,tts_deinit},
		[UNIMPLEMENTED] = err
	};

	int curindex = 0;
	int bytes_consumed = 0;
	{
		callback.dispatcher(&callback, SPOP_INIT);

		if(handlers[soundp.format](&soundp, SPOP_INIT)) goto END;
		soundp.input += soundp.p.offset; //if struct sp is used uninitialized this could be a problem
		do { //play song
			if((fileinfo.st_size - curindex) > 4096){
				soundp.size = 4096;
			}
			else{
				soundp.size = fileinfo.st_size - curindex;
			}
			bytes_consumed = handlers[soundp.format](&soundp, SPOP_DECODE);
			soundp.input += bytes_consumed;
			curindex += bytes_consumed;
			progress(curindex, fileinfo.st_size);
		} while(curindex < fileinfo.st_size);
		handlers[soundp.format](&soundp, SPOP_DEINIT);

		//cleanup
		END:
		callback.dispatcher(&callback, SPOP_DEINIT);
	}

	//exit
	return 0;
}


int arg_parse(int argc, char **argv) {
	fmt = UNIMPLEMENTED; //initialize value

	int parse = 0;
	while((parse = getopt(argc, argv, "f:o:")) != -1){
		switch(parse){
			case 'f':
				fmt = atoi(optarg);
				break;
			case 'o':
				//i don't have an easy way to select outputs yet. not outputs[][]
			//	callback.audio_init = output_file_init;
			//	callback.audio_play = output_file_play;
			//	callback.audio_deinit = output_file_deinit;
			//	callback.audio_configure = output_file_configure;
				break;
		}
	}

	return 0;
}

int err() {
	printf("Decoder Error!\n");
	return SP_ABORT;
}

void detect_filetype_from_filename(char* filename){
	//set fmt to the real format
	const long ext[] = {
		*(int*)("wav "),
		*(int*)("aac "),
		*(int*)("mp3 "),
		*(int*)("ogg "),
		*(int*)("flac"),
		*(int*)("spc "),
		*(int*)("txt ") };

	long file_ext;
	int i = 0;
	fmt = UNIMPLEMENTED;

	while(filename[i++]);
	i -= 5; //position index on last 4 bytes
	if(i < 0) return; //avoid accessing invalid memory
	file_ext = *(int*)(filename+i); //grab the last 8 bytes of filename
	//crazy

	//shift past the period
	while((char)file_ext != '.' && file_ext) {
		file_ext >>= 8; //shift one character
	}
	file_ext >>= 8; //we now have the extension
	file_ext |= *(int*)("    "); //ORing 4 spaces converts to lower with minimal punctuation loss

	for(i = 0; i < 7; i++){
		//search all fileextensions
		if(file_ext == ext[i]) {
			fmt = i;
			break;
		}
	}
}
