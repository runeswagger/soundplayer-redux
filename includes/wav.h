#ifndef H_WAV
#define H_WAV

int wav(struct sp* env, enum sp_ops operation);
int wav_init(struct sp *env);
int wav_deinit(struct sp *env);
int wav_play(struct sp *env);

#endif
