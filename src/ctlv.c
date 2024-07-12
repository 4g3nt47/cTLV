#include "ctlv.h"
#include <stdio.h>
#include <string.h>

short tlv_load(tlv_t *tlv, tlv_ubytes_t ubytes){

  tlv->type = (tlv_type_t)ubytes[0];
  tlv->len = tlv_ubytes_to_uint32(ubytes + 1);
  if (tlv->type == TLV_UINT8){
    tlv->value.v_uint8 = (uint8_t)ubytes[TLV_HEADER_SIZE];
  }else if (tlv->type == TLV_UINT16){
    tlv->value.v_uint16 = tlv_ubytes_to_uint16(ubytes + TLV_HEADER_SIZE);
  }else if (tlv->type == TLV_UINT32){
    tlv->value.v_uint32 = tlv_ubytes_to_uint32(ubytes + TLV_HEADER_SIZE);
  }else if (tlv->type == TLV_ULONG){
    tlv->value.v_ulong = tlv_ubytes_to_ulong(ubytes + TLV_HEADER_SIZE);
  }else if (tlv->type == TLV_UBYTES){
    // Allocate memory and copy the bytes so caller can disregard the input bytes safely.
    tlv->value.v_ubytes = (tlv_ubytes_t)malloc(tlv->len);
    if (tlv->value.v_ubytes)
      memcpy(tlv->value.v_ubytes, ubytes + TLV_HEADER_SIZE, tlv->len);
  }else{
    return 1;
  }
  return 0;
}

tlv_ubytes_t tlv_dump(tlv_t *tlv){

  tlv_ubytes_t ubytes = (tlv_ubytes_t)malloc(TLV_HEADER_SIZE + tlv->len);
  if (!ubytes)
    return NULL;
  ubytes[0] = tlv->type;
  tlv_uint32_to_ubytes(tlv->len, ubytes + 1);
  if (tlv->type == TLV_UINT8){
    ubytes[TLV_HEADER_SIZE] = (uint8_t)tlv->value.v_uint8;
  }else if (tlv->type == TLV_UINT16){
    tlv_uint16_to_ubytes(tlv->value.v_uint16, ubytes + TLV_HEADER_SIZE);
  }else if (tlv->type == TLV_UINT32){
    tlv_uint32_to_ubytes(tlv->value.v_uint32, ubytes + TLV_HEADER_SIZE);
  }else if (tlv->type == TLV_ULONG){
    tlv_ulong_to_ubytes(tlv->value.v_ulong, ubytes + TLV_HEADER_SIZE);
  }else if (tlv->type == TLV_UBYTES){
    memcpy(ubytes + TLV_HEADER_SIZE, tlv->value.v_ubytes, tlv->len);
  }else{
    free(ubytes);
    return NULL;
  }
  return ubytes;
}

tlv_ubytes_t tlv_pack_uint8(uint8_t v){

  tlv_len_t len = TLV_HEADER_SIZE + sizeof(v);
  tlv_ubytes_t ubytes = (tlv_ubytes_t)malloc(len);
  if (!ubytes)
    return NULL;
  memset(ubytes, 0, len);
  ubytes[0] = TLV_UINT8;
  ubytes[1] = sizeof(v);
  ubytes[TLV_HEADER_SIZE] = v;
  return ubytes;
}

tlv_ubytes_t tlv_pack_uint16(uint16_t v){

  tlv_len_t len = TLV_HEADER_SIZE + sizeof(v);
  tlv_ubytes_t ubytes = (tlv_ubytes_t)malloc(len);
  if (!ubytes)
    return NULL;
  memset(ubytes, 0, len);
  ubytes[0] = TLV_UINT16;
  ubytes[1] = sizeof(v);
  tlv_uint16_to_ubytes(v, ubytes + TLV_HEADER_SIZE);
  return ubytes;
}

tlv_ubytes_t tlv_pack_uint32(uint32_t v){

  tlv_len_t len = TLV_HEADER_SIZE + sizeof(v);
  tlv_ubytes_t ubytes = (tlv_ubytes_t)malloc(len);
  if (!ubytes)
    return NULL;
  memset(ubytes, 0, len);
  ubytes[0] = TLV_UINT32;
  ubytes[1] = sizeof(v);
  tlv_uint32_to_ubytes(v, ubytes + TLV_HEADER_SIZE);
  return ubytes;
}

