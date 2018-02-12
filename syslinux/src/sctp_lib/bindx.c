/* SCTP kernel Implementation: User API extensions.
 *
 * bindx.c
 *
 * Distributed under the terms of the LGPL v2.1 as described in
 *    http://www.gnu.org/copyleft/lesser.txt 
 *
 * This file is part of the user library that offers support for the
 * SCTP kernel Implementation. The main purpose of this
 * code is to provide the SCTP Socket API mappings for user
 * application to interface with the SCTP in kernel.
 *
 * This implementation is based on the Socket API Extensions for SCTP
 * defined in <draft-ietf-tsvwg-sctpsocket-10.txt.
 *
 * (C) Copyright IBM Corp. 2001, 2003
 * Copyright (c) 2002 Intel Corporation.
 *
 * Written or modified by:
 *   La Monte H.P. Yarroll <piggy@acm.org>
 *   Daisy Chang           <daisyc@us.ibm.com>
 *   Inaky Perez-Gonzalez  <inaky.gonzalez@intel.com>
 *   Sridhar Samudrala     <sri@us.ibm.com>
 */

#include <sys/socket.h>   /* struct sockaddr_storage, setsockopt() */
#include <netinet/in.h>
#include "sctp.h" /* SCTP_SOCKOPT_BINDX_* */
#include <errno.h>

/* Support the sctp_bindx() interface.
 *
 * See Sockets API Extensions for SCTP. Section 8.1.
 *
 * Instead of implementing through a socket call in sys_socketcall(),
 * tunnel the request through setsockopt().
 */
INT32 sctp_bindx(INT32 fd, sock_addr_t *addrs, INT32 addrcnt, INT32 flags) {
	INT32 setsock_option = 0;
	VOID *addrbuf;
	sock_addr_t *sa_addr;
	socklen_t addrs_size = 0;
	INT32 i;

	switch (flags) {
	case SCTP_BINDX_ADD_ADDR:
		setsock_option = SCTP_SOCKOPT_BINDX_ADD;
		break;
	case SCTP_BINDX_REM_ADDR:
		setsock_option = SCTP_SOCKOPT_BINDX_REM;
		break;
	default:
		errno = EINVAL;
		return ERROR;
	}

	addrbuf = addrs;
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

	return setsockopt(fd, SOL_SCTP, setsock_option, addrs, addrs_size);
}
