#ifndef H_SPLIB
#define H_SPLIB

inline int sp_module_init(sp_module_t *module);
inline int sp_module_deinit(sp_module_t *module);
inline int sp_data_in(sp_module_t *module, char* buffer, unsigned long len);
sp_module_t* sp_module_new(sp_type_t format);
void sp_module_destroy(sp_module_t* module);

#endif
