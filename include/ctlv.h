/**
 * @file ctlv.h
 * @brief A basic TLV (type-length-value) structure implementation.
 * Format:
 *    type   (1 bytes)
 *    length (4 bytes)
 *    value  (>=1 bytes)
 * @author https://github.com/4g3nt47
 */

#ifndef CTLV_H
#define CTLV_H

#include <stdint.h>
#include <stdlib.h>

// Define some data types we'll need.
typedef uint8_t   tlv_type_t;    // The type data type.
typedef uint32_t   tlv_len_t;    // The length data type.
typedef u_char * tlv_ubytes_t;   // The bytes data type for storing raw TLV struct for parsing.


// The number of bytes required for TLV header (type + length)
#define TLV_HEADER_SIZE (sizeof(tlv_type_t) + sizeof(tlv_len_t))

// Enums used to denote the type of value the TLV holds. Hardcoded enum values just to be sure :)
typedef enum{
  TLV_UINT8 = 0, TLV_UINT16 = 1, TLV_UINT32 = 2, TLV_ULONG = 3, TLV_UBYTES = 4
} lv_type_e;

// Union to use in TLV struct for holding values.
typedef union{
    uint8_t v_uint8;
    uint16_t v_uint16;
    uint32_t v_uint32;
    ulong v_ulong;
    tlv_ubytes_t v_ubytes;
} tlv_value_u;

// The TLV structure.
typedef struct {
    tlv_type_t  type;
    tlv_len_t    len;
    tlv_value_u  value;
} tlv_t;

/**
 * @brief Create a TLV structure by parsing a given TLV bytes.
 * @warning If the parsed TLV is of type TVL_UBYTES, memory is dynamically allocated to tlv->value.v_ubytes, and it's up to the caller to free it when done, ideally using tlv_free()
 * @param tlv The TLV structure to load into
 * @param ubytes The TLV bytes to parse
 * @return 0 on success
 */
short tlv_load(tlv_t *tlv, tlv_ubytes_t ubytes);

/**
 * @brief Convert a TLV structure into a TLV bytes.
 * @param tlv The TLV structure to convert
 * @return Bytes array containing the TLV data. Format: type (1 byte) + length (4 bytes) + value
 */
tlv_ubytes_t tlv_dump(tlv_t *tlv);

/**
 * @brief Create a TLV structure for an 8-bit unsigned int and pack it into bytes.
 * @param v The value to pack
 * @return The TLV structure bytes. Caller can determine the length of the array using: TLV_HEADER_SIZE + sizeof(v)
 */
tlv_ubytes_t tlv_pack_uint8(uint8_t v);

/**
 * @brief Create a TLV structure for a 16-bit unsigned int and pack it into bytes.
 * @param v The value to pack
 * @return The TLV structure bytes
*/
tlv_ubytes_t tlv_pack_uint16(uint16_t v);

/**
 * @brief Create a TLV structure for a 32-bit unsigned int and pack it into bytes.
 * @param v The value to pack
 * @return The TLV structure bytes
 */
tlv_ubytes_t tlv_pack_uint32(uint32_t v);

/**
 * @brief Create a TLV structure for an unsigned long and pack it into bytes.
 * @param v The value to pack
 * @return The TLV structure bytes
 */
tlv_ubytes_t tlv_pack_ulong(ulong v);

/**
 * @brief Create a TLV structure for a given bytes array and pack it into bytes.
 * @param v The array to pack
 * @param len The length of the array
 * @return The TLV structure bytes
 */
tlv_ubytes_t tlv_pack_ubytes(tlv_ubytes_t v, tlv_len_t len);

/**
 * @brief Convert a bytes array to a 16-bit unsigned int.
 * @param ubytes A bytes array
 * @return A 16-bit unsigned int
 */
uint16_t tlv_ubytes_to_uint16(tlv_ubytes_t ubytes);

/**
 * @brief Convert a 16-bit unsigned int to bytes.
 * @param v The value to pack
 * @param ubytes The bytes array to write to
 */
void tlv_uint16_to_ubytes(uint16_t v, tlv_ubytes_t ubytes);

/**
 * @brief Convert a bytes array to a 32-bit unsigned int.
 * @param ubytes A bytes array
 * @return A 32-bit unsigned int
 */
uint32_t tlv_ubytes_to_uint32(tlv_ubytes_t ubytes);

/**
 * @brief Convert a 32-bit unsigned int to bytes.
 * @param v The value to pack
 * @param ubytes The bytes array to write to
 */
void tlv_uint32_to_ubytes(uint32_t v, tlv_ubytes_t ubytes);

/**
 * @brief Convert a bytes array to a 64-bit unsigned long.
 * @param ubytes A bytes array
 * @return A 64-bit unsigned long
 */
ulong tlv_ubytes_to_ulong(tlv_ubytes_t ubytes);

/**
 * @brief Convert an unsigned long value to bytes.
 * @param v The value to pack
 * @param ubytes The bytes array to write to
 */
void tlv_ulong_to_ubytes(ulong v, tlv_ubytes_t ubytes);

/**
 * @brief Print a given TLV structure to stdout.
 * @param tlv The TLV structure to print
 */
void tlv_print(tlv_t *tlv);

/**
 * @brief Print a hexdump of given bytes array.
 * @param ubytes The bytes array to print
 * @param len The number of bytes in the bytes array
 * @param width Number of bytes to print per line, usually a multiple of 8.
 */
void tlv_hexdump(tlv_ubytes_t ubytes, tlv_len_t len, short width);

/**
 * @brief Free any dynamic memory that may have been allocated within the TLV.
 * @warning The TLV structure itself is not freed as it may not have been dynamically allocated. That's up to the caller.
 * @param tlv
 */
void tlv_free(tlv_t *tlv);

#endif // CTLV_H
