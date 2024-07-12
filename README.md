# cTLV

**cTLV** is a simple library for working with TLV (type-length-value) structures. It can be used in C and C++ programs, and should also work cross-platform.

## Build

1. Clone the repo;

```shell
git clone https://github.com/4g3nt47/cTLV.git
```

2. Compile;

```shell
cd cTLV
mkdir build
cd build
cmake ..
make
```

You should now have a `libctlv.so` binary file. Simply copy this file and the header file at `include/ctlv.h` to your own project directory and you should be good to go.

## Usage examples

1. Create a TLV bytes array containing the packed structure of a 32-bit unsigned integer value;

```c
tlv_ubytes_t bytes = tlv_pack_uint32(20);
tlv_hexdump(bytes, TLV_HEADER_SIZE + sizeof(uint32_t), 16);
```

The variable `bytes` now contains the complete data of our TLV structure. Note that the length of the `bytes` array is `TLV_HEADER_SIZE` (the number of bytes occupied by the TLV value type, and TLV value length) + `sizeof(uint32_t)` (the number of bytes needed to store the packed 32-bit unsigned int). We can now send these bytes to another device, or save it to a file for later use. Just make sure to free the memory by calling `free(bytes)` when you are done with it to avoid leaks.

2. Parsing a TLV bytes array;

We can parse the TLV `bytes` array generated in the previous example using;

```c
tlv_t my_tlv;
if (tlv_load(&my_tlv, bytes)){
  // Error has occured while loading. Handle it.
}else{
  // Successful parsing. We can now free the $bytes array.
  free(bytes);
  // Now use the TLV as you wish
  tlv_print(&my_tlv);
  // When you are done, always call tlv_free() to free up any memory that may have been allocated within the structure
  tlv_free(&my_tlv);
}
```

3. Dump an existing TLV structure;

```c
tlv_ubytes_t bytes = tlv_dump(&my_tlv);
// free it after u are done
free(bytes)
```

4. Creating a TLV structure from scratch;

Following example shows how to create a TLV struct to hold some bytes.

```c
char username[] = "4g3nt47";
tlv_t my_tlv;
my_tlv.type = TLV_UBYTES;
my_tlv.len = strlen(username) + 1; // Extra bytes for a null since we aren't working with binary data
my_tlv.value.v_ubytes = (tlv_ubytes_t)malloc(my_tlv.len);
memcpy(my_tlv.value.v_ubytes, username, my_tlv.len);
// Our struct is now ready. Use it.
tlv_print(&my_tlv);
// Cleanup
tlv_free(&my_tlv);
```

## Documentation

This library is documented using *Doxygen*. To generate the docs, make sure you have `doxygen` installed, and you are in the project root directory where `Doxyfile` is located. Then run;

```shell
doxygen ./Doxyfile
```

This will generate the documentation in `doc/`
