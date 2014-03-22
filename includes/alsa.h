#ifndef H_OUTPUT_ALSA
#define H_OUTPUT_ALSA

int alsa(struct sp* env, enum sp_ops operation);
int alsa_configure(struct sp* env);
int alsa_init(struct sp* env);
int alsa_decode(struct sp* env);
int alsa_deinit(struct sp* env);

#endif
