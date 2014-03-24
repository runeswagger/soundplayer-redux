#ifndef H_OGG
#define H_OGG

int ogg_init(sp_module_t *env);
int ogg_play(sp_module_t *env);
int ogg_deinit(sp_module_t *env);
int ogg(sp_module_t *env, sp_operation_t operation);

#endif
