/****************************************************************************
 *  *
 *   * $Source: /usr/local/cvsroot/unixlib/source/math/c/ilogb,v $
 *    * $Date: 1997/10/09 20:00:05 $
 *     * $Revision: 1.3 $
 *      * $State: Exp $
 *       * $Author: unixlib $
 *        *
 *         ***************************************************************************/

/* @(#)s_ilogb.c 5.1 93/09/24 */
/*
 *  * ====================================================
 *   * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *    *
 *     * Developed at SunPro, a Sun Microsystems, Inc. business.
 *      * Permission to use, copy, modify, and distribute this
 *       * software is freely granted, provided that this notice
 *        * is preserved.
 *         * ====================================================
 *          */

/* ilogb(double x)
 *  * return the binary exponent of non-zero x
 *   * ilogb(0) = 0x80000001
 *    * ilogb(inf/NaN) = 0x7fffffff (no signal is raised)
 *     */

#include <math.h>
#include "spu_common.h"

/* The original fdlibm code used statements like:
 *         n0 = ((*(INT32*)&one)>>29)^1;             * index of high word *
 *                 ix0 = *(n0+(INT32*)&x);                   * high word of x *
 *                         ix1 = *((1-n0)+(INT32*)&x);               * low word of x *
 *                            to dig two 32 bit words out of the 64 bit IEEE floating point
 *                               value.  That is non-ANSI, and, moreover, the gcc instruction
 *                                  scheduler gets it wrong.  We instead use the following macros.
 *                                     Unlike the original code, we determine the endianness at compile
 *                                        time, not at run time; I don't see much benefit to selecting
 *                                           endianness at run time.  */

/* A union which permits us to convert between a double and two 32 bit
 *    ints.  */

typedef union {
	double value;
	struct {
		UINT32 msw;
		UINT32 lsw;
	} parts;
} ieee_double_shape_type;

/* Get the more significant 32 bit INT32 from a double.  */
#define GET_HIGH_WORD(i,d)                                      \
		do {                                                            \
			ieee_double_shape_type gh_u;                                  \
			gh_u.value = (d);                                             \
			(i) = gh_u.parts.msw;                                         \
		} while (0)

#define GET_LOW_WORD(i,d)                                       \
		do {                                                            \
			ieee_double_shape_type gl_u;                                  \
			gl_u.value = (d);                                             \
			(i) = gl_u.parts.lsw;                                         \
		} while (0)

INT32 ilogb(double x) {
	INT32 hx = 0, lx = 0, ix = 0;

	GET_HIGH_WORD(hx, x);
	hx &= 0x7fffffff;
	if (hx < 0x00100000) {
		GET_LOW_WORD(lx, x);
		if ((hx | lx) == 0)
			return 0x80000001; /* ilogb(0) = 0x80000001 */
		else
		/* subnormal x */if (hx == 0) {
			for (ix = -1043; lx > 0; lx <<= 1)
				ix -= 1;
		} else {
			for (ix = -1022, hx <<= 11; hx > 0; hx <<= 1)
				ix -= 1;
		}
		return ix;
	} else if (hx < 0x7ff00000)
		return (hx >> 20) - 1023;
	else
		return 0x7fffffff;
}
