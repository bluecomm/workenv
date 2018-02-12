#ifndef _BITVEC_H
#define _BITVEC_H

#include "spu_common.h"
#include <stdint.h>

/* bit vector utility routines */

/*! \defgroup bitvec Bit vectors
 *  @{
 */

/*! \file bitvec.h
 *  \brief Sylincom bit vector abstraction
 */

/*! \brief A single GSM bit
 *
 * In GSM mac blocks, every bit can be 0 or 1, or L or H.  L/H are
 * defined relative to the 0x2b padding pattern */
typedef enum bit_value_e {
	ZERO = 0, /*!< \brief A zero (0) bit */
	ONE = 1, /*!< \brief A one (1) bit */
	L = 2, /*!< \brief A CSN.1 "L" bit */
	H = 3, /*!< \brief A CSN.1 "H" bit */
} bit_value_et;

/*! \brief structure describing a bit vector */
typedef struct bitvec_s {
	UINT32 cur_bit; /*!< \brief curser to the next unused bit */
	UINT32 data_len; /*!< \brief length of data array in bytes */
	UINT8 *data; /*!< \brief pointer to data array */
} bitvec_t;

bitvec_t *bitvec_alloc(unsigned size);

/*bit_value_et bitvec_get_bit_pos(const bitvec_t *bv, UINT32 bitnr);*/
INT32 bitvec_get_bit_pos(const bitvec_t *bv, UINT32 bitnr);

/*bit_value_et bitvec_get_bit_pos_high(const bitvec_t *bv,
 UINT32 bitnr);*/
INT32 bitvec_get_bit_pos_high(const bitvec_t *bv, UINT32 bitnr);

INT32 bitvec_set_bit_pos(bitvec_t *bv, UINT32 bitnum, bit_value_et bit);

INT32 bitvec_set_bit(bitvec_t *bv, bit_value_et bit);

VOID bitvec_free(bitvec_t *bv);

INT32 bitvec_unhex(bitvec_t *bv, const CHAR* src);

UINT64 bitvec_read_field(bitvec_t *bv, unsigned* read_index, unsigned len);

INT32 bitvec_write_field(bitvec_t *bv, unsigned* write_index, UINT64 val, unsigned len);

INT32 bitvec_unpack(bitvec_t *bv, UINT8 *buffer);
/*! @} */

#endif /* _BITVEC_H */