tlv_ubytes_t tlv_pack_ulong(ulong v){

  tlv_len_t len = TLV_HEADER_SIZE + sizeof(v);
  tlv_ubytes_t ubytes = (tlv_ubytes_t)malloc(len);
  if (!ubytes)
    return NULL;
  memset(ubytes, 0, len);
  ubytes[0] = TLV_ULONG;
  ubytes[1] = sizeof(v);
  tlv_ulong_to_ubytes(v, ubytes + TLV_HEADER_SIZE);
  return ubytes;
}

tlv_ubytes_t tlv_pack_ubytes(tlv_ubytes_t v, tlv_len_t len){

  tlv_len_t total_len = TLV_HEADER_SIZE + len;
  tlv_ubytes_t ubytes = (tlv_ubytes_t)malloc(total_len);
  if (!ubytes)
    return NULL;
  memset(ubytes, 0, total_len);
  ubytes[0] = TLV_UBYTES;
  tlv_uint32_to_ubytes(len, ubytes + 1);
  memcpy(ubytes + TLV_HEADER_SIZE, v, len);
  return ubytes;
}

uint16_t tlv_ubytes_to_uint16(tlv_ubytes_t ubytes){
  return (ubytes[0] + (ubytes[1] << 8));
}

void tlv_uint16_to_ubytes(uint16_t v, tlv_ubytes_t ubytes){
  ubytes[0] = (v & 0xff);
  ubytes[1] = ((v >> 8) & 0xff);
}

uint32_t tlv_ubytes_to_uint32(u_char *ubytes){

  uint32_t val = 0;
  for (int i = 0; i < sizeof(val); i++)
    val += (ubytes[i] << (8 * i));
  return val;
}

void tlv_uint32_to_ubytes(uint32_t v, tlv_ubytes_t ubytes){

  for (int i = 0; i < sizeof(v); i++){
    ubytes[i] = (v & 0xff);
    v = (v >> 8);
  }
}

ulong tlv_ubytes_to_ulong(tlv_ubytes_t ubytes){

  ulong val = 0;
  for (int i = 0; i < sizeof(val); i++)
    val += ((ulong)ubytes[i] << (8 * i));
  return val;
}

void tlv_ulong_to_ubytes(ulong v, tlv_ubytes_t ubytes){

  for (int i = 0; i < sizeof(v); i++){
    ubytes[i] = (v & 0xff);
    v = (v >> 8);
  }
}

void tlv_print(tlv_t *tlv){

  const char tlv_type_e_names[][12] = {
    "TLV_UINT8", "TLV_UINT16", "TLV_UINT32", "TLV_ULONG", "TLV_UBYTES"
  };
  printf("[*]    Type: %s\n", tlv_type_e_names[tlv->type]);
  printf("[*]     Len: %d\n", tlv->len);
  printf("[*]   Value: ");
  if (tlv->type == TLV_UINT8){
    printf("%u\n", tlv->value.v_uint8);
  }else if (tlv->type == TLV_UINT16){
    printf("%u\n", tlv->value.v_uint16);
  }else if (tlv->type == TLV_UINT32){
    printf("%u\n", tlv->value.v_uint32);
  }else if (tlv->type == TLV_ULONG){
    printf("%lu\n", tlv->value.v_ulong);
  }else{
    printf("\n");
    tlv_hexdump(tlv->value.v_ubytes, tlv->len, 16);
  }
}

void tlv_hexdump(tlv_ubytes_t ubytes, tlv_len_t len, short width){

  for (tlv_len_t i = 0; i < len; i++){
    if (i == 0){ // First item. Print starting address
      printf("%08x:  ", i);
    }else if((i % width) == 0){ // Width reached?
      printf("\n");
      if (i >= len) // Buffer exhausted?
        return; // No need to print address.
      printf("%08x:  ", i); // Print address of new line.
    }
    printf("%02x  ", ubytes[i]); // Print the current byte
  }
  printf("\n");
}

void tlv_free(tlv_t *tlv){

  if (tlv->type == TLV_UBYTES && tlv->value.v_ubytes != NULL)
    free(tlv->value.v_ubytes);
}
