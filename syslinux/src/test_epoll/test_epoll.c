#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

#define MAX_EVENTS 256


int udp_init(){
	int sockfd;
	struct sockaddr_in server_addr;
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sockfd < 0) {
		printf("socket error: %d, %s\n", errno, strerror(errno));
		return -1;
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(8190);

	int reuseaddr = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("bind error: %d, %s\n",
				errno, strerror(errno));
		return -1;
	}
	return sockfd;
}


int tcp_init(){
	int sockfd;
	struct sockaddr_in server_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sockfd < 0) {
		printf("socket error: %d, %s\n", errno, strerror(errno));
		return -1;
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(8190);

	int reuseaddr = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("bind error: %d, %s\n",
				errno, strerror(errno));
		return -1;
	}

	if (listen(sockfd, 10) < 0){
		printf("bind error: %d, %s\n",
				errno, strerror(errno));
		return -1;
	}
	return sockfd;
}


int sctp_init(){
	int sockfd;
	struct sockaddr_in server_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

	if (sockfd < 0) {
		printf("socket error: %d, %s\n", errno, strerror(errno));
		return -1;
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(8190);

	int reuseaddr = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("bind error: %d, %s\n",
				errno, strerror(errno));
		return -1;
	}

	if (listen(sockfd, 10) < 0){
		printf("bind error: %d, %s\n",
				errno, strerror(errno));
		return -1;
	}
	return sockfd;
}

int read_data(int fd){
	char recv_buf[256];
	memset(recv_buf, 0, sizeof(recv_buf));
	if (read(fd, recv_buf, sizeof(recv_buf)) <= 0){
		//printf("read error: %d, %s\n", errno, strerror(errno));
		perror("read error");
		return -1;
	}else {
		printf("receiced: %s\n", recv_buf);
		return 0;
	}
}

int write_data(int fd){
	char recv_buf[256];
	memset(recv_buf, 0, sizeof(recv_buf));
	if (write(fd, recv_buf, sizeof(recv_buf)) <= 0){
		//printf("write error: %d, %s\n", errno, strerror(errno));
		perror("write error");
		return -1;
	}else{
		printf("receiced: %s\n", recv_buf);
		return 0;
	}
}


int main(){
	struct epoll_event ev, events[MAX_EVENTS];
	int listen_sock, conn_sock, nfds, epollfd;

	struct sockaddr_in cli_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	int n = 0, fd;
	//int non_block = 1;

	listen_sock = tcp_init();

	epollfd = epoll_create(256);
	if (epollfd == -1) {
	    perror("epoll_create");
	    exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
	    perror("epoll_ctl: listen_sock");
	    exit(EXIT_FAILURE);
	}

	for (;;) {
	    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
	    if (nfds == -1) {
	        perror("epoll_pwait");
	        exit(EXIT_FAILURE);
	    }

	    printf("nfds: %d\n", nfds);

	    for (n = 0; n < nfds; ++n) {
	        if (events[n].data.fd == listen_sock) {
	            conn_sock = accept(listen_sock,
	                            (struct sockaddr *) &cli_addr, &addrlen);
	            if (conn_sock == -1) {
	                perror("accept");
	                exit(EXIT_FAILURE);
	            }
	            printf("recv from %s:%d\n",
	            		inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
	            //setnonblocking(conn_sock);
	            fcntl(conn_sock, F_SETFL, O_NONBLOCK);
	            ev.events = EPOLLIN | EPOLLET;
	            ev.data.fd = conn_sock;
	            if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
	                        &ev) == -1) {
	                perror("epoll_ctl: conn_sock");
	                exit(EXIT_FAILURE);
	            }
	        } else if (events[n].events && EPOLLIN){
	        	fd = events[n].data.fd;
	            if (read_data(fd) != 0){
	            	close(fd);
	            	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
	            }
	        } else if  (events[n].events && EPOLLOUT){
	        	printf("write\n");
	        }
	    }
	}
}
