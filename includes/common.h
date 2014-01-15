#ifndef H_COMMON
#define H_COMMON
enum {
	INIT,
	PLAY,
	DEINIT
};

enum type {
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
};

enum sp_state {
	SP_CONTINUE
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
	void *private_data;
	unsigned long size;
	enum type format;
	enum sp_state state;
	struct param p;
};

struct cb {
	void *handle;
	int (*audio_configure)(int sample_rate, int channels, void* module_data);
	int (*audio_init)(void* arg);
	int (*audio_play)(char*,long);
	int (*audio_deinit)();
};
#endif
