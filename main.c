#include <stdio.h>
#include <strings.h>
#include <stdint.h>

static int write_u16_le(FILE *f, uint16_t v){
    uint8_t b[2];
    b[0] = (uint8_t)(v & 0xFF);
    b[1] = (uint8_t)(v >> 8);
    return fwrite(b, 1, 2, f) == 2 ? 0 : -1;
}

static int write_u32_le(FILE *f, uint32_t v){
    uint8_t b[2];
    b[0] = (uint8_t)(v & 0xFF);
    b[1] = (uint8_t)(v >> 8);
    b[2] = (uint8_t)(v >> 16);
    b[3] = (uint8_t)(v >> 24);
    return fwrite(b, 1, 4, f) == 4 ? 0 : -1;
}

int dcpf_write_header(
        FILE *f,
        uint16_t version,
        uint16_t record_size,
        uint32_t flags,
        uint16_t channel_count
        ){
    if(!f) return -1;

    if(fwrite("DCPF", 1, 4, f) != 4) return -1; //magic
    if(write_u16_le(f, version)) return -1;
    if(write_u16_le(f, record_size)) return -1;
    if(write_u16_le(f, flags)) return -1;
    if(write_u16_le(f, channel_count)) return -1;
}

int createFile(char name[], FILE **out) {
    if (!name || !out) return 1;

    size_t nameLength = strlen(name);
    char nameWithExtension[nameLength + 5];
    strcpy(nameWithExtension, name);
    strcat(nameWithExtension, ".dcp");

    FILE *fptr = fopen(nameWithExtension, "w");

    if(!fptr) return 2;

    *out = fptr;
    return 0;
}

int main(void) {
    FILE *fptr;
    if(createFile("test", &fptr) != 0){
        printf("File creation Failed!");
        return 0;
    }
    printf("File created Succesfully!");

    fprintf(fptr, "abc");

    fclose(fptr);
    return 0;
}
