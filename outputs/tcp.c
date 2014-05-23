//TCP module for soundplayer

#include "../includes/common.h"
#include "../includes/splib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

struct tcp_data {
	int sockfd;
	struct sockaddr_in addr;
};

int tcp_init(sp_module_t *env){
	//set up the tcp module
	struct tcp_data *data = malloc(sizeof(struct tcp_data));
	
	data->sockfd = socket(AF_INET, SOCK_STREAM, 0);

	env->data = data;
	return SP_OK;
}

int tcp_input(sp_module_t *env){
	//return data from the internet
	return 0;
}

int tcp_output(sp_module_t *env){
	//output data to the internet
	//act as a server
	struct tcp_data *data = env->data;

	//listen(data->sockfd, 1); //listen on sockfd, with backlog of 1 incomming connection in queue
	return 0;
}

int tcp_deinit(sp_module_t *env){
	//clean up module
	struct tcp_data *data = env->data;
	
	return SP_OK;
}

int tcp(sp_module_t *module, sp_operation_t operation){
	//tcp event dispatcher
	switch(operation){
		case SPOP_AUTO:
			return tcp_output(module);
		case SPOP_DECODE:
		case SPOP_OUTPUT:
			return tcp_output(module);
		case SPOP_INPUT:
		case SPOP_ENCODE:
			return tcp_input(module);
		case SPOP_INIT:
			return tcp_init(module);
		case SPOP_DEINIT:
			return tcp_deinit(module);
		default:
			return SP_NOCODE;
	}
	return SP_ABORT;
}
