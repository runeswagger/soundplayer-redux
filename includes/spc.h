#ifndef H_SPC
#define H_SPC

int spc_init(sp_module_t *env);
int spc_decode(sp_module_t *env);
int spc_deinit(sp_module_t *env);
int spc(sp_module_t *module, sp_operation_t operation);

#endif
