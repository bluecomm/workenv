#include "unp.h"
#include <netinet/sctp.h>


void
print_notification(char *notify_buf)
{
	union sctp_notification *snp;
	struct sctp_assoc_change *sac;
	struct sctp_paddr_change *spc;
	struct sctp_remote_error *sre;
	struct sctp_send_failed *ssf;
	struct sctp_shutdown_event *sse;
	struct sctp_adaptation_event *ae;
	struct sctp_pdapi_event *pdapi;
	const char *str;

	snp = (union sctp_notification *)notify_buf;
	switch(snp->sn_header.sn_type) {
	case SCTP_ASSOC_CHANGE:
		sac = &snp->sn_assoc_change;
		switch(sac->sac_state) {
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
		       (uint32_t)sac->sac_assoc_id);
		break;
	case SCTP_PEER_ADDR_CHANGE:
		spc = &snp->sn_paddr_change;
		switch(spc->spc_state) {
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
		       inet_ntoa(((struct sockaddr_in *)&spc->spc_aaddr)->sin_addr),
		       (uint32_t)spc->spc_assoc_id);
		break;
	case SCTP_REMOTE_ERROR:
		sre = &snp->sn_remote_error;
		printf("SCTP_REMOTE_ERROR: assoc=0x%x error=%d\n",
		       (uint32_t)sre->sre_assoc_id, sre->sre_error);
		break;
	case SCTP_SEND_FAILED:
		ssf = &snp->sn_send_failed;
		printf("SCTP_SEND_FAILED: assoc=0x%x error=%d\n",
		       (uint32_t)ssf->ssf_assoc_id, ssf->ssf_error);
		break;
	case SCTP_ADAPTATION_INDICATION:
		ae = &(snp->sn_adaptation_event);
		printf("SCTP_ADAPTION_INDICATION: 0x%x\n",
		    (u_int)(ae->sai_adaptation_ind));
		break;
	case SCTP_PARTIAL_DELIVERY_EVENT:
	    pdapi = &snp->sn_pdapi_event;
	    if(pdapi->pdapi_indication == SCTP_PARTIAL_DELIVERY_ABORTED)
		    printf("SCTP_PARTIAL_DELIEVERY_ABORTED\n");
	    else
		    printf("Unknown SCTP_PARTIAL_DELIVERY_EVENT 0x%x\n",
			   pdapi->pdapi_indication);
	    break;
	case SCTP_SHUTDOWN_EVENT:
		sse = &snp->sn_shutdown_event;
		printf("SCTP_SHUTDOWN_EVENT: assoc=0x%x\n",
		       (uint32_t)sse->sse_assoc_id);
		break;
	default:
		printf("Unknown notification event type=0x%x\n",
		       snp->sn_header.sn_type);
	}
}

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
	evnts.sctp_association_event = 1;
	evnts.sctp_address_event = 1;
	evnts.sctp_send_failure_event = 1;
	evnts.sctp_peer_error_event = 1;
	evnts.sctp_shutdown_event = 1;
	evnts.sctp_partial_delivery_event = 1;
	evnts.sctp_adaptation_layer_event = 1;
	setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

	listen(sock_fd, LISTENQ);
#if 1
	for (;; ) {
		len = sizeof(struct sockaddr_in);
		rd_sz = sctp_recvmsg(sock_fd, readbuf, sizeof(readbuf),
				(SA *)&cliaddr, &len,
				&sri,&msg_flags);

		/******************************************/
		if(msg_flags & MSG_NOTIFICATION) {
			print_notification(readbuf);
			continue;
		}
		/*********************************************/

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
#if 0

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

