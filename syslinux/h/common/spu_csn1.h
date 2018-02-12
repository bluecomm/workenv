#ifndef _CSN1_H_
#define _CSN1_H_

/*! \file csn1.h
 *  \brief Declarations and types for CSN1 dissection 
 */
#include "spu_bitvec.h"
#define MIN_CSN(a,b) (((a)<(b))?(a):(b))

/*!< Error codes */
#define  CSN_OK                               0
#define  CSN_ERROR_GENERAL                   -1
#define  CSN_ERROR_DATA_NOT_VALID            -2
#define  CSN_ERROR_IN_SCRIPT                 -3
#define  CSN_ERROR_INVALID_UNION_INDEX       -4
#define  CSN_ERROR_NEED_MORE_BITS_TO_UNPACK  -5
#define  CSN_ERROR_ILLEGAL_BIT_VALUE         -6
#define  CSN_ERROR_INTERNAL                  -7
#define  CSN_ERROR_STREAM_NOT_SUPPORTED      -8
#define  CSN_ERROR_MESSAGE_TOO_LONG          -9
#define  CSN_ERROR_                         -10

/*!< CallBack return status */

/*!< CallBack return status */
typedef INT16 CSN_CallBackStatus_t;

#define  CSNCBS_OK                      0
#define  CSNCBS_NOT_OK                -10
#define  CSNCBS_NOT_TO_US             -11
#define  CSNCBS_NOT_COMPLETE          -12

#define CSNCBS_REVISION_LIMIT_STOP   -20 /*!< Stop packing/unpacking - revision limit */
#define CSNCBS_NOT_SUPPORTED_IE      -21 /*!< Handling of the unpacked IE is not supported by MS-software */

#ifndef ElementsOf
#define ElementsOf(array) (sizeof(array) / sizeof(array[0]))
#endif

/*!< Context holding CSN1 parameters */
typedef struct csn_stream_s {
	INT32 remaining_bits_len; /*!< IN to an csn stream operation */
	INT32 bit_offset; /*!< IN/OUT to an csn stream operation */
	INT32 direction; /* 0 - decode; 1 - encode */
} csn_stream_t;

typedef INT16 (*StreamSerializeFcn_t)(csn_stream_t* ar, bitvec_t *vector,
		UINT32* readIndex, VOID* data);
typedef enum csn_type_e {
	CSN_END = 0,
	CSN_BIT,
	CSN_UINT,
	CSN_TYPE,
	CSN_CHOICE,
	CSN_UNION,
	CSN_UNION_LH,
	CSN_UINT_ARRAY,
	CSN_TYPE_ARRAY,
	CSN_BITMAP, /*!< Bitmap with constant: <bitmap: bit(64)> */
	CSN_VARIABLE_BITMAP, /*!< <N: bit (5)> <bitmap: bit(N + offset)> */
	CSN_VARIABLE_BITMAP_1, /*!< <bitmap: bit**> i.e. to the end of message (R99) */
	CSN_LEFT_ALIGNED_VAR_BMP, /*!< As variable bitmap but the result is left aligned (R99) */
	CSN_LEFT_ALIGNED_VAR_BMP_1,/*!< As above only size is to the end of message (R99) */
	CSN_PADDING_BITS, /*!< Padding bits fill to the end of the buffer */
	CSN_VARIABLE_ARRAY, /*!< Array with length specified in parameter: <N: bit(4)> <list: octet(N + offset)> */
	CSN_VARIABLE_TARRAY, /*!< Type Array with length specified in parameter: <N: bit(x)> <Type>*N */
	CSN_VARIABLE_TARRAY_OFFSET,/*!< As above but with offset. The offset is stored as third parameter of csn_descr_t (descr.value) */
	CSN_RECURSIVE_ARRAY, /*!< Recursive way to specify an array of uint:   <list> ::= {1 <number: bit(4) <list>|0}; */
	CSN_RECURSIVE_TARRAY, /*!< Recursive way to specify an array of type:   <list> ::= {1 <type>} ** 0 ; */
	CSN_RECURSIVE_TARRAY_1, /*!< same as above but first element always exist:<list> ::= <type> {1 <type>} ** 0 ; */
	CSN_RECURSIVE_TARRAY_2, /*!< same as above but with reversed separators :<lists> ::= <type> { 0 <type> } ** 1 ; */
	CSN_EXIST,
	CSN_EXIST_LH,
	CSN_NEXT_EXIST,
	CSN_NEXT_EXIST_LH,
	CSN_NULL,
	CSN_FIXED,
	CSN_CALLBACK,
	CSN_UINT_OFFSET, /*!< unpack will add offset, inverse pack will subtract offset */
	CSN_UINT_LH, /*!< Low High extraction of INT32 */
	CSN_SERIALIZE,
	CSN_TRAP_ERROR
} csn_type_et;

