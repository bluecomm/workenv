#include "unp.h"
#include <netinet/sctp.h>

#define	SCTP_MAXLINE	800

/* include sock_ntop */
char *
sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char		portstr[8];
    static char str[128];		/* Unix domain is largest */

	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return(NULL);
		if (ntohs(sin->sin_port) != 0) {
			snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
			strcat(str, portstr);
		}
		return(str);
	}
/* end sock_ntop */

#ifdef	IPV6
	case AF_INET6: {
		struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

		str[0] = '[';
		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
			return(NULL);
		if (ntohs(sin6->sin6_port) != 0) {
			snprintf(portstr, sizeof(portstr), "]:%d", ntohs(sin6->sin6_port));
			strcat(str, portstr);
			return(str);
		}
		return (str + 1);
	}
#endif

#ifdef	AF_UNIX
	case AF_UNIX: {
		struct sockaddr_un	*unp = (struct sockaddr_un *) sa;

			/* OK to have no pathname bound to the socket: happens on
			   every connect() unless client calls bind() first. */
		if (unp->sun_path[0] == 0)
			strcpy(str, "(no pathname bound)");
		else
			snprintf(str, sizeof(str), "%s", unp->sun_path);
		return(str);
	}
#endif

#ifdef	HAVE_SOCKADDR_DL_STRUCT
	case AF_LINK: {
		struct sockaddr_dl	*sdl = (struct sockaddr_dl *) sa;

		if (sdl->sdl_nlen > 0)
			snprintf(str, sizeof(str), "%*s (index %d)",
					 sdl->sdl_nlen, &sdl->sdl_data[0], sdl->sdl_index);
		else
			snprintf(str, sizeof(str), "AF_LINK, index=%d", sdl->sdl_index);
		return(str);
	}
#endif
	default:
		snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
				 sa->sa_family, salen);
		return(str);
	}
    return (NULL);
}


char *
Sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
	char	*ptr;

	if ( (ptr = sock_ntop(sa, salen)) == NULL)
		err_sys("sock_ntop error");	/* inet_ntop() sets errno */
	return(ptr);
}

void sctp_print_addresses(struct sockaddr_storage *addrs, int num) {
	struct sockaddr_storage *ss;
	int i, salen;

	ss = addrs;
	for (i = 0; i < num; i++) {
		printf("%s\n", sock_ntop((SA *) ss, salen));
#ifdef HAVE_SOCKADDR_SA_LEN
		salen = ss->ss_len;
#else
		switch (ss->ss_family) {
		case AF_INET:
			salen = sizeof(struct sockaddr_in);
			break;
#ifdef IPV6
			case AF_INET6:
			salen = sizeof(struct sockaddr_in6);
			break;
#endif
		default:
			err_quit("sctp_print_addresses: unknown AF");
			break;
		}
#endif
		ss = (struct sockaddr_storage *) ((char *) ss + salen);
	}
}

void check_notification(int sock_fd, char *recvline, int rd_len) {
	union sctp_notification *snp;
	struct sctp_assoc_change *sac;
	struct sockaddr_storage *sal, *sar;
	int num_rem, num_loc;

	snp = (union sctp_notification *) recvline;
	if (snp->sn_header.sn_type == SCTP_ASSOC_CHANGE) {
		sac = &snp->sn_assoc_change;
		if ((sac->sac_state == SCTP_COMM_UP)
				|| (sac->sac_state == SCTP_RESTART)) {
			num_rem = sctp_getpaddrs(sock_fd, sac->sac_assoc_id, (struct sockaddr **)(&sar));
			printf("There are %d remote addresses and they are:\n", num_rem);
//			printf("%s: %d\n", __FILE__, __LINE__);
			sctp_print_addresses(sar, num_rem);
//			printf("%s: %d\n", __FILE__, __LINE__);
			sctp_freepaddrs((struct sockaddr *)sar);
//			printf("%s: %d\n", __FILE__, __LINE__);

			num_loc = sctp_getladdrs(sock_fd, sac->sac_assoc_id, (struct sockaddr **)(&sal));
			printf("There are %d local addresses and they are:\n", num_loc);
//			printf("%s: %d\n", __FILE__, __LINE__);
			sctp_print_addresses(sal, num_loc);
//			printf("%s: %d\n", __FILE__, __LINE__);
			sctp_freeladdrs((struct sockaddr *)sal);
//			printf("%s: %d\n", __FILE__, __LINE__);
		}
	}
}

void sctpstr_cli(FILE *fp, int sock_fd, struct sockaddr *to, socklen_t tolen) {
	struct sockaddr_in peeraddr;
	struct sctp_sndrcvinfo sri;
	char sendline[MAXLINE], recvline[MAXLINE];
	socklen_t len;
	int out_sz, rd_sz;
	int msg_flags;

	bzero(&sri, sizeof(sri));
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		if (sendline[0] != '[') {
			printf("Error, line must be of the form '[streamnum]text'\n");
			continue;
		}
		sri.sinfo_stream = strtol(&sendline[1], NULL, 0);
		out_sz = strlen(sendline);
		sctp_sendmsg(sock_fd, sendline, out_sz, to, tolen, 0, 0,
				sri.sinfo_stream, 0, 0);
		/* include mod_strcli1 */
		do {
			len = sizeof(peeraddr);
			rd_sz = sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
					(SA *) &peeraddr, &len, &sri, &msg_flags);
			if (msg_flags & MSG_NOTIFICATION)
				check_notification(sock_fd, recvline, rd_sz);
		} while (msg_flags & MSG_NOTIFICATION);
		printf("From str:%d seq:%d (assoc:0x%x):", sri.sinfo_stream,
				sri.sinfo_ssn, (u_int) sri.sinfo_assoc_id);
		printf("%.*s", rd_sz, recvline);
		/* end mod_strcli1 */
	}
}

