#ifndef H_SPC
#define H_SPC

int spc_init(struct sp *env);
int spc_play_sp(struct sp *env); //unfortunate name clash in blargg snes_spc
int spc_deinit(struct sp *env);

#endif
