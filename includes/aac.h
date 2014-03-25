#ifndef H_AAC
#define H_AAC

int aac_init(struct sp* env);
int aac_play(struct sp* env);
int aac_deinit(struct sp* env);
int aac(sp_module_t *env, sp_operation_t operation);

#endif
