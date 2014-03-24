#ifndef H_OUTPUT_ALSA
#define H_OUTPUT_ALSA

int alsa_configure(sp_module_t *env);
int alsa_init(sp_module_t *env);
int alsa_decode(sp_module_t *env);
int alsa_deinit(sp_module_t *env);
int alsa(sp_module_t *env, sp_operation_t operation);

#endif
