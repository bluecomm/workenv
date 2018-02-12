#include "unp.h"
#include <netinet/sctp.h>

int main(int argc, char **argv) {
	int sock_fd, msg_flags;
	char readbuf[BUFFSIZE];
	struct sockaddr_in servaddr, cliaddr;
	struct sctp_sndrcvinfo sri;
	struct sctp_event_subscribe evnts;

	int stream_increment = 1;
	socklen_t len;
	size_t rd_sz;

	if (argc == 2)
		stream_increment = atoi(argv[1]);
	sock_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
	bzero(&servaddr, sizeof(servaddr));

#if 1
	/*************************************************************/
	/* set max associate number */
	struct sctp_initmsg initm;
	bzero(&initm, sizeof(initm));
	initm.sinit_num_ostreams = SERV_MORE_STRMS_SCTP;
	setsockopt(sock_fd, IPPROTO_SCTP, SCTP_INITMSG, &initm, sizeof(initm));
	/*************************************************************/
	/* set auto close time */
	int close_time = 120;
	setsockopt(sock_fd, IPPROTO_SCTP, SCTP_AUTOCLOSE,
		   &close_time, sizeof(close_time));

#endif

	servaddr.sin_family = AF_INET;

	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(sock_fd, (SA *) &servaddr, sizeof(servaddr));

	bzero(&evnts, sizeof(evnts));
	evnts.sctp_data_io_event = 1;
	setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

	listen(sock_fd, LISTENQ);
#if 0
	for (;; ) {
		len = sizeof(struct sockaddr_in);
		rd_sz = sctp_recvmsg(sock_fd, readbuf, sizeof(readbuf),
				(SA *)&cliaddr, &len,
				&sri,&msg_flags);

		if(stream_increment) {
			sri.sinfo_stream++;
			if(sri.sinfo_stream >= 65535/*sctp_get_no_strms(sock_fd,(SA *)&cliaddr, len)*/)
			sri.sinfo_stream = 0;
		}
		sctp_sendmsg(sock_fd, readbuf, rd_sz,
				(SA *)&cliaddr, len,
				sri.sinfo_ppid,
				sri.sinfo_flags,
				sri.sinfo_stream,
				0, 0);
	}
#endif
#if 1

	for (;;) {
		len = sizeof(struct sockaddr_in);
		rd_sz = sctp_recvmsg(sock_fd, readbuf, sizeof(readbuf), (SA *) &cliaddr,
				&len, &sri, &msg_flags);
		if (stream_increment) {
			sri.sinfo_stream++;
			if (sri.sinfo_stream >= 65535)
				sri.sinfo_stream = 0;
		}
//		if (rd_sz > 0)
//			printf("recv: %s\n", readbuf);

		sctp_sendmsg(sock_fd, readbuf, rd_sz, (SA *) &cliaddr, len,
				sri.sinfo_ppid, (sri.sinfo_flags | SCTP_EOF), sri.sinfo_stream,
				0, 0);
	}
#endif
}

