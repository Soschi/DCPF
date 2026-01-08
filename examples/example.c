#include "dcpf.h"

int main(void) {
    FILE *f;
    if (dcpfCreateFile("dataset", &f) != 0) return 1;

    dcpfWriteDataU32(f, 0, 1, 21);
    dcpfWriteDataI32(f, 0, 123123, 2323331);
    dcpfWriteDataF32(f, 1, 3123.0f, 2233);
    dcpfWriteDataU64(f, 2, 31231ULL, 2123);

    dcpfCloseFile(f);
    return 0;
}