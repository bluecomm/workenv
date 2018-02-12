#include "unp.h"
#include <netinet/sctp.h>
#include <error.h>
#include <string.h>

void print_notification(char *notify_buf) {
	union sctp_notification *snp;
	struct sctp_assoc_change *sac;
	struct sctp_paddr_change *spc;
	struct sctp_remote_error *sre;
	struct sctp_send_failed *ssf;
	struct sctp_shutdown_event *sse;
	struct sctp_adaptation_event *ae;
	struct sctp_pdapi_event *pdapi;
	const char *str;

	snp = (union sctp_notification *) notify_buf;
	switch (snp->sn_header.sn_type) {
	case SCTP_ASSOC_CHANGE:
		sac = &snp->sn_assoc_change;
		switch (sac->sac_state) {
		case SCTP_COMM_UP:
			str = "COMMUNICATION UP";
			break;
		case SCTP_COMM_LOST:
			str = "COMMUNICATION LOST";
			break;
		case SCTP_RESTART:
			str = "RESTART";
			break;
		case SCTP_SHUTDOWN_COMP:
			str = "SHUTDOWN COMPLETE";
			break;
		case SCTP_CANT_STR_ASSOC:
			str = "CAN'T START ASSOC";
			break;
		default:
			str = "UNKNOWN";
			break;
		} /* end switch(sac->sac_state) */
		printf("SCTP_ASSOC_CHANGE: %s, assoc=0x%x\n", str,
				(uint32_t) sac->sac_assoc_id);
		break;
	case SCTP_PEER_ADDR_CHANGE:
		spc = &snp->sn_paddr_change;
		switch (spc->spc_state) {
		case SCTP_ADDR_AVAILABLE:
			str = "ADDRESS AVAILABLE";
			break;
		case SCTP_ADDR_UNREACHABLE:
			str = "ADDRESS UNREACHABLE";
			break;
		case SCTP_ADDR_REMOVED:
			str = "ADDRESS REMOVED";
			break;
		case SCTP_ADDR_ADDED:
			str = "ADDRESS ADDED";
			break;
		case SCTP_ADDR_MADE_PRIM:
			str = "ADDRESS MADE PRIMARY";
			break;
		default:
			str = "UNKNOWN";
			break;
		} /* end switch(spc->spc_state) */
		printf("SCTP_PEER_ADDR_CHANGE: %s, addr=%s, assoc=0x%x\n", str,
				inet_ntoa(((struct sockaddr_in *) &spc->spc_aaddr)->sin_addr),
				(uint32_t) spc->spc_assoc_id);
		break;
	case SCTP_REMOTE_ERROR:
		sre = &snp->sn_remote_error;
		printf("SCTP_REMOTE_ERROR: assoc=0x%x error=%d\n",
				(uint32_t) sre->sre_assoc_id, sre->sre_error);
		break;
	case SCTP_SEND_FAILED:
		ssf = &snp->sn_send_failed;
		printf("SCTP_SEND_FAILED: assoc=0x%x error=%d\n",
				(uint32_t) ssf->ssf_assoc_id, ssf->ssf_error);
		break;
	case SCTP_ADAPTATION_INDICATION:
		ae = &(snp->sn_adaptation_event);
		printf("SCTP_ADAPTION_INDICATION: 0x%x\n",
				(u_int) (ae->sai_adaptation_ind));
		break;
	case SCTP_PARTIAL_DELIVERY_EVENT:
		pdapi = &snp->sn_pdapi_event;
		if (pdapi->pdapi_indication == SCTP_PARTIAL_DELIVERY_ABORTED)
			printf("SCTP_PARTIAL_DELIEVERY_ABORTED\n");
		else
			printf("Unknown SCTP_PARTIAL_DELIVERY_EVENT 0x%x\n",
					pdapi->pdapi_indication);
		break;
	case SCTP_SHUTDOWN_EVENT:
		sse = &snp->sn_shutdown_event;
		printf("SCTP_SHUTDOWN_EVENT: assoc=0x%x\n",
				(uint32_t) sse->sse_assoc_id);
		break;
	default:
		printf("Unknown notification event type=0x%x\n",
				snp->sn_header.sn_type);
	}
}