void sctpstr_cli_echoall(FILE *fp, int sock_fd, struct sockaddr *to,
		socklen_t tolen) {
	struct sockaddr_in peeraddr;
	struct sctp_sndrcvinfo sri;
	char sendline[SCTP_MAXLINE], recvline[SCTP_MAXLINE];
	socklen_t len;
	int rd_sz, i, strsz;
	int msg_flags;

	bzero(sendline, sizeof(sendline));
	bzero(&sri, sizeof(sri));
	while (fgets(sendline, SCTP_MAXLINE - 9, fp) != NULL) {
		strsz = strlen(sendline);
		if (sendline[strsz - 1] == '\n') {
			sendline[strsz - 1] = '\0';
			strsz--;
		}
		for (i = 0; i < SERV_MAX_SCTP_STRM; i++) {
			snprintf(sendline + strsz, sizeof(sendline) - strsz, ".msg.%d", i);
			sctp_sendmsg(sock_fd, sendline, sizeof(sendline), to, tolen, 0, 0,
					i, 0, 0);
		}
		for (i = 0; i < SERV_MAX_SCTP_STRM; i++) {
			len = sizeof(peeraddr);
			rd_sz = sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
					(SA *) &peeraddr, &len, &sri, &msg_flags);
			printf("From str:%d seq:%d (assoc:0x%x):", sri.sinfo_stream,
					sri.sinfo_ssn, (u_int) sri.sinfo_assoc_id);
			printf("%.*s\n", rd_sz, recvline);
		}
	}
}

void sctpstr_cli_echoall2(FILE *fp, int sock_fd, struct sockaddr *to,
		socklen_t tolen) {
	struct sockaddr_in peeraddr;
	struct sctp_sndrcvinfo sri;
	char sendline[SCTP_MAXLINE], recvline[SCTP_MAXLINE];
	socklen_t len;
	int rd_sz, i, strsz;
	int msg_flags;

	bzero(sendline, sizeof(sendline));
	bzero(&sri, sizeof(sri));
	while (fgets(sendline, SCTP_MAXLINE - 9, fp) != NULL) {
		strsz = strlen(sendline);
		if (sendline[strsz - 1] == '\n') {
			sendline[strsz - 1] = '\0';
			strsz--;
		}
		/* include modified_echo */
		for (i = 0; i < SERV_MAX_SCTP_STRM; i++) {
			snprintf(sendline + strsz, sizeof(sendline) - strsz, ".msg.%d 1",
					i);
			sctp_sendmsg(sock_fd, sendline, sizeof(sendline), to, tolen, 0, 0,
					i, 0, 0);
			snprintf(sendline + strsz, sizeof(sendline) - strsz, ".msg.%d 2",
					i);
			sctp_sendmsg(sock_fd, sendline, sizeof(sendline), to, tolen, 0, 0,
					i, 0, 0);
		}
		for (i = 0; i < SERV_MAX_SCTP_STRM * 2; i++) {
			len = sizeof(peeraddr);
			/* end modified_echo */
			rd_sz = sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
					(SA *) &peeraddr, &len, &sri, &msg_flags);
			printf("From str:%d seq:%d (assoc:0x%x):", sri.sinfo_stream,
					sri.sinfo_ssn, (u_int) sri.sinfo_assoc_id);
			printf("%.*s\n", rd_sz, recvline);
		}
	}
}

int main(int argc, char **argv) {
	int sock_fd;
	struct sockaddr_in servaddr;
	struct sctp_event_subscribe evnts;
	int echo_to_all = 0;

	if (argc < 2)
		err_quit("Missing host argument - use '%s host [echo]'\n", argv[0]);
	if (argc > 2) {
		printf("Echoing messages to all streams\n");
		echo_to_all = 1;
	}
	sock_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	/* include mod_client04 */
	bzero(&evnts, sizeof(evnts));
	evnts.sctp_data_io_event = 1;
	evnts.sctp_association_event = 1;
	setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

	sctpstr_cli(stdin, sock_fd, (SA *) &servaddr, sizeof(servaddr));
	/* end mod_client04 */

#if 0
	if (echo_to_all == 0)
	sctpstr_cli(stdin, sock_fd, (SA *) &servaddr, sizeof(servaddr));
	else
	sctpstr_cli_echoall2(stdin, sock_fd, (SA *) &servaddr,
			sizeof(servaddr));
#endif

#if 0
	char byemsg[10];
	/* include modified_client02 */
	if(echo_to_all == 0)
	sctpstr_cli(stdin,sock_fd,(SA *)&servaddr,sizeof(servaddr));
	else
	sctpstr_cli_echoall2(stdin,sock_fd,(SA *)&servaddr,sizeof(servaddr));
	strcpy(byemsg,"goodbye");
	sctp_sendmsg(sock_fd, byemsg, strlen(byemsg),
			(SA *)&servaddr, sizeof(servaddr),
			0,
			SCTP_ABORT,
			0, 0, 0);

#endif
	close(sock_fd);
	return (0);
}
