#ifndef H_OUTPUT_ALSA
#define H_OUTPUT_ALSA

int output_alsa_configure(int sample_rate, int channels, void* module_data);
int output_alsa_init(void* arg);
int output_alsa_play(char* in, long size);
int output_alsa_deinit(void);

#endif
