#ifndef H_WAV
#define H_WAV

int wav(sp_module_t *env, sp_operation_t operation);
int wav_init(sp_module_t *env);
int wav_deinit(sp_module_t *env);
int wav_play(sp_module_t *env);

#endif
