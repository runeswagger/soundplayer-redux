#ifndef H_COMMON
#define H_COMMON

#define sp_flag_set(module, y) do { module->flags |= (y); } while(0);
#define sp_flag_clr(module, y) do { module->flags &= ~(y); } while(0);
#define sp_flag_isset(module, y) (module->flags & (y))

//forward definitions for typedefs
enum sp_type;
enum sp_flags;
enum sp_error;
enum sp_state;
enum sp_operation;
struct sp_param;
struct sp_module;

//typedef everything. because aesthetics
typedef enum sp_type sp_type_t;
typedef enum sp_flags sp_flags_t;
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

enum sp_flags {
	SP_CODEC = 0, //can be pipelined
	SP_ENDPOINT = 1, //endpoints cannot pass data to other modules
	SP_MANUAL = 0,
	SP_AUTO = 2,
	SP_INPUT = 4, //module is an input
	SP_INITIALIZED = 8, //module is initialized
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
	SPOP_OUTPUT, //synonymous with DECODE, for modules where it doesn't make sense
	SPOP_DECODE,
	SPOP_INIT,
	SPOP_DEINIT,
	SPOP_INPUT, //synonymous with ENCODE
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
	sp_flags_t flags; //used with SPOP_AUTO
	sp_module_t *next;
	int (*dispatcher)(sp_module_t *env, sp_operation_t operation);
};

#endif
