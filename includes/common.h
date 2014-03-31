#ifndef H_COMMON
#define H_COMMON

//forward definitions for typedefs
enum sp_type;
enum sp_error;
enum sp_state;
enum sp_operation;
struct sp_param;
struct sp_module;

//typedef everything. because aesthetics
typedef enum sp_type sp_type_t;
typedef enum sp_error sp_error_t;
typedef enum sp_state sp_state_t;
typedef enum sp_operation sp_operation_t;
typedef struct sp_param sp_param_t;
typedef struct sp_module sp_module_t;

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

enum sp_error {
	SP_OK = 0, 
	SP_AGAIN = -1,
	SP_ABORT = -2,
	SP_NOCODE = -3
};

enum sp_state {
	SP_CONTINUE
};

enum sp_operation {
	SPOP_AUTO, //use clues in sp_module_t when called from another module
	SPOP_DECODE,
	SPOP_INIT,
	SPOP_DEINIT,
	SPOP_ENCODE,
	SPOP_CONFIGURE
};

struct sp_param {
	int rate;
	char chan;
	char id;
	char bits;
	char pad;
	int offset;
};

struct sp_module {
	char *input;
	void *data;
	unsigned long size;
	sp_type_t format;
	sp_state_t state;
	sp_param_t p;
	int flags; //used with SPOP_AUTO
	sp_module_t *next;
	int (*dispatcher)(sp_module_t *env, sp_operation_t operation);
};



#endif
