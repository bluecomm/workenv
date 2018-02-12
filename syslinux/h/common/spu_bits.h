/*******************************************************************************
 ** 
 ** Copyright (c) 2006-2010 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: mac_bits.h
 ** Description: Header file for bits operation.
 **
 ** Current Version: 1.0
 ** Author: Jiangtao.Dong (dongjiangtao@ict.ac.cn)
 ** Date: 2007.05.15
 **
 ******************************************************************************/

#ifndef _SPU_BITS_H_
#define _SPU_BITS_H_

#include "spu_type.h"
/* Dependencies ------------------------------------------------------------- */

/* Constants ---------------------------------------------------------------- */

/*
 * The Bits operation type.
 */
typedef enum {
	BIT_PACKED, /* Packed bits. */
	BIT_UNPACKED /* Unpacked bits. */
} BitOptType;

/* Types -------------------------------------------------------------------- */

/*
 * Type of Bits for pack message operation.
 */
typedef struct {
	UINT8 *msg_p; /* Point to the message whitch packed or unpacked */
	UINT32 *bit_offset; /* The bit offset from the start of msg_p. */
	UINT32 *msg_len; /* The total length of the message. */
} BitOpt;

/*
 * Type of Bits for unpack message operation.
 */
typedef struct {
	UINT8 *msg_p; /* Point to the message whitch packed or unpacked */
	UINT32 *bit_offset; /* The bit offset from the start of msg_p. */
	UINT32 *msg_offset; /* The bit offset from the point to the message,indicate the bit length of the message. */
} UnpackBitOpt;

/* Macros ------------------------------------------------------------------- */

#define pack_bits(bit_p, n, value) internal_pack_bits(bit_p, n, value, __FILE__, __LINE__)
#define unpack_bits(unpack_bitopt_p, n) internal_unpack_bits(unpack_bitopt_p, n, __FILE__, __LINE__)
/* Globals ------------------------------------------------------------------ */

/* Functions ---------------------------------------------------------------- */
/*******************************************************************************
 * Convert length of bits to length of bytes.
 *
 * Input: bit_len : The lenght of bits.
 *
 * Output: return the length of bytes.
 ******************************************************************************/
extern UINT64 convert_bitlen_to_bytelen(UINT64 bit_len);

/*******************************************************************************
 * Get the length of message.
 *
 * Input: bit_p : The pointer to struct bit operation.
 *
 * Output: return the length of message, or -1 when filed.
 ******************************************************************************/
extern INT32 get_bits_msg_len(BitOpt *bit_p, UINT32 opt_type);

/*******************************************************************************
 * Show every bits of memory.
 *
 * Input: msg_p: The start pointer of memory.
 * 		  msg_len: The length of memory.
 * 		  lable: The lable of the memory.
 *
 * Output: None.
 ******************************************************************************/
extern INT32 show_bits(UINT8 *msg_p, UINT32 msg_len, CHAR *name);

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
extern INT32 init_bits(BitOpt *bit_p, UINT8 *msg_p, UINT32 *msg_len, UINT32 *bit_offset);

/*******************************************************************************
 * Initial the struct of UnpackBitOpt.
 *
 * Input: unpck_bitopt_p : The pointer to struct bit operation.
 *        msg_p : Point to the message, that will be packed or unpacked.
 *        bit_offset : The bit offset from the start of msg_p;
 *        msg_offset : The bit offset from the point to the message,indicate the bit length of the message.;
 *
 * Output: return OK, or -1 when filed.
 ******************************************************************************/
extern INT32 init_unpack_bits(UnpackBitOpt *unpack_bitopt_p, UINT8 *msg_p,
		UINT32 *msg_offset, UINT32 *bit_offset);

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
extern INT32 return_bits(BitOpt *bit_p, UINT8 *msg_p, UINT8 *msg_len, UINT32 *bit_offset);

/*******************************************************************************
 * Skip n bits in packed or unpacked operation. Pack operation, not insure the 
 * validity of the bits that ware sikped.
 *
 * Input: bit_p : The pointer to struct bit operation.
 *        n : The number of bits will be skiped.
 *
 * Output: return OK, or -1 when filed.
 ******************************************************************************/
extern INT32 skip_bits(BitOpt *bit_p, UINT32 n, UINT8 opt_type);

/*******************************************************************************
 * Unpack n bits from message, not support 32 bits or more.
 *
 * Input: unpck_bitopt_p : The pointer to struct bit operation.
 *        n : The number of bits will be unpacked.
 *
 * Output: return the unpacked value, or -1 when error occur.
 ******************************************************************************/
INT32 internal_unpack_bits(UnpackBitOpt *unpack_bitopt_p, UINT32 n, CHAR *pFile,
		INT32 line);

/*******************************************************************************
 * Pack the value into message with n bits, not support more than 32 bits.
 *
 * Input: bit_p : The pointer to struct bit operation.
 *        n : The number of bits will be packed.
 *        value : The value need pacded into message.
 *
 * Output: return the length of message after packed, or -1 when filed.
 ******************************************************************************/
INT32 internal_pack_bits(BitOpt *bit_p, UINT32 n, UINT32 value, CHAR *pFile, INT32 line);

#endif	/* BITS_H */