/* \brief csn_descr_t structure:
 *
 * \param[in]:type:
 *       This is the CSN type. All existing types are specified in the section above.
 *
 * \param[in]:i:
 *       Depending on the contents of the type parameter,  the parameter "i" may have following meaning:
 *       - specifies the number of bits for the CSN_UINT or CSN_UINT_OR_NULL types
 *       - the offset for an array size by which the size is incremented
 *          for the CSN_VAR_ARRAY type
 *       - the length of each element of an array for the CSN_REC_ARRAY type
 *       - the number of the elements in an array for the CSN_TYPE_ARRAY type
 *       - the offset to the variable keeping the number of elements of an array for in the CSN_VAR_TARRAY type
 *       - the number of different data types in a union for the CSN_UNION, CSN_UNION_LH, and  for the CSN_CHOICE types
 *       - the length in bits of the fixed number defined for  the CSN_FIXED type
 *       - the number of lines to skip in the csn_descr_t type specified for the  CSN_NEXT_EXIST, CSN_NEXT_EXIST_LH,
 *          CSN_NEXT_EXIST_OR_NULL, and CSN_NEXT_EXIST_OR_NULL_LH types
 *       - the number of bits in a bitmap for the CSN_BITMAP type
 *       - the value by which the number of bits in a bitmap has to be incremented or decremented for the
 *          CSN_VAR_BITMAP, CSN_LEFT_VAR_BMP, and CSN_LEFT_BMP_1 types
 *       - the offset to param1 for the CSN_CALLBACK type
 *       - ERRORCODE  used by the CSN_ERROR type
 *       - the bit-length of the LENGTH field in a CSN_SERIALISE type
 *
 * param[in]:descr
 *       This parameter has different meaning depending on the value of the type parameter:
 *       - the offset for  the CSN_UINT_OFFSET type
 *       - the number of the elements in an array of the CSN_UINT_ARRAY type
 *       - the offset to the parameter where the size of the array has to be stored for the CSN_REC_ARRAY type
 *       - the address of the internal structure, describing the member type (by means of the csn_descr_t type) in the
 *          CSN_TYPE_ARRAY, CSN_VAR_TARRAY, and CSN_TYPE types
 *       - the address of the variable of type csn_choice_element_t describing all elements in the CSN_CHOICE type union
 *       - the offset to the variable where the number of bits has to be or is stored for the CSN_VAR_BITMAP,
 *          CSN_LEFT_VAR_BMP, and CSN_LEFT_BMP_1 types
 *       - the function number (case number) for the CSN_CALLBACK and CSN_CALLBACK_NO_ARGS types
 *       - the free text used by the CSN_TRAP_ERROR
 *
 * param[in]: offset
 *         This is an offset to the _MEMBER parameter counting from the beginning of struct
 *         where the unpacked or packed value shall be stored or fetched. The meaning of the _MEMBER parameter
 *         varies depending on the type which is specified  and so is the meaning of the offset parameter.
 *         Some types (and corresponding macros) do not have the _MEMBER parameter and then the offset parameter
 *         is not used or is different from the offset to the _MEMBER.
 *         - the fixed value for the CSN_FIXED type
 *         - an offset to the variable UnionType for CSN_UNION and CSN_UNION_LH types
 *         - an offset to the variable Exist for CSN_NEXT_EXIST and CSN_NEXT_EXIST_LH types
 *         - an offset to param2 in the CSN_CALLBACK  type
 *
 * param[in]:may_be_null
 *         TRUE: if dissection may be attempted at an offset beyond the length of existing data bits
 *         FALSE: othewise
 *
 * param[in]:sz
 *    - is the name of the parameter within the descr where their unpacked or packed value shall be stored or fetched.
 *      This paramater is pointed out by the offset parameter in the same csn_descr_t variable as the sz.
 *    - the free text used by the CSN_TRAP_ERROR (the same as parameter "i")
 *
 * param[in]:serialize
 *    - stores the size of _MEMBER type in case of the M_TYPE_ARRAY and M_VAR_TARRAY,
 *    - the address of the function which is provided by the M_SERIALIZE type.
 */