sctp_assoc_t sctp_address_to_associd(int sock_fd, struct sockaddr *sa,
		socklen_t salen) {
	struct sctp_paddrparams sp;
	int siz;

	siz = sizeof(struct sctp_paddrparams);

	bzero(&sp, siz);
	memcpy(&sp.spp_address, sa, salen);

	sctp_opt_info(sock_fd, 0, SCTP_PEER_ADDR_PARAMS, &sp, &siz);

	return (sp.spp_assoc_id);
}

ssize_t /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n) {
	size_t nleft, nwritten;
	const char *ptr;

	ptr = vptr; /* can't do pointer arithmetic on void* */
	nleft = n;
	while (nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
			return (nwritten); /* error */

		nleft -= nwritten;
		ptr += nwritten;
	}
	return (n);
}

void str_echo(int sockfd) {
	err_msg("%s,%d: %s() begin", __FILE__, __LINE__, __FUNCTION__);
	ssize_t n;
	char buf[MAXLINE];

	again: while ((n = read(sockfd, buf, MAXLINE)) > 0) {
		//err_msg("%s,%d: %s()", __FILE__, __LINE__, __FUNCTION__);
		writen(sockfd, buf, n);
	}

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");

	err_msg("%s,%d: %s() end", __FILE__, __LINE__, __FUNCTION__);
	return;
}


void sig_wait(int sign){
	int status;
	int pid = wait(&status);
	err_msg("process %d exit with status %d", pid, status);
}

int main(int argc, char **argv) {
	int sock_fd, msg_flags, connfd, childpid;
	sctp_assoc_t assoc;
	char readbuf[BUFFSIZE];
	struct sockaddr_in servaddr, cliaddr;
	struct sctp_sndrcvinfo sri;
	struct sctp_event_subscribe evnts;
	socklen_t len;
	size_t rd_sz;

	fd_set rset;


	struct sigaction act;

	signal(SIGCHLD, sig_wait);

	sock_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(sock_fd, (SA *) &servaddr, sizeof(servaddr));

	bzero(&evnts, sizeof(evnts));
	evnts.sctp_data_io_event = 1;
	evnts.sctp_association_event = 1;
	evnts.sctp_address_event = 1;
	evnts.sctp_send_failure_event = 1;
	evnts.sctp_peer_error_event = 1;
	evnts.sctp_shutdown_event = 1;
	evnts.sctp_partial_delivery_event = 1;
	evnts.sctp_adaptation_layer_event = 1;
	setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

	listen(sock_fd, LISTENQ);

	/* include mod_servfork */
	FD_ZERO(&rset);

	for (;;) {
		FD_SET(sock_fd, &rset);
		select(sock_fd + 1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sock_fd, &rset)) { /* socket is readable */
			len = sizeof(struct sockaddr_in);
			rd_sz = sctp_recvmsg(sock_fd, readbuf, sizeof(readbuf),
					(SA *) &cliaddr, &len, &sri, &msg_flags);

			if (rd_sz <= 0) {
				continue;
			}

			if (msg_flags & MSG_NOTIFICATION) {
				print_notification(readbuf);
				continue;
			}

			sctp_sendmsg(sock_fd, readbuf, rd_sz, (SA *) &cliaddr, len,
					sri.sinfo_ppid, sri.sinfo_flags, sri.sinfo_stream, 0, 0);

			/* cannot work */
			//assoc = sctp_address_to_associd(sock_fd, (SA *) &cliaddr, len);
			/* can work */
			assoc = sri.sinfo_assoc_id;
			if ((int) assoc == 0) {
				err_ret("Can't get association id");
				continue;
			}

			connfd = sctp_peeloff(sock_fd, assoc);
			if (connfd == -1) {
				err_ret("sctp_peeloff fails");
				continue;
			}

			printf("peeroff ok\n");

			if ((childpid = fork()) == 0) {
				//printf("child process\n");
				close(sock_fd);
				str_echo(connfd);
				exit(0);
			} else {
				err_msg("fork ok, childpid: %d",childpid);
				close(connfd);
			}
		}
		/* end mod_servfork */
	}
}

