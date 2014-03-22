#ifndef H_COMMON
#define H_COMMON

enum sp_type {
	WAV,
	AAC,
	MP3,
	OGG,
	FLAC,
	SPC,
	TXT,
	UNIMPLEMENTED,
	LEN_MODULES
};

enum sp_err {
	SP_OK = 0, 
	SP_AGAIN = -1,
	SP_ABORT = -2,
	SP_NOCODE = -3
};

enum sp_state {
	SP_CONTINUE
};

enum sp_ops {
	SPOP_DECODE,
	SPOP_INIT,
	SPOP_DEINIT,
	SPOP_ENCODE,
	SPOP_CONFIGURE
};

struct param {
	int rate;
	char chan;
	char id;
	char bits;
	char pad;
	int offset;
};

struct sp {
	char *input;
	void *data;
	unsigned long size;
	enum sp_type format;
	enum sp_state state;
	struct param p;
	struct sp* next;
	int (*dispatcher)(struct sp* env, enum sp_ops operation);
	int (*audio_configure)(int sample_rate, int channels, void* module_data);
	int (*audio_init)(void* arg);
	int (*audio_play)(char*,long);
	int (*audio_deinit)();
};

#endif
