#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <net/sctp/user.h>

#include <sock_init.h>
#include <spu_log.h>

#define log_msg(level, module, fmt, arg...)

/* ****************************************************************************
 *
 * Function:1.This function is used to initialize the sctp socket.
 *          2.it also wait for >the connected gts,blocked in
 *          3.we use the type of SOCK_STREAM
 *
 * Input:NULL.
 *
 * Output:NULL;
 *
 * Retrun:sockfd
 *****************************************************************************/
int32_t server_sock_init_sctp(int port, int blocking) {
	int32_t fd;
	struct sockaddr_in serv_addr;
	int32_t no_nagle = 0;
	int conf_true = 1;

	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	if (fd < 0) {
		log_msg(EROR_LOG, SIMTEST, "socket error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}
	setsockopt(fd, IPPROTO_SCTP, SCTP_NODELAY, &no_nagle, sizeof(int32_t));
	/*we use the option of SCTP_NODELAY to ban the algorthm of nagle*/

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//	gsc_serv_addr.sin_len = sizeof(gsc_serv_addr);
	if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		log_msg(EROR_LOG, SIMTEST, "bind error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	if (ioctl(fd, FIONBIO, (int *) &conf_true) == -1) {
		log_msg(EROR_LOG, SIMTEST, "ioctl error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	listen(fd, SA_MAX_LISTEN_NUM);

	return fd;
}

int32_t client_sock_init_sctp(char *cli_ip, uint16_t cli_port, char *serv_ip,
		uint16_t serv_port, int blocking) {
	int32_t fd;
	struct sockaddr_in cli_addr, serv_addr;
	int32_t no_nagle = 0;
	int conf_true = 1;

	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	if (fd < 0) {
		log_msg(EROR_LOG, SIMTEST, "socket error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}
	setsockopt(fd, IPPROTO_SCTP, SCTP_NODELAY, &no_nagle, sizeof(int32_t));
	/*we use the option of SCTP_NODELAY to ban the algorthm of nagle*/

	if (cli_port > 0) {
		bzero((char *) &cli_addr, sizeof(cli_addr));

		cli_addr.sin_family = AF_INET;
		cli_addr.sin_port = htons(cli_port);

		if (cli_ip != NULL)
			inet_aton(serv_ip, &cli_addr.sin_addr);
		else
			cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(fd, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) < 0) {
			log_msg(EROR_LOG, SIMTEST, "bind error: %d, %s\n", errno,
					strerror(errno));
			return -1;
		}
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(serv_port);
	//sa_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_aton(serv_ip, &serv_addr.sin_addr);

	if (ioctl(fd, FIONBIO, (int *) &conf_true) == -1) {
		log_msg(EROR_LOG, SIMTEST, "ioctl error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	if (connect(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		log_msg(EROR_LOG, SIMTEST, "connect error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	return fd;
}
/******************************************************************************
 *
 * sa_send_msg_sctp:
 *
 * send packet to connected client by sctp.
 *
 * ***************************************************************************/
int32_t send_msg_sctp(int32_t sockfd, char *msg_p, uint32_t msg_len) {
	int32_t sendn;

	/* the specifical struct for senmmsg */
	struct msghdr send_mp;
	struct iovec send_io;

	memset(&send_mp, 0, sizeof(struct msghdr));
	memset(&send_io, 0, sizeof(struct iovec));

	send_io.iov_base = msg_p;
	send_io.iov_len = msg_len;
	send_mp.msg_iov = &send_io;
	send_mp.msg_iovlen = 1;/* !!!!!!!!!!!! */

	sendn = sendmsg(sockfd, &send_mp, 0);

	if (sendn <= 0) {
		log_msg(EROR_LOG, SIMTEST, "sendmsg error: %d, %s\n", errno,
				strerror(errno));
	}
	return sendn;
}

/******************************************************************************
 *
 * sa_recv_msg_sctp:
 *
 * recv packet from connected client by sctp.
 *
 * ***************************************************************************/
int32_t recv_msg_sctp(int32_t sockfd, char *recv_msg, uint32_t max_msg_size) {
	uint32_t recvn;

	struct sockaddr_in peeraddr;
	size_t addr_len = sizeof(struct sockaddr);

	/*the specifical struct for revmsg*/
	struct msghdr recive_mp;
	struct iovec recive_io;

	memset(&recive_mp, 0, sizeof(struct msghdr));
	memset(&recive_io, 0, sizeof(struct iovec));

	recive_io.iov_base = (char *) recv_msg;
	recive_io.iov_len = max_msg_size;
	recive_mp.msg_iov = &recive_io;
	recive_mp.msg_iovlen = 1;/* !!!!!!!!!!!! */

	recvn = recvmsg(sockfd, &recive_mp, 0);
	if (recvn <= 0) {
		//sctp_getpaddrs(sockfd, 0, (struct sockaddr *) (&peeraddr), &addr_len);
		getpeername(sockfd, (struct sockaddr *) (&peeraddr), &addr_len);
		log_msg(EROR_LOG, SIMTEST, "recvmsg error from %s: %d, %s\n",
				inet_ntoa(peeraddr.sin_addr), errno, strerror(errno));
		//return -1;
	}

	return recvn;
}

/******************************************************************************
 *
 * sa_server_sock_init_tcp:
 *
 * init conf server socket use tcp.
 *
 * ***************************************************************************/

int32_t server_sock_init_tcp(int port, int blocking) {
	struct sockaddr_in server_addr;

	int sockfd;

	int conf_true = 1;

	int sndbuf_len = 0;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		log_msg(EROR_LOG, SIMTEST, "socket error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sndbuf_len,
			sizeof(sndbuf_len));

	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr))) {
		log_msg(EROR_LOG, SIMTEST, "bind error: %d  %s\n", errno,
				strerror(errno));
		return -1;
	}

	/* send buf imediately ??? */
	setsockopt(sockfd, IPPROTO_TCP, SO_SNDBUF, &sndbuf_len, sizeof(sndbuf_len));

	if (ioctl(sockfd, FIONBIO, (int *) &conf_true) == -1) {
		log_msg(EROR_LOG, SIMTEST, "ioctl error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	if (listen(sockfd, SA_MAX_LISTEN_NUM)) {
		log_msg(EROR_LOG, SIMTEST, "listen error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	return sockfd;
}

int32_t client_sock_init_tcp(char *cli_ip, uint16_t cli_port, char *serv_ip,
		uint16_t serv_port, int blocking) {

	struct sockaddr_in cli_addr, serv_addr;
	int sockfd;
	int conf_true = 1;
	int sndbuf_len = 0;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		log_msg(EROR_LOG, SIMTEST, "socket error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	if (cli_port > 0) {
		bzero((char *) &cli_addr, sizeof(cli_addr));

		cli_addr.sin_family = AF_INET;
		cli_addr.sin_port = htons(cli_port);

		if (cli_ip != NULL)
			inet_aton(serv_ip, &cli_addr.sin_addr);
		else
			cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) < 0) {
			log_msg(EROR_LOG, SIMTEST, "bind error: %d, %s\n", errno,
					strerror(errno));
			return -1;
		}
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(serv_port);
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_aton(serv_ip, &serv_addr.sin_addr);

	/* send buf imediately ??? */
	setsockopt(sockfd, IPPROTO_TCP, SO_SNDBUF, &sndbuf_len, sizeof(sndbuf_len));

	if (ioctl(sockfd, FIONBIO, (int *) &conf_true) == -1) {
		log_msg(EROR_LOG, SIMTEST, "ioctl error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
			< 0) {
		log_msg(EROR_LOG, SIMTEST, "connect error: %d, %s\n", errno,
				strerror(errno));
		return -1;
	}

	return sockfd;
}
/******************************************************************************
 *
 * sa_send_msg_tcp:
 *
 * send packet to connected client by tcp.
 *
 * ***************************************************************************/

int32_t send_msg_tcp(int32_t sockfd, char *msg_send_p, uint32_t msg_send_len) {
	int sendn;

	sendn = send(sockfd, msg_send_p, msg_send_len, 0);

	if (sendn < 0) {
		log_msg(EROR_LOG, SIMTEST, "send error:%d, %s\n", errno,
				strerror(errno));
	}

	return sendn;
}

/******************************************************************************
 *
 * sa_recv_msg_tcp:
 *
 * recv packet from connected client by tcp.
 *
 * ***************************************************************************/
int32_t recv_msg_tcp(int32_t sockfd, char *recv_msg, uint32_t max_msg_size) {
	int recvn;

	struct sockaddr_in peeraddr;
	size_t addr_len = sizeof(struct sockaddr);

	recvn = recv(sockfd, recv_msg, max_msg_size, 0);
	if (recvn < 0) {
		getpeername(sockfd, (struct sockaddr *) (&peeraddr), &addr_len);
		log_msg(EROR_LOG, SIMTEST, "recv error from %s: %d, %s\n",
				inet_ntoa(peeraddr.sin_addr), errno, strerror(errno));
		return -1;
	}
	return recvn;
}
