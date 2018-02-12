/* bit vector utility routines */

/*! \addtogroup bitvec
 *  @{
 */

/*! \file bitvec.c
 *  \brief Sylincom bit vector abstraction
 */

#include "spu_bitvec.h"

#include <errno.h>
#include <stdlib.h>

#ifdef USE_LOG
#include "spu_log.h"

#endif
static UINT32 bytenum_from_bitnum(UINT32 bitnum) {
	UINT32 bytenum = bitnum / 8;
	return bytenum;
}

/* convert ZERO/ONE/L/H to a bitmask at given pos in a byte */
static UINT8 bitval2mask(bit_value_et bit, UINT8 bitnum) {
	INT32 bitval = 0;

	switch (bit) {
	case ZERO:
		bitval = (0 << bitnum);
		break;
	case ONE:
		bitval = (1 << bitnum);
		break;
	case L:
		bitval = ((0x2b ^ (0 << bitnum)) & (1 << bitnum));
		break;
	case H:
		bitval = ((0x2b ^ (1 << bitnum)) & (1 << bitnum));
		break;
	default:
		return OK;
	}
	return bitval;
}

/*! \brief check if the bit is 0 or 1 for a given position inside a bitvec
 *  \param[in] bv the bit vector on which to check
 *  \param[in] bitnr the bit number inside the bit vector to check
 *  \returns 
 */

/* ---------Modified by wyy-------
 * "bit_value_et" only can be 0,1,2,3.
 bit_value_et bitvec_get_bit_pos(const bitvec_t *bv, UINT32 bitnr)
 */
INT32 bitvec_get_bit_pos(const bitvec_t *bv, UINT32 bitnr) {
	UINT32 bytenum = bytenum_from_bitnum(bitnr);
	UINT32 bitnum = 7 - (bitnr % 8);
	UINT8 bitval = 0;

	if (bytenum >= bv->data_len) {
		log_msg(EROR_LOG, CSL, "bytenum >= bv->data_len %d\n", bytenum);
		return ERROR;
	}

	bitval = bitval2mask(ONE, bitnum);

	if (bv->data[bytenum] & bitval)
		return ONE;

	return ZERO;
}

/*! \brief check if the bit is L or H for a given position inside a bitvec
 *  \param[in] bv the bit vector on which to check
 *  \param[in] bitnr the bit number inside the bit vector to check
 */

/*  Modified by wyy----------change the return value type. 
 bit_value_et bitvec_get_bit_pos_high(const bitvec_t *bv,
 UINT32 bitnr)
 */
INT32 bitvec_get_bit_pos_high(const bitvec_t *bv, UINT32 bitnr) {
	UINT32 bytenum = bytenum_from_bitnum(bitnr);
	UINT32 bitnum = 7 - (bitnr % 8);
	UINT8 bitval = 0;

	if (bytenum >= bv->data_len) {
		log_msg(EROR_LOG, CSL, "bytenum >= bv->data_len \n");
		return ERROR;
	}

	bitval = bitval2mask(H, bitnum);

	if ((bv->data[bytenum] & (1 << bitnum)) == bitval)
		return H;

	return L;
}

INT32 bitvec_unpack(bitvec_t *bv, UINT8 *buffer) {
	INT32 i = 0;
	for (i = 0; i < bv->data_len; i++) {
		bv->data[i] = buffer[i];
	}
	return i;
}

/*! \brief set a bit at given position in a bit vector
 *  \param[in] bv bit vector on which to operate
 *  \param[in] bitnum number of bit to be set
 *  \param[in] bit value to which the bit is to be set
 */
INT32 bitvec_set_bit_pos(bitvec_t *bv, UINT32 bitnr, bit_value_et bit) {
	UINT32 bytenum = bytenum_from_bitnum(bitnr);
	UINT32 bitnum = 7 - (bitnr % 8);
	UINT8 bitval = 0;

	if (bytenum >= bv->data_len) {
		log_msg(EROR_LOG, CSL, "bytenum(%d) >= bv->data_len(%d) \n",
				bytenum, bv->data_len);
		return ERROR;
	}

	/* first clear the bit */
	bitval = bitval2mask(ONE, bitnum);
	bv->data[bytenum] &= ~bitval;

	/* then set it to desired value */
	bitval = bitval2mask(bit, bitnum);
	bv->data[bytenum] |= bitval;

	return OK;
}

/*! \brief set the next bit inside a bitvec
 *  \param[in] bv bit vector to be used
 *  \param[in] bit value of the bit to be set
 */
INT32 bitvec_set_bit(bitvec_t *bv, bit_value_et bit) {
	INT32 rc = 0;
	rc = bitvec_set_bit_pos(bv, bv->cur_bit, bit);
	if (!rc)
		bv->cur_bit++;

	return rc;
}

bitvec_t *bitvec_alloc(UINT32 size) {
	bitvec_t *bv = NULL;

	if (NULL == (bv = spu_calloc(1, sizeof(bitvec_t)))) {
		log_msg(EROR_LOG, CSL, "calloc error!\n");
		return NULL;
	}

	bv->data_len = size;
	bv->cur_bit = 0;
	if (NULL == (bv->data = spu_calloc(1, size))) {
		log_msg(EROR_LOG, CSL, "calloc error!\n");
		return NULL;
	}

	return bv;
}

VOID bitvec_free(bitvec_t *bv) {
	spu_free(bv->data);
	spu_free(bv);
}

INT32 bitvec_unhex(bitvec_t *bv, const CHAR* src) {
	UINT32 val = 0;
	UINT32 write_index = 0;
	UINT32 digits = bv->data_len * 2;
	UINT32 i = 0;
	for (i = 0; i < digits; i++) {
		if (sscanf(src + i, "%1x", &val) < 1) {
			return 1;
		}
		bitvec_write_field(bv, &write_index, val, 4);
	}
	return OK;
}

UINT64 bitvec_read_field(bitvec_t *bv, UINT32* read_index, UINT32 len) {
	INT32 i = 0;
	UINT64 ui = 0;
	bv->cur_bit = *read_index;
	for (i = 0; i < len; i++) {
		INT32 bit = bitvec_get_bit_pos((const bitvec_t *) bv, bv->cur_bit);
		if (bit < 0) {
			log_msg(EROR_LOG, CSL,
					"bit = bitvec_get_bit_pos((const bitvec_t *) bv, bv->cur_bit) %d\n",
					bit);
			return bit;
		}

		if (bit)
			ui |= ((UINT64) 1 << (len - i - 1));
		bv->cur_bit++;
	}
	*read_index += len;
	return ui;
}

INT32 bitvec_write_field(bitvec_t *bv, UINT32* write_index, UINT64 val, UINT32 len) {
	INT32 i = 0, rc = 0;

	bv->cur_bit = *write_index;

	for (i = 0; i < len; i++) {
		INT32 bit = 0;
		if (val & ((UINT64) 1 << (len - i - 1)))
			bit = 1;

		if ((rc = bitvec_set_bit(bv, (bit_value_et) bit))) {
			log_msg(EROR_LOG, CSL, "bitvec_set_bit failed! !!!\n");
			exit(1);
			return rc;
		}

	}
	*write_index += len;

	return OK;
}
