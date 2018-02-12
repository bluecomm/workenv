#ifndef H_SOCK_INIT_H
#define H_SOCK_INIT_H

#include <stdint.h>

#define SA_MAX_LISTEN_NUM 10

int32_t server_sock_init_sctp(int port, int blocking);
int32_t client_sock_init_sctp(char *cli_ip, uint16_t cli_port, char *serv_ip,
		uint16_t serv_port, int blocking);
int32_t send_msg_sctp(int32_t sockfd, char *msg_p, uint32_t msg_len);
int32_t recv_msg_sctp(int32_t sockfd, char *recv_msg, uint32_t max_msg_size);
int32_t server_sock_init_tcp(int port, int blocking);
int32_t client_sock_init_tcp(char *cli_ip, uint16_t cli_port, char *serv_ip,
		uint16_t serv_port, int blocking);
int32_t send_msg_tcp(int32_t sockfd, char *msg_send_p, uint32_t msg_send_len);
int32_t recv_msg_tcp(int32_t sockfd, char *recv_msg, uint32_t max_msg_size);

#endif