typedef struct csn_descr_s {
	INT16 type;
	INT16 i;
	union {
		VOID* ptr;
		UINT32 value;
	} descr;
	unsigned offset;
	INT32 may_be_null;
	const CHAR* sz;
	union {
		StreamSerializeFcn_t fcn;
		UINT32 value;
		INT32* hf_ptr;
	} serialize;
} csn_descr_t;

typedef struct csn_choice_element_s {
	UINT8 bits;
	UINT8 value;
	UINT8 keep_bits;
	csn_descr_t descr;
} csn_choice_element_t;

VOID csnStreamInit(csn_stream_t* ar, INT32 BitOffset, INT32 BitCount);

/*\brief  UnPacks data from bit stream. According to CSN description.
 * \param[in]: ar        stream will hold the parameters to the pack function
 *            ar->remaining_bits_len    [IN] Number of bits to unpack [OUT] number of bits left to unpack.
 *            ar->bit_offset            [IN/OUT] is the current bit where to proceed with the next bit to unpack.

 *   \param[in]:pDescr    CSN description.
 *   \param[in]: vector buffer containing the bit stream to unpack.
 *   \param[in]:data      unpacked data.
 *   \param[in]: readIndex the index of bit stream
 * 	\return  INT32  Number of bits left to be unpacked. Negative Error code if failed to unpack all bits
 **/

INT32 csnStreamDecoder(csn_stream_t* ar, const csn_descr_t* pDescr, bitvec_t *vector,
		unsigned* readIndex, VOID* data);

INT32 csnStreamEncoder(csn_stream_t* ar, const csn_descr_t* pDescr, bitvec_t *vector,
		unsigned* readIndex, VOID* data);

/*!< CSN struct macro's */
#define  CSN_DESCR_BEGIN(_STRUCT)\
        csn_descr_t CSNDESCR_##_STRUCT[] = {

#define  CSN_DESCR_END(_STRUCT)\
        {CSN_END, 0, {0}, 0, FALSE, "", {(StreamSerializeFcn_t)0}} };

/*!<*
 * CSN_ERROR(Par1, Par2, Par3)
 * May be called at any time when an abort of packing or unpacking of a message
 * is desired
 *      Par1: C structure name
 *      Par2: free text which will appear in the error handler
 *      Par3: Error code
 */
