#ifndef H_OUTPUT_FILE
#define H_OUTPUT_FILE

int output_file_configure(int sample_rate, int channels, void* module_data);
int output_file_init();
int output_file_deinit();
int output_file_play(char *buff, long size);

#endif
