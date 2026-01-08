#pragma once
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    int dcpfCreateFile(char name[], FILE **out);
    int dcpfCloseFile(FILE *f);

    int dcpfWriteDataI64(FILE *f, uint16_t channelid, int64_t data, uint32_t timestamp);
    int dcpfWriteDataU64(FILE *f, uint16_t channelid, uint64_t data, uint32_t timestamp);
    int dcpfWriteDataF64(FILE *f, uint16_t channelid, double data, uint32_t timestamp);

    int dcpfWriteDataF32(FILE *f, uint16_t channelid, float data, uint32_t timestamp);
    int dcpfWriteDataU32(FILE *f, uint16_t channelid, uint32_t data, uint32_t timestamp);
    int dcpfWriteDataI32(FILE *f, uint16_t channelid, int32_t data, uint32_t timestamp);


#ifdef __cplusplus
}
#endif