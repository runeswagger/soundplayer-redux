/* Module Test
 * single module/single output varient of soundplayer
 * designed to just drop in any module easily
 * updated for the new soundplayer api
 */

//standard includes
#include <stdio.h>
#include <string.h>

//configure soundplayer environment
#define USE_WAV
#define USE_OUTPUT_ALSA

//includes for soundplayer
//you really only need to include the headers for output-alsa and the module
#include "includes/common.h"
#include "includes/splib.h"
#include "includes/decoders.h"
#include "includes/outputs.h"

#define BUF_SIZE 32768

struct sp output = {
	.dispatcher = alsa
};

struct sp soundp = {
	.dispatcher = wav,
	.next = &output
};

int refill_buffer(FILE* input, unsigned char *b, int b_size, int bytes_remaining){
	int i = bytes_remaining; //make a copy for looping
	//changed from --i to i-- because bytes_remaining == 0 resulted in while(-1) an infinite loop
	//or at least infinite until int overflowed
	while (i--) b[bytes_remaining - i] = b[b_size - i];

	return fread(b + bytes_remaining, 1, b_size - bytes_remaining, input);
}

int main(int argc, char* argv[]){
	//play a whole file using a module
	int bytes_consumed = 0;
	char buffer[BUF_SIZE];

	if(argc < 2){ //no argument supplied
		printf("Usage:\n\t%s infile\n", argv[0]); //print help
		return -1;
	}
	
	FILE* input = fopen(argv[1], "r"); //open input for reading

	if(input == NULL){ //can't open input
		perror("input");
		return -1;
	}

	//start setting up struct sp
	soundp.input = buffer; //yes I want to take the address of a local
	soundp.size = fread(soundp.input, 1, BUF_SIZE, input); //read into buffer
	soundp.format = UNIMPLEMENTED; //the module actually doesn't care about this
	//it's there so the program can distinguish multiple decoder instances
	
	//running our main loop
	sp_module_init(&output);

	if(sp_module_init(&soundp) == SP_ABORT) goto CLEANUP;
	//start seeking the input to an offset if necessary
	fseek(input, soundp.p.offset, SEEK_SET);

	soundp.size = fread(soundp.input, 1, BUF_SIZE, input); //read past offset into buffer
	
	do{
		bytes_consumed = soundp.dispatcher(&soundp, SPOP_DECODE);
		soundp.size = refill_buffer(input, soundp.input, BUF_SIZE, BUF_SIZE - bytes_consumed);
	} while(soundp.size);

	
	sp_module_deinit(&soundp);

	CLEANUP:
	sp_module_deinit(&output);

	return 0;
}
