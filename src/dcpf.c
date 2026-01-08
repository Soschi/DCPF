#include <stdio.h>
#include <string.h>
#include <stdint.h>

//infos
#define DCPF_VERSION     1u
#define DCPF_RECORD_SIZE 12u
#define DCPF_FILE_FLAGS  0u

// ---- type field: bits 0..3 ----
#define DCPF_TYPE_MASK   0x000Fu
#define DCPF_TYPE_U32    0x0001u
#define DCPF_TYPE_I32    0x0002u
#define DCPF_TYPE_F32    0x0003u
#define DCPF_TYPE_U64    0x0004u
#define DCPF_TYPE_I64    0x0005u
#define DCPF_TYPE_F64    0x0006u

// ---- extra flags ----
#define DCPF_FLAG_CONT   0x0010u

// Reserved bits (must be 0 in v1)
#define DCPF_FLAG_RESERVED_MASK 0xFFE0u

//Reserved Channel
#define DCPF_CONT_CHANNEL 0xFFFF

//help functions

static int write_u16_le(FILE *f, uint16_t v){
    uint8_t b[2];
    b[0] = (uint8_t)(v & 0xFF);
    b[1] = (uint8_t)(v >> 8);
    return fwrite(b, 1, 2, f) == 2 ? 0 : -1;
}

static int write_u32_le(FILE *f, uint32_t v){
    uint8_t b[4];
    b[0] = (uint8_t)(v & 0xFF);
    b[1] = (uint8_t)(v >> 8);
    b[2] = (uint8_t)(v >> 16);
    b[3] = (uint8_t)(v >> 24);
    return fwrite(b, 1, 4, f) == 4 ? 0 : -1;
}

static uint64_t double_to_u64_bits(double x) {
    uint64_t u;
    memcpy(&u, &x, sizeof u);
    return u;
}

static inline void u64_to_u32_pair(uint64_t v, uint32_t *lo, uint32_t *hi) {
    *lo = (uint32_t)(v & 0xFFFFFFFFu);
    *hi = (uint32_t)(v >> 32);
}

//write data functions

int dcpfWriteDataI32(FILE *f, uint16_t channelid, int32_t data, uint32_t timestamp) {
    if (!f) return -1;
    if (channelid == DCPF_CONT_CHANNEL) return -1;


    if (write_u16_le(f, channelid)) return -1;
    if (write_u16_le(f, DCPF_TYPE_I32)) return -1;
    if (write_u32_le(f, (uint32_t)data)) return -1;
    if (write_u32_le(f, timestamp)) return -1;

    return 0;
}

int dcpfWriteDataU32(FILE *f, uint16_t channelid, uint32_t data, uint32_t timestamp) {
    if (!f) return -1;
    if (channelid == DCPF_CONT_CHANNEL) return -1;


    if (write_u16_le(f, channelid)) return -1;
    if (write_u16_le(f, DCPF_TYPE_U32)) return -1;
    if (write_u32_le(f, data)) return -1;
    if (write_u32_le(f, timestamp)) return -1;

    return 0;
}

int dcpfWriteDataF32(FILE *f, uint16_t channelid, float data, uint32_t timestamp) {
    if (!f) return -1;
    if (channelid == DCPF_CONT_CHANNEL) return -1;


    uint32_t convertedData;
    memcpy(&convertedData, &data, sizeof convertedData);

    if (write_u16_le(f, channelid)) return -1;
    if (write_u16_le(f, DCPF_TYPE_F32)) return -1;
    if (write_u32_le(f, convertedData)) return -1;
    if (write_u32_le(f, timestamp)) return -1;

    return 0;
}

int dcpfWriteDataF64(FILE *f, uint16_t channelid, double data, uint32_t timestamp) {
    if (!f) return -1;
    if (channelid == DCPF_CONT_CHANNEL) return -1;


    uint64_t convertedData = double_to_u64_bits(data);
    uint32_t lo, hi;
    u64_to_u32_pair(convertedData, &lo, &hi);

    if (write_u16_le(f, channelid)) return -1;
    if (write_u16_le(f, DCPF_TYPE_F64 | DCPF_FLAG_CONT)) return -1;
    if (write_u32_le(f, lo)) return -1;
    if (write_u32_le(f, timestamp)) return -1;

    if (write_u16_le(f, DCPF_CONT_CHANNEL)) return -1;
    if (write_u16_le(f, DCPF_TYPE_F64)) return -1;
    if (write_u32_le(f, hi)) return -1;
    if (write_u32_le(f, timestamp)) return -1;

    return 0;
}

int dcpfWriteDataU64(FILE *f, uint16_t channelid, uint64_t data, uint32_t timestamp) {
    if (!f) return -1;
    if (channelid == DCPF_CONT_CHANNEL) return -1;


    uint32_t lo, hi;
    u64_to_u32_pair(data, &lo, &hi);

    if (write_u16_le(f, channelid)) return -1;
    if (write_u16_le(f, DCPF_TYPE_U64 | DCPF_FLAG_CONT)) return -1;
    if (write_u32_le(f, lo)) return -1;
    if (write_u32_le(f, timestamp)) return -1;

    if (write_u16_le(f, DCPF_CONT_CHANNEL)) return -1;
    if (write_u16_le(f, DCPF_TYPE_U64)) return -1;
    if (write_u32_le(f, hi)) return -1;
    if (write_u32_le(f, timestamp)) return -1;

    return 0;
}

int dcpfWriteDataI64(FILE *f, uint16_t channelid, int64_t data, uint32_t timestamp) {
    if (!f) return -1;
    if (channelid == DCPF_CONT_CHANNEL) return -1;


    uint32_t lo, hi;
    u64_to_u32_pair((uint64_t)data, &lo, &hi);

    if (write_u16_le(f, channelid)) return -1;
    if (write_u16_le(f, DCPF_TYPE_I64 | DCPF_FLAG_CONT)) return -1;
    if (write_u32_le(f, lo)) return -1;
    if (write_u32_le(f, timestamp)) return -1;

    if (write_u16_le(f, DCPF_CONT_CHANNEL)) return -1;
    if (write_u16_le(f, DCPF_TYPE_I64)) return -1;
    if (write_u32_le(f, hi)) return -1;
    if (write_u32_le(f, timestamp)) return -1;

    return 0;
}

//organisation functions

int dcpf_write_header(FILE *f){
    if(!f) return -1;

    if(fwrite("DCPF", 1, 4, f) != 4) return -1; //magic
    if(write_u16_le(f, DCPF_VERSION)) return -1;
    if(write_u16_le(f, DCPF_RECORD_SIZE)) return -1;
    if(write_u32_le(f, DCPF_FILE_FLAGS)) return -1;

    if(write_u16_le(f, 0)) return -1;
    if(write_u16_le(f, 0)) return -1;

    return 0;
}

int dcpfCreateFile(char name[], FILE **out) {
    if (!name || !out) return 1;

    size_t nameLength = strlen(name);
    char nameWithExtension[nameLength + 5];
    strcpy(nameWithExtension, name);
    strcat(nameWithExtension, ".dcp");

    FILE *fptr = fopen(nameWithExtension, "wb");

    if(!fptr) return 2;

    if (dcpf_write_header(fptr) != 0) { fclose(fptr); return 3; }

    *out = fptr;
    return 0;
}

int dcpfCloseFile(FILE *f) {
    if (!f) return -1;
    return fclose(f) == 0 ? 0 : 1;
}
