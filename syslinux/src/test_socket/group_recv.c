#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <stdlib.h>

#define RECEIVE_BUF_LENGTH 1024
#define BROADCAST_UDP_PORT 18041
#define BROADCAST_UDP_IP   "224.0.0.41"

#define TRANSFER_UDP_PORT  8041
#define SERVER_UDP_IP  "10.0.0.218"

#define CLIENT_UDP_IP  "10.0.0.205"

socklen_t sockaddr_len = sizeof(struct sockaddr_in);
struct sockaddr_in broadcast_serv_addr;
struct sockaddr_in udp_serv_addr;
struct sockaddr_in udp_client_addr;

int32_t print_memory(uint8_t *msg_p, uint32_t len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		if (i % 4 == 0) {
			if (i != 0) {
				printf(" ");
				if (i % 32 == 0)
					printf("\n");
			}
		}
		printf("%02x ", msg_p[i]);
	}
	printf("\nlen = %d (0x%x)\n", len, len);

	return 0;
}

static int broadcast_read(int fd) {
	printf("******************* recv broadcast ********************\n");
	int rc = -1;
	uint8_t msg_buf[RECEIVE_BUF_LENGTH] = { 0 };

	while (1) {
		rc = recvfrom(fd, msg_buf, RECEIVE_BUF_LENGTH, 0,
				(struct sockaddr *) &broadcast_serv_addr, &sockaddr_len);

		if (rc <= 0) {
			printf("recvfrom error: %d, %s\n", errno, strerror(errno));
			return -1;
		}

		print_memory(msg_buf, rc);
	}
	return 0;
}

static int broadcast_open() {
	int rc;
	int optval = 1;
	struct ip_mreq ipMreq;

	int broadcast_fd = 0;

	if ((broadcast_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket error: %d\n", errno, strerror(errno));
		return -1;
	}

	if (setsockopt(broadcast_fd, SOL_SOCKET, SO_REUSEADDR,
			(const char *) &optval, sizeof(int)) < 0) {
		printf("broadcast setsockopt IP_ADD_MEMBERSHIP error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	bzero((char *) &broadcast_serv_addr, sizeof(struct sockaddr_in));

	broadcast_serv_addr.sin_family = AF_INET;
	broadcast_serv_addr.sin_addr.s_addr = INADDR_ANY;
	broadcast_serv_addr.sin_port = htons(BROADCAST_UDP_PORT);

	if (bind(broadcast_fd, (struct sockaddr *) &broadcast_serv_addr,
			sockaddr_len) < 0) {
		printf("broadcast bind error: %d, %s\n", errno, strerror(errno));
		return -1;
	}

	ipMreq.imr_multiaddr.s_addr = inet_addr(BROADCAST_UDP_IP);
	ipMreq.imr_interface.s_addr = inet_addr(CLIENT_UDP_IP);

	if (setsockopt(broadcast_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
			(int8_t *) &ipMreq, sizeof(ipMreq)) < 0) {
		printf("broadcast setsockopt IP_ADD_MEMBERSHIP error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	return broadcast_fd;
}

int main() {

	int fd;
	fd = broadcast_open();
	broadcast_read(fd);
	return 0;
}