#define CSN_ERROR(_STRUCT, _Text, _ERRCODE)\
        {CSN_TRAP_ERROR, _ERRCODE, {(VOID*)_Text}, 0, FALSE, _Text, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_BIT(Par1, Par2)
 * Defines one bit element in the CSN1 syntax.
 *      Par1: C structure name
 *      Par2: C structure element name
 */
#define M_BIT(_STRUCT, _MEMBER)\
        {CSN_BIT, 0, {0}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_BIT_OR_NULL(Par1, Par2)
 * Similar to the M_BIT except that not only bit 0 or 1 but also the
 * end of the message may be encountered when looking for the next element in
 * the message.
 * Covers the case {null | 0 | 1}
 */
#define M_BIT_OR_NULL(_STRUCT, _MEMBER)\
         {CSN_BIT, 0, {0}, offsetof(_STRUCT, _MEMBER), TRUE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_NEXT_EXIST(Par1, Par2, Par3)
 * Indicates whether the next element or a group of elements defined in the 
 * structure is present or not.
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: number of lines to skip in the csn_descr_t type specified if the
 *            element(s) does not exist
 */
#define M_NEXT_EXIST(_STRUCT, _MEMBER, _NoOfExisting)\
        {CSN_NEXT_EXIST, _NoOfExisting, {0}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_NEXT_EXIST_LH(Par1, Par2, Par3)
 * similar to the M_NEXT_EXIST except that instead of bit 0/1 which is fetched
 * from the message in order to find out whether the next element/elements are
 * present in the message, the logical operation XOR with the background 
 * pattern 0x2B is performed on the read bit before the decision is made.
 */
#define M_NEXT_EXIST_LH(_STRUCT, _MEMBER, _NoOfExisting)\
        {CSN_NEXT_EXIST_LH, _NoOfExisting, {0}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_NEXT_EXIST_OR_NULL(Par1, Par2, Par3)
 * Similar to the M_NEXT_EXIST except that not only bit 0 or 1 but also the end
 * of the message may be encountered when looking for the next element in the 
 * message.
 * Covers the case {null | 0 | 1 < IE >} 
 */
#define M_NEXT_EXIST_OR_NULL(_STRUCT, _MEMBER, _NoOfExisting)\
        {CSN_NEXT_EXIST, _NoOfExisting, {(VOID*)1}, offsetof(_STRUCT, _MEMBER), TRUE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_NEXT_EXIST_OR_NULL_LH(Par1, Par2, Par3)
 * Similar to the M_NEXT_EXIST_LH except that not only bit 0 or 1 but also the
 * end of the message may be encountered when looking for the next element in
 * the message.
 * Covers the case {null | L | H < IE >} 
 */
#define M_NEXT_EXIST_OR_NULL_LH(_STRUCT, _MEMBER, _NoOfExisting)\
        {CSN_NEXT_EXIST_LH, _NoOfExisting, {(VOID*)1}, offsetof(_STRUCT, _MEMBER), TRUE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_UINT(Par1, Par2, Par3)
 * Defines an integer number.
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: number of bits used to code the element (between 1 and 32)
 */
#define M_UINT(_STRUCT, _MEMBER, _BITS)\
        {CSN_UINT, _BITS, {(VOID*)1}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_UINT_OR_NULL(Par1, Par2, Par3)
 * Similar to the M_UINT except that not only the request set of bits but also the
 * end of the message may be encountered when looking for the next element in
 * the message.
 * Covers the case {null | 0 | 1 < IE >}
 */
#define M_UINT_OR_NULL(_STRUCT, _MEMBER, _BITS)\
         {CSN_UINT, _BITS, {0}, offsetof(_STRUCT, _MEMBER), TRUE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_UINT(Par1, Par2, Par3)
 * This macro has the same functionality as M_UINT except that  in addition the
 * logical "exclusive or" operation with the background value "0x2B" is 
 * performed before the final value of the integer number is delivered from the 
 * received CSN.1 message
 */
#define M_UINT_LH(_STRUCT, _MEMBER, _BITS)\
        {CSN_UINT_LH, _BITS, {(VOID*)1}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_UINT_OFFSET(Par1, Par2, Par3, Par4)
 * Defines an integer number.
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: number of bits used to code the element (between 1 and 32)
 *      Par4: value added to the returned integer (offset)
 */
#define M_UINT_OFFSET(_STRUCT, _MEMBER, _BITS, _OFFSET)\
        {CSN_UINT_OFFSET, _BITS, {(VOID*)_OFFSET}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_UINT_ARRAY(Par1, Par2, Par3, Par4)
 * Defines an array of integer numbers. The size of the array is fixed.
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: number of bits used to code the each integer element (between 1 and 32)
 *      Par4: number of elements in the array (fixed integer value)
 */
#define M_UINT_ARRAY(_STRUCT, _MEMBER, _BITS, _ElementCount)\
        {CSN_UINT_ARRAY, _BITS, {(VOID*)_ElementCount}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_VAR_UINT_ARRAY(Par1, Par2, Par3, Par4)
 * Defines an array of integer numbers. The size of the array is variable.
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: number of bits used to code the each integer element (between 1 and 32)
 *      Par4: number of elements in the array supplied by reference to the
 *            structure member holding the length value
 */
#define M_VAR_UINT_ARRAY(_STRUCT, _MEMBER, _BITS, _ElementCountField)\
        {CSN_UINT_ARRAY, _BITS, {(VOID*)offsetof(_STRUCT, _ElementCountField)}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)1}}

/*!<*
 * M_VAR_ARRAY(Par1, Par2, Par3, Par4)
 * Defines an array of 8 bit large integer numbers. The size of the array is variable.
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: name of the structure member holding the size of the array
 *      Par4: offset that is added to the Par3 to get the actual size of the array
 */
#define M_VAR_ARRAY(_STRUCT, _MEMBER, _ElementCountField, _OFFSET)\
        {CSN_VARIABLE_ARRAY, _OFFSET, {(VOID*)offsetof(_STRUCT, _ElementCountField)}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_VAR_TARRAY(Par1, Par2, Par3, Par4)
 * Similar to M_TYPE_ARRAY except that the size of the array is variable.
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: the type of each element of the array
 *      Par4: name of the structure member holding the size of the array
 */
#define M_VAR_TARRAY(_STRUCT, _MEMBER, _MEMBER_TYPE, _ElementCountField)\
        {CSN_VARIABLE_TARRAY, offsetof(_STRUCT, _ElementCountField), {(VOID*)CSNDESCR_##_MEMBER_TYPE}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)sizeof(_MEMBER_TYPE)}}

/*!<*
 * M_VAR_TARRAY_OFFSET(Par1, Par2, Par3, Par4)
 * Same as M_VAR_TARRAY with offset
 */
#define M_VAR_TARRAY_OFFSET(_STRUCT, _MEMBER, _MEMBER_TYPE, _ElementCountField)\
        {CSN_VARIABLE_TARRAY_OFFSET, offsetof(_STRUCT, _ElementCountField), {(VOID*)CSNDESCR_##_MEMBER_TYPE}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)sizeof(_MEMBER_TYPE)}}

/*!<*
 * M_REC_ARRAY(Par1, Par2, Par3, Par4)
 * similar to the M_VAR_ARRAY. The difference is that the size of the array is 
 * not known in advance and it has to be calculated during unpacking. Its value
 * is stored in a variable which belongs to the same structure as the array. 
 * A zero element terminates the array. The CSN.1 syntax describes it 
 * recursively as:
 * <array> ::={1 <element> <array>| 0} 
 *
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: name of the structure member where the calculated the size of the 
 *            array will be stored
 *      Par4: length of each element in bits
 */
#define M_REC_ARRAY(_STRUCT, _MEMBER, _ElementCountField, _BITS)\
        {CSN_RECURSIVE_ARRAY, _BITS, {(VOID*)offsetof(_STRUCT, _ElementCountField)}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_VAR_TYPE_ARRAY(Par1, Par2, Par3, Par4)
 * Defines an array of structures. The size of the array is variable.
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: name of the structure
 *      Par4: number of elements in the array (fixed integer value)
 */
#define M_TYPE_ARRAY(_STRUCT, _MEMBER, _MEMBER_TYPE, _ElementCount)\
        {CSN_TYPE_ARRAY, _ElementCount, {(VOID*)CSNDESCR_##_MEMBER_TYPE}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)sizeof(_MEMBER_TYPE)}}

/*!<*
 * M_REC_TARRAY(Par1, Par2, Par3, Par4)
 * Defines an recursive array of structures. The size of the array is variable.
 * <list> ::= {1 <type>} ** 0 ;
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: name of the structure
 *      Par4: will hold the number of element in the array after unpacking
 */
#define M_REC_TARRAY(_STRUCT, _MEMBER, _MEMBER_TYPE, _ElementCountField)\
        {CSN_RECURSIVE_TARRAY, offsetof(_STRUCT, _ElementCountField), {(VOID*)CSNDESCR_##_MEMBER_TYPE}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)sizeof(_MEMBER_TYPE)}}

/*!<*
 * M_REC_TARRAY1(Par1, Par2, Par3, Par4)
 * Same as M_REC_TARRAY but first element always exist:
 * <list> ::= <type> {1 <type>} ** 0 ;
 */
#define M_REC_TARRAY_1(_STRUCT, _MEMBER, _MEMBER_TYPE, _ElementCountField)\
        {CSN_RECURSIVE_TARRAY_1, offsetof(_STRUCT, _ElementCountField), {(VOID*)CSNDESCR_##_MEMBER_TYPE}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)sizeof(_MEMBER_TYPE)}}

/*!<*
 * M_REC_TARRAY2(Par1, Par2, Par3, Par4)
 * Same as M_REC_TARRAY but with reversed separators :
 * <lists> ::= <type> { 0 <type> } ** 1 ;
 */
#define M_REC_TARRAY_2(_STRUCT, _MEMBER, _MEMBER_TYPE, _ElementCountField)\
        {CSN_RECURSIVE_TARRAY_2, offsetof(_STRUCT, _ElementCountField), {(VOID*)CSNDESCR_##_MEMBER_TYPE}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)sizeof(_MEMBER_TYPE)}}

/*!<*
 * M_TYPE(Par1, Par2, Par3)
 * Defines a reference to a structure which is described elsewhere
 * <list> ::= {1 <type>} ** 0 ;
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: type of member
 */
#define M_TYPE(_STRUCT, _MEMBER, _MEMBER_TYPE)\
        {CSN_TYPE, 0, {(VOID*)CSNDESCR_##_MEMBER_TYPE}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_UNION(Par1, Par2)
 * Informs the CSN.1 library that a union follows and how many possible choices
 * there are in the union. The actual value of the choice, which points out the
 * chosen element of the union is stored in the uint8 variable and is usually 
 * called UnionType. The elements of the union have to be listed directly after 
 * the M_UNION statement.
 *      Par1: C structure name
 *      Par2: number of possible choice in the union
 */
#define M_UNION(_STRUCT, _COUNT)\
        {CSN_UNION, _COUNT, {0}, offsetof(_STRUCT, UnionType), FALSE, "UnionType", {(StreamSerializeFcn_t)0}}

/*!<*
 * M_UNION_LH(Par1, Par2)
 * Same as M_UNION but masked with background value 0x2B
 */
#define M_UNION_LH(_STRUCT, _COUNT)\
        {CSN_UNION_LH, _COUNT, {0}, offsetof(_STRUCT, UnionType), FALSE, "UnionType", {(StreamSerializeFcn_t)0}}

/*!<*
 * M_CHOICE(Par1, Par2, Par3, Par4)
 * Similar to the M_UNION. In the M_UNION the selected element of all possible 
 * choices in the union is referred as a sequential numbers,  i.e., the first 
 * choice is addressed as choice 0 the second as choice 1, the third as choice 
 * 2 and so on, both in the encoded message and in the variable UnionType which
 * is the part of the message.  In the CSN_CHOICE case, this rule does not 
 * apply. There is free but predefined mapping of the element of the union and 
 * the value which addresses this element.
 * The value of the address is called a selector.
 * After unpacking, this value is then converted to the sequential number of the
 * element in the union and stored in the UnionType variable.
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: address of an array of type csn_choice_element_t where all possible
 *            values of the selector are provided, together with the selector 
 *            length expressed in bits and the address of  the csn_descr_t type 
 *            where the element is defined. For every element in the union 
 *            there is one line in the Choice variable. These lines have to 
 *            appear in the _CHOICE in the same order as the elements in the 
 *            union. The element of the union selected in the message through 
 *            the _CHOICE parameter is after unpacking translated to the 
 *            corresponding sequential number of this element and stored in 
 *            the variable pointed out by the _MEMBER
 *      Par4: number of possible choices in the union
 */
#define M_CHOICE(_STRUCT, _MEMBER, _CHOICE, _ElementCount)\
        {CSN_CHOICE, _ElementCount, {(VOID*)_CHOICE}, offsetof(_STRUCT, _MEMBER), FALSE, #_CHOICE, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_FIXED(Par1, Par2, Par3)
 * Defines a fixed value of type integer which should be fetched from or stored 
 * in  the message
 *      Par1: C structure name
 *      Par2: gives the length of the fixed number in bits.
 *      Par3: the value of the number. If the expected value is not present in 
 *             the message the unpacking procedure is aborted
 */
#define M_FIXED(_STRUCT, _BITS, _BITVALUE)\
        {CSN_FIXED, _BITS, {0}, _BITVALUE, FALSE, #_BITVALUE, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_SERIALIZE(Par1, Par2, Par3)
 * Allows using a complete free format of data being encoded or decoded. 
 * When the M_SERIALIZE is encounted during encoding or decoding of a message
 * the CSNstream program passes the control over to the specified function 
 * together with all necessary parameters about the current position within 
 * the message being unpacked or packed.  When transferring of "serialized" 
 * data to or from the message is finished by the function the CSNstream gets 
 * back control over the data stream and continues to work with the message.
 */
#define M_SERIALIZE(_STRUCT, _MEMBER, _LENGTH_LEN, _SERIALIZEFCN)\
        {CSN_SERIALIZE, _LENGTH_LEN, {0}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {_SERIALIZEFCN}}

#define M_CALLBACK(_STRUCT, _CSNCALLBACKFCN, _PARAM1, _PARAM2)\
        {CSN_CALLBACK, offsetof(_STRUCT, _PARAM1), {_CSNCALLBACKFCN}, offsetof(_STRUCT, _PARAM2),  FALSE, "CallBack_"#_CSNCALLBACKFCN, {(StreamSerializeFcn_t)0}}

/*!<*
 * M_BITMAP(Par1, Par2, Par3)
 * Defines a type which consists of a bitmap. The size of the bitmap in bits 
 * is fixed and provided by the parameter Par3
 *      Par1: C structure name
 *      Par2: C structure element name
 *      Par3: length of the bitmap expressed in bits
 */
#define M_BITMAP(_STRUCT, _MEMBER, _BITS)\
        {CSN_BITMAP, _BITS, {0}, offsetof(_STRUCT, _MEMBER),  FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!< variable length, right aligned bitmap i.e. _ElementCountField = 11 => 00000111 11111111 */
#define M_VAR_BITMAP(_STRUCT, _MEMBER, _ElementCountField, _OFFSET)\
        {CSN_VARIABLE_BITMAP, _OFFSET, {(VOID*)offsetof(_STRUCT, _ElementCountField)}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!< variable length, right aligned bitmap filling the rest of message
 * - when unpacking the _ElementCountField will be set in runtime
 * - when packing _ElementCountField contains the size of bitmap
 */
#define M_VAR_BITMAP_1(_STRUCT, _MEMBER, _ElementCountField, _OFFSET)\
        {CSN_VARIABLE_BITMAP_1, _OFFSET, {(VOID*)offsetof(_STRUCT, _ElementCountField)}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!< variable length, left aligned bitmap i.e. _ElementCountField = 11 => 11111111 11100000 */
#define M_LEFT_VAR_BMP(_STRUCT, _MEMBER, _ElementCountField, _OFFSET)\
        {CSN_LEFT_ALIGNED_VAR_BMP, _OFFSET, {(VOID*)offsetof(_STRUCT, _ElementCountField)}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!< variable length, left aligned bitmap filling the rest of message
 *- when unpacking the _ElementCountField will be set in runtime
 * - when packing _ElementCountField contains the size of bitmap
 */
#define M_LEFT_VAR_BMP_1(_STRUCT, _MEMBER, _ElementCountField, _OFFSET)\
        {CSN_LEFT_ALIGNED_VAR_BMP_1, _OFFSET, {(VOID*)offsetof(_STRUCT, _ElementCountField)}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

/*!< todo: dissect padding bits looking for unexpected extensions */
#define M_PADDING_BITS(_STRUCT)\
        {CSN_PADDING_BITS, 0, {0}, 0, TRUE, "Padding", {(StreamSerializeFcn_t)0}}

#define M_NULL(_STRUCT, _MEMBER)\
        {CSN_NULL, 0, {0}, offsetof(_STRUCT, _MEMBER), FALSE, #_MEMBER, {(StreamSerializeFcn_t)0}}

#define M_THIS_EXIST(_STRUCT)\
        {CSN_EXIST, 0, {0}, offsetof(_STRUCT, Exist), FALSE, "Exist", {(StreamSerializeFcn_t)0}}

#define M_THIS_EXIST_LH(_STRUCT)\
        {CSN_EXIST_LH, 0, {0}, offsetof(_STRUCT, Exist), FALSE, "Exist", {(StreamSerializeFcn_t)0}}

/*!< return value 0 if ok else discontionue the unpacking */
typedef INT16 (*CsnCallBackFcn_t)(VOID* pv, ...);

#define CSNDESCR(_FuncType) CSNDESCR_##_FuncType

/*! @} */

#endif /*!<_PACKET_CSN1_H_*/
