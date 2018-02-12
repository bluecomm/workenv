/*******************************************************************************
 ** 
 ** Copyright (c) 2006-20010 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: mac_bits.c
 ** Description: Source file for bits operation.
 **
 ** Current Version: 1.0
 ** $Revision: 1.1.1.1 $
 ** Author: Jiangtao.Dong (dongjiangtao@ict.ac.cn)
 ** Date: 2007.05.16
 **
 ******************************************************************************/

/* Dependencies ------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "spu_bits.h"
#include "spu_log.h"
#include "spu_common.h"

/* Constants ---------------------------------------------------------------- */

/* Types -------------------------------------------------------------------- */

/* Macros ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------ */

UINT8 g_bits_mask[9] = { 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
UINT8 g_bits_mask_b[9] = { 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00 };

/* Functions ---------------------------------------------------------------- */
/*******************************************************************************
 * 
 * Input: 	x: a signed integer
 *		y: a unsigned integer
 * Output: return  x of the y-th power
 ******************************************************************************/
/*
 static UINT64 bits_pow(INT32 x, UINT32 y)
 {
 UINT64 ret = 1;
 
 while (y > 0) {
 ret *= x;
 y--;
 }
 log_msg(DEBUG_LOG,CSL,("ret=%1llx\n",ret);
 return ret;
 }
 */

/*******************************************************************************
 * Convert length of bits to length of bytes.
 *
 * Input: bit_len : The lenght of bits.
 *
 * Output: return the length of bytes.
 ******************************************************************************/
UINT64 convert_bitlen_to_bytelen(UINT64 bit_len) {
	return (bit_len % 8 == 0) ? (bit_len / 8) : (bit_len / 8 + 1);
}

/*******************************************************************************
 * Get the length of message.
 *
 * Input: bit_p : The pointer to struct bit operation.
 *
 * Output: return the length of message, or -1 when filed.
 ******************************************************************************/
INT32 get_bits_msg_len(BitOpt *bit_p, UINT32 opt_type) {
	if (bit_p == NULL) {
		log_msg(EROR_LOG, CSL, "bit_p == NULL !\n");
		return ERROR;
	} else {
		if (opt_type == BIT_PACKED) {
			return *(bit_p->msg_len);
		} else if (opt_type == BIT_UNPACKED) {
			return (*(bit_p->msg_len) - (*(bit_p->bit_offset) / 8));
		} else {
			log_msg(EROR_LOG, CSL, "Not support the Bit operation type!\n");
			return ERROR;
		}
	}
}

/*******************************************************************************
 * Show every bits of memory.
 *
 * Input: msg_p: The start pointer of memory.
 * 		  msg_len: The length of memory.
 * 		  lable: The lable of the memory.
 *
 * Output: None.
 ******************************************************************************/
INT32 show_bits(UINT8 *msg_p, UINT32 msg_len, CHAR *name) {
	INT32 byte = -1;
	UINT8 bit = 0, mask = 0;

	if (msg_p == NULL) {
		log_msg(EROR_LOG, CSL, "The pointer is NULL in show_bits()\n");
		return ERROR;
	}

	if (name != NULL) {
		log_msg(DEBUG_LOG, CSL, "The bits of memory %s is:\n", name);
	} else {
		//log_msg(DEBUG_PRINT, 0,"The bits of memory is:\n");
		log_msg(DEBUG_LOG, CSL, "The bits of memory is:\n");
	}

	for (byte = 0; byte < msg_len; byte++) {
		for (bit = 0, mask = 0x80; bit < 8; bit++) {
			if (*(msg_p + byte) & (mask >> bit)) {
				//log_msg(DEBUG_PRINT, 0,"1");
				log_msg(DEBUG_LOG, CSL, "1");
			} else {
				//log_msg(DEBUG_PRINT, 0,"0");
				log_msg(DEBUG_LOG, CSL, "0");
			}
		}
		//log_msg(DEBUG_PRINT, 0," ");
		log_msg(DEBUG_LOG, CSL, " ");

		if ((byte + 1) % 8 == 0) {
			//log_msg(DEBUG_PRINT, 0,"\n");
			log_msg(DEBUG_LOG, CSL, "\n");
		}
	}
	//log_msg(DEBUG_PRINT, 0,"\n");
	log_msg(DEBUG_LOG, CSL, "\n");
	return OK;
}

/*******************************************************************************
 * Initial the struct of BitOpt.
 *
 * Input: bit_p : The pointer to struct bit operation.
 *        msg_p : Point to the message, that will be packed or unpacked.
 *        msg_len : The length of message.
 *        bit_offset : The bit offset from the start of msg_p;
 *
 * Output: return OK, or -1 when filed.
 ******************************************************************************/
INT32 init_bits(BitOpt *bit_p, UINT8 *msg_p, UINT32 *msg_len, UINT32 *bit_offset) {
	if ((msg_p == NULL) || (bit_p == NULL) || (msg_len == NULL) || (bit_offset == NULL)) {
		log_msg(EROR_LOG, CSL, "init_bits if ((msg_p == NULL )!! \n ");
		return ERROR;
	}

	bit_p->msg_p = msg_p;
	bit_p->bit_offset = bit_offset;
	bit_p->msg_len = msg_len;

	return OK;
}

/*******************************************************************************
 * Initial the struct of UnpackBitOpt.
 *
 * Input: unpack_bitopt_p : The pointer to struct bit operation.
 *        msg_p : Point to the message, that will be packed or unpacked.
 *        bit_offset : The bit offset from the start of msg_p;
 *        msg_offset : The bit offset from the point to the message,indicate the bit length of the message.;
 *
 * Output: return OK, or -1 when filed.
 ******************************************************************************/
INT32 init_unpack_bits(UnpackBitOpt *unpack_bitopt_p, UINT8 *msg_p, UINT32 *msg_offset,
		UINT32 *bit_offset) {
	if ((msg_p == NULL) || (unpack_bitopt_p == NULL) || (msg_offset == NULL)
			|| (bit_offset == NULL)) {
		log_msg(EROR_LOG, CSL, "init_unpack_bit ! \n ");
		return ERROR;
	}

	unpack_bitopt_p->msg_p = msg_p;
	unpack_bitopt_p->msg_offset = msg_offset;
	unpack_bitopt_p->bit_offset = bit_offset;

	return OK;
}
/*******************************************************************************
 * Return the point back
 *
 * Input: bit_p : The pointer to struct bit operation.
 *        msg_p : Point to the message, that will be packed or unpacked.
 *        msg_len : The length of message.
 *        bit_offset : The bit offset from the start of msg_p;
 *
 * Output: return OK, or -1 when filed.
 ******************************************************************************/
INT32 return_bits(BitOpt *bit_p, UINT8 *msg_p, UINT8 *msg_len, UINT32 *bit_offset) {
	//UINT8 i = 0;
	if ((msg_p == NULL) || (bit_p == NULL) || (msg_len == NULL) || (bit_offset == NULL)) {
		log_msg(EROR_LOG, CSL, "return_bits if ((msg_p == NULL )!! \n ");
		return ERROR;
	}

	msg_p = bit_p->msg_p;
	bit_offset = bit_p->bit_offset;
	msg_len = (UINT8 *) bit_p->msg_len;

	return OK;
}
/*******************************************************************************
 * Skip n bits in packed or unpacked operation. Pack operation, not insure the
 * validity of the bits that ware sikped.
 *
 * Input: bit_p : The pointer to struct bit operation.
 *        n : The number of bits will be skiped.
 *
 * Output: return OK, or -1 when filed.
 ******************************************************************************/
INT32 skip_bits(BitOpt *bit_p, UINT32 n, UINT8 opt_type) {
	if (bit_p == NULL) {
		log_msg(EROR_LOG, CSL, "skip_bits bit_p == NUL! \n ");
		return ERROR;
	}

	if (opt_type == BIT_PACKED) { /* Skip bits when packing bits. */
		*(bit_p->bit_offset) += n;
		*(bit_p->msg_len) = convert_bitlen_to_bytelen(*(bit_p->bit_offset));
	} else if (opt_type == BIT_UNPACKED) { /* Skip bits when unpacking bits */
		*(bit_p->bit_offset) += n;
		if (*(bit_p->msg_len) < convert_bitlen_to_bytelen(*(bit_p->bit_offset))) {
			log_msg(EROR_LOG, CSL, "skip_bits() err! -- %d -- is too large\n", n);
			*(bit_p->bit_offset) -= n;
			log_msg(EROR_LOG, CSL, "skip_bits!  \n");
			return ERROR;
		}
	} else {
		log_msg(EROR_LOG, CSL, "skip_bits() err! Unknon bits operation type! \n");
		return ERROR;
	}

	return OK;
}

/*******************************************************************************
 * Unpack n bits from message, not support 32 bits or more.
 *
 * Input: unpack_bitopt_p : The pointer to struct bit operation.
 *        n : The number of bits will be unpacked.
 *
 * Output: return the unpacked value, or -1 when error occur.
 ******************************************************************************/
INT32 internal_unpack_bits(UnpackBitOpt *unpack_bitopt_p, UINT32 n, CHAR *pFile,
		INT32 line) {
	UINT8 bit_used = 0, bit_left = 0;
	UINT8 *current_byte_p = NULL;
	UINT32 ret = 0;

	if (unpack_bitopt_p == NULL) {
		log_msg(EROR_LOG, CSL, "Pointer is NULL in unpack_bits() at %d in %s!!!\n",
				line, pFile);
		return ERROR;
	}

	/*	if (n > 31) { */
	if (n > 32) {
		log_msg(EROR_LOG, CSL,
				"system not support bit unpacked more than 31 bits\
				at %d in %s!!!\n",
				line, pFile);
		return ERROR;
	} //?

	if (*(unpack_bitopt_p->bit_offset) + n > *(unpack_bitopt_p->msg_offset)) {
		log_msg(EROR_LOG, CSL, "Unpack bits out boundary at %d in %s!!!\n", line, pFile);
		return ERROR;
	}

	current_byte_p = unpack_bitopt_p->msg_p + (*(unpack_bitopt_p->bit_offset)) / 8;
	bit_used = *(unpack_bitopt_p->bit_offset) % 8;
	bit_left = 8 - bit_used;

	*(unpack_bitopt_p->bit_offset) += n;

	if (n <= bit_left) {
		/*
		 * All bits within current byte.
		 */
		ret = (*current_byte_p >> (bit_left - n)) & g_bits_mask[n];
		//		ret = (*current_byte_p >> (bit_left - n));
	} else {
		/*
		 * Bits wanted in this and at least some of next byte.
		 * Get left bits left in current byte 1st- all high bits wanted.
		 */
		ret = *current_byte_p++ & g_bits_mask[bit_left];
		n -= bit_left;

		/* Get whole bytes. */
		while (n >= 8) {
			ret = (ret << 8) + *current_byte_p++;
			n -= 8;
		}

		/* Get remaining n bits (n < 8), masking out unwanted high bits. */
		if (n > 0) {
			ret = (ret << n) + ((*current_byte_p >> (8 - n)) & g_bits_mask[n]);
		}

	}

	return ret;
}

/*******************************************************************************
 * Pack the value into message with n bits, not support more than 32 bits.
 *
 * Input: bit_p : The pointer to struct bit operation.
 *        n : The number of bits will be packed.
 *        value : The value need pacded into message.
 *
 * Output: return the length of message after packed, or -1 when filed.
 ******************************************************************************/
INT32 internal_pack_bits(BitOpt *bit_p, UINT32 n, UINT32 value, CHAR *pFile, INT32 line) {
	UINT8 bit_used = 0, bit_left = 0;
	UINT8 *current_byte_p = NULL;
	UINT32 current_value = 0;

	if ((bit_p == NULL) || (n > 32)) {
		log_msg(EROR_LOG, CSL, "internal_pack_bits!!\n");
		return ERROR;
	}

	bit_used = (*(bit_p->bit_offset)) % 8;
	bit_left = 8 - bit_used;

	current_byte_p = bit_p->msg_p + (*(bit_p->bit_offset) / 8);

	*(bit_p->bit_offset) += n;
	*(bit_p->msg_len) = convert_bitlen_to_bytelen(*(bit_p->bit_offset));

	if (n <= bit_left) {
		/*
		 * All bits can packed into current byte.
		 */
		/**current_byte_p += (value >> bit_used) & g_bits_mask[n];*/

		//*current_byte_p = (*current_byte_p & g_bits_mask_b[bit_left]); 
		*current_byte_p = (*current_byte_p)
				| ((value & g_bits_mask[n]) << (bit_left - n));
	} else {
		/*
		 * value can't packed into left bits in current byte,   
		 * pack the the bit_left high bits in to bit_left low bits of current
		 * byte, and increment byte pointer afterwards.
		 */
		//modify:	*current_byte_p++ += (value >> (n - bit_left)) & g_bits_mask[bit_left];
		current_value = 0x00000000;
		current_value = value;
		//*current_byte_p = (*current_byte_p & g_bits_mask_b[bit_left]); 
		*current_byte_p = (*current_byte_p)
				| ((current_value >> (n - bit_left)) & g_bits_mask[bit_left]);

		current_byte_p++;
		n -= bit_left;

		while (n >= 8) {
			//modify:	*current_byte_p++ = (value >> (n - 8)) & g_bits_mask[8];
			current_value = 0x00000000;
			current_value = value;
			//*current_byte_p = (*current_byte_p & g_bits_mask_b[8]); 
			*current_byte_p = (*current_byte_p)
					| ((current_value >> (n - 8)) & g_bits_mask[8]);
			current_byte_p++;
			n -= 8;
		}

		/* Pack remaining n bits (n < 8) into current byte. */
		if (n > 0) {
			current_value = 0x00000000;
			current_value = value;
			// *current_byte_p = (*current_byte_p & g_bits_mask_b[8]); 
			*current_byte_p = (*current_byte_p)
					| ((current_value & g_bits_mask[n]) << (8 - n));
		}
	}

	return OK;
}
