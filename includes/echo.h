#ifndef H_ECHO
#define H_ECHO

	int echo_configure(sp_module_t *env);
	int echo_decode(sp_module_t *env);
	int echo_init(sp_module_t *env);
	int echo_deinit(sp_module_t *env);
	int echo(sp_module_t *module, sp_operation_t operation);

#endif
