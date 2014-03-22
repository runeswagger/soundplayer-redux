#ifndef H_MP3
#define H_MP3

int mp3_init(struct sp *env);
int mp3_deinit(struct sp *env);
int mp3_play(struct sp *env);
int mp3(struct sp* env, enum sp_ops operation);

#endif
