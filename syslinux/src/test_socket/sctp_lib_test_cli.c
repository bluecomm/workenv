#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sctp.h"
#include "common_types.h"

#define IP_LOCAL "10.0.0.96"
#define PORT_LOCAL 4096
#define IP_REMOTE "10.0.0.99"
#define PORT_REMOTE 4096

int main() {
	ipv4_sock_addr_t local;
	ipv4_sock_addr_t remote;
	int32_t sockfd;
	INT32 optval_reuse = 1;
	struct sctp_event_subscribe e;
	socklen_t optlen = sizeof(struct sctp_event_subscribe);

	char buf[] = "hello!";

	if ((sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0) {
		printf("failed to create sctp socket: %s\n", strerror(errno));
		return ERROR;
	}

	memset(&local, 0, sizeof(ipv4_sock_addr_t));
	local.sin_family = AF_INET;
	//local.sin_addr.s_addr = htonl(get_local_ipaddr());
	inet_aton(IP_LOCAL, &local.sin_addr);
	local.sin_port = htons(PORT_LOCAL);

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (CHAR *) &optval_reuse,
			sizeof(INT32)) < 0) {
		printf("reuse address failed: %s!\n", strerror(errno));
		return ERROR;
	}

	/* No other events subscribed except Data IO and State Change events */
	memset(&e, 1, sizeof(struct sctp_event_subscribe));

	if (setsockopt(sockfd, SOL_SCTP, SCTP_EVENTS, &e, optlen) < 0) {
		printf("subscribe events failed: %s\n", strerror(errno));
		return ERROR;
	}

	if (sctp_bindx(sockfd, (sock_addr_t *) &local, 1, SCTP_BINDX_ADD_ADDR)
			< 0) {
		printf("failed to bind socket: %s\n", strerror(errno));
		return ERROR;
	}

	if (listen(sockfd, 10) < 0) {
		printf("listen error: %s\n", strerror(errno));
		return ERROR;
	}

	memset(&remote, 0, sizeof(ipv4_sock_addr_t));
	remote.sin_family = AF_INET;
	//remote.sin_addr.s_addr = htonl(grm_net_info_p->grm_serv_info.grm_addr);
	inet_aton(IP_REMOTE, &remote.sin_addr);
	remote.sin_port = htons(PORT_REMOTE);

	while(1){
		if (sctp_sendmsg(sockfd, buf, sizeof(buf),
				(sock_addr_t *) &remote, sizeof(sock_addr_t),
				htonl(getpid()), 0,
				1, 0, 0) <= 0) {
			printf("send msg failed: %s\n", strerror(errno));
		} else {
			printf("send msg ok !\n");
		}

		sleep(1);

	}

	return 0;
}
