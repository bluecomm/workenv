/* SCTP kernel Implementation: User API extensions.
 *
 * addrs.c
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
 * (C) Copyright IBM Corp. 2003
 * Copyright (c) 2001-2002 Intel Corp.
 *
 * Written or modified by:
 *  Ardelle Fan     <ardelle.fan@intel.com>
 *  Sridhar Samudrala <sri@us.ibm.com>
 *  Ivan Skytte Jørgensen <isj-sctp@i1.dk>
 */

#include <malloc.h>
#include <netinet/in.h>
#include "sctp.h"
#include <string.h>
#include <errno.h>

/* 
 * Get local/peer addresses using the old API 
 * Old kernels (2.6.13 and earlier) only support this API but it breaks 32-bit
 * programs on 64-bit kernels.
 */
static INT32 sctp_getaddrs_old(INT32 sd, sctp_assoc_t id, INT32 optname_num,
		INT32 optname_old, sock_addr_t **addrs) {
	socklen_t len = sizeof(sctp_assoc_t);
	INT32 cnt = ERROR, err = ERROR;
	struct sctp_getaddrs_old getaddrs;

	cnt = getsockopt(sd, SOL_SCTP, optname_num, &id, &len);
	if (cnt < 0)
		return ERROR;

	if (0 == cnt) {
		*addrs = NULL;
		return OK;
	}

	len = cnt * sizeof(struct sockaddr_in6);

	getaddrs.assoc_id = id;
	getaddrs.addr_num = cnt;
	getaddrs.addrs = (sock_addr_t *) malloc(len);
	if (NULL == getaddrs.addrs)
		return ERROR;

	len = sizeof(getaddrs);
	err = getsockopt(sd, SOL_SCTP, optname_old, &getaddrs, &len);
	if (err < 0) {
		free(getaddrs.addrs);
		return ERROR;
	}

	*addrs = getaddrs.addrs;

	return getaddrs.addr_num;

} /* sctp_getaddrs_old() */

/* 
 * Common getsockopt() layer 
 * If the NEW getsockopt() API fails this function will fall back to using
 * the old API
 */
static INT32 sctp_getaddrs(INT32 sd, sctp_assoc_t id, INT32 optname_new,
		INT32 optname_num_old, INT32 optname_old, sock_addr_t **addrs) {
	INT32 cnt, err;
	socklen_t len;
	size_t bufsize = 4096; /*enough for most cases*/

	struct sctp_getaddrs *getaddrs = (struct sctp_getaddrs*) malloc(bufsize);
	if (!getaddrs)
		return ERROR;

	for (;;) {
		CHAR *new_buf;

		len = bufsize;
		getaddrs->assoc_id = id;
		err = getsockopt(sd, SOL_SCTP, optname_new, getaddrs, &len);
		if (err == 0) {
			/*got it*/
			break;
		}
		if (errno == ENOPROTOOPT) {
			/*Kernel does not support new API*/
			free(getaddrs);
			return sctp_getaddrs_old(sd, id, optname_num_old, optname_old, addrs);
		}
		if (errno != ENOMEM) {
			/*unknown error*/
			free(getaddrs);
			return ERROR;
		}
		/*expand buffer*/
		if (bufsize > 128 * 1024) {
			/*this is getting ridiculous*/
			free(getaddrs);
			errno = ENOBUFS;
			return ERROR;
		}
		new_buf = realloc(getaddrs, bufsize + 4096);
		if (!new_buf) {
			free(getaddrs);
			return ERROR;
		}
		bufsize += 4096;
		getaddrs = (struct sctp_getaddrs*) new_buf;
	}

	/* we skip traversing the list, allocating a new buffer etc. and enjoy
	 * a simple hack*/
	cnt = getaddrs->addr_num;
	memmove(getaddrs, getaddrs + 1, len);
	*addrs = (sock_addr_t*) getaddrs;

	return cnt;
} /* sctp_getaddrs() */

/* Get all peer address on a socket.  This is a new SCTP API
 * described in the section 8.3 of the Sockets API Extensions for SCTP.
 * This is implemented using the getsockopt() interface.
 */
INT32 sctp_getpaddrs(INT32 sd, sctp_assoc_t id, sock_addr_t **addrs) {
	return sctp_getaddrs(sd, id, SCTP_GET_PEER_ADDRS, SCTP_GET_PEER_ADDRS_NUM_OLD,
			SCTP_GET_PEER_ADDRS_OLD, addrs);
} /* sctp_getpaddrs() */

/* Frees all resources allocated by sctp_getpaddrs().  This is a new SCTP API
 * described in the section 8.4 of the Sockets API Extensions for SCTP.
 */
INT32 sctp_freepaddrs(sock_addr_t *addrs) {
	free(addrs);
	return OK;

} /* sctp_freepaddrs() */

/* Get all locally bound address on a socket.  This is a new SCTP API
 * described in the section 8.5 of the Sockets API Extensions for SCTP.
 * This is implemented using the getsockopt() interface.
 */
INT32 sctp_getladdrs(INT32 sd, sctp_assoc_t id, sock_addr_t **addrs) {
	return sctp_getaddrs(sd, id, SCTP_GET_LOCAL_ADDRS, SCTP_GET_LOCAL_ADDRS_NUM_OLD,
			SCTP_GET_LOCAL_ADDRS_OLD, addrs);
} /* sctp_getladdrs() */

/* Frees all resources allocated by sctp_getladdrs().  This is a new SCTP API
 * described in the section 8.6 of the Sockets API Extensions for SCTP.
 */
INT32 sctp_freeladdrs(sock_addr_t *addrs) {
	free(addrs);
	return OK;

} /* sctp_freeladdrs() */

INT32 sctp_getaddrlen(sa_family_t family) {
	/* We could call into the kernel to see what it thinks the size should
	 * be, but hardcoding the address families here is: (a) faster,
	 * (b) easier, and (c) probably good enough for forseeable future.
	 */
	switch (family) {
	case AF_INET:
		return sizeof(struct sockaddr_in);
	case AF_INET6:
		return sizeof(struct sockaddr_in6);
	default:
		/* Currently there is no defined error handling in
		 * draft-ietf-tsvwg-sctpsocket-13.txt.
		 * -1 might cause the application to overwrite buffer
		 * or misinterpret data. 0 is more likely to cause
		 * an endless loop.
		 */
		return OK;
	}
}
