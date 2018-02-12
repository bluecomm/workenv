/* SCTP kernel Implementation: User API extensions.
 *
 * connectx.c
 *
 * Distributed under the terms of the LGPL v2.1 as described in
 * http://www.gnu.org/copyleft/lesser.txt.
 *
 * This file is part of the user library that offers support for the
 * SCTP kernel Implementation. The main purpose of this
 * code is to provide the SCTP Socket API mappings for user
 * application to interface with the SCTP in kernel.
 *
 * This implementation is based on the Socket API Extensions for SCTP
 * defined in <draft-ietf-tsvwg-sctpsocket-10.txt.
 *
 * (C) Copyright IBM Corp. 2001, 2005
 *
 * Written or modified by:
 *   Frank Filz     <ffilz@us.ibm.com>
 */

#include <sys/socket.h>   /* struct sockaddr_storage, setsockopt() */
#include <netinet/in.h>
#include "sctp.h" /* SCTP_SOCKOPT_CONNECTX_* */
#include <errno.h>
#include <stdio.h>

/* Support the sctp_connectx() interface.
 *
 * See Sockets API Extensions for SCTP. Section 8.1.
 *
 * Instead of implementing through a socket call in sys_socketcall(),
 * tunnel the request through setsockopt().
 */
static INT32 __connectx_addrsize(const sock_addr_t *addrs, const INT32 addrcnt) {
	VOID *addrbuf;
	sock_addr_t *sa_addr;
	INT32 addrs_size = 0;
	INT32 i;

	addrbuf = (VOID *) addrs;
	for (i = 0; i < addrcnt; i++) {
		sa_addr = (sock_addr_t *) addrbuf;
		switch (sa_addr->sa_family) {
		case AF_INET:
			addrs_size += sizeof(struct sockaddr_in);
			addrbuf += sizeof(struct sockaddr_in);
			break;
		case AF_INET6:
			addrs_size += sizeof(struct sockaddr_in6);
			addrbuf += sizeof(struct sockaddr_in6);
			break;
		default:
			errno = EINVAL;
			return ERROR;
		}
	}

	return addrs_size;
}

INT32 __sctp_connectx(INT32 fd, sock_addr_t *addrs, INT32 addrcnt) {
	socklen_t addrs_size = __connectx_addrsize(addrs, addrcnt);

	if (addrs_size < 0)
		return addrs_size;

	return setsockopt(fd, SOL_SCTP, SCTP_SOCKOPT_CONNECTX_OLD, addrs, addrs_size);
}

extern INT32 sctp_connectx_orig(INT32) __attribute ((alias ("__sctp_connectx")));

INT32 sctp_connectx_new(INT32 fd, sock_addr_t *addrs, INT32 addrcnt, sctp_assoc_t *id) {
	socklen_t addrs_size = __connectx_addrsize(addrs, addrcnt);
	INT32 status;

	if (addrs_size < 0)
		return addrs_size;

	if (id)
		*id = 0;

	status = setsockopt(fd, SOL_SCTP, SCTP_SOCKOPT_CONNECTX, addrs, addrs_size);

	/* the kernel doesn't support the new connectx interface */
	if (status < 0 && errno == ENOPROTOOPT)
		return setsockopt(fd, SOL_SCTP, SCTP_SOCKOPT_CONNECTX_OLD, addrs, addrs_size);

	/* Normalize status and set association id */
	if (status > 0) {
		if (id)
			*id = status;
		status = 0;
	}

	return status;
}

__asm__(".symver __sctp_connectx, sctp_connectx@");
__asm__(".symver sctp_connectx_orig, sctp_connectx@VERS_1");
__asm__(".symver sctp_connectx_new, sctp_connectx@@VERS_2");
