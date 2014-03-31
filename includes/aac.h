#ifndef H_AAC
#define H_AAC

int aac_init(sp_module_t *env);
int aac_decode(sp_module_t *env);
int aac_deinit(sp_module_t *env);
int aac(sp_module_t *env, sp_operation_t operation);

#endif
