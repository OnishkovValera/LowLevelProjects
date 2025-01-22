#ifndef IMAGE_TRANSFORMER_BMP_READER_H
#define IMAGE_TRANSFORMER_BMP_READER_H

#include "image.h"

#include <stdio.h>

#define TYPE 19778
#define RESERVED 0
#define OFF_BITS 54
#define DIB_HEADER_SIZE 40
#define PLANES 1
#define BYTES_COUNT 3
#define COMPRESSION 0
#define X_PELS_PER_METER 0
#define Y_PELS_PER_METER 0
#define CLR_USED 0
#define CLR_IMPORTANT 0

#pragma pack(push, 1)
struct bmp_header
{
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bfOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

enum read_status  {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_INVALID_FILE
};

enum  write_status  {
    WRITE_OK = 0,
    WRITE_ERROR
};

enum read_status read_header(FILE* name, struct bmp_header* header);


enum write_status to_bmp(FILE* out, struct image const* img );

enum read_status from_bmp(FILE* in, struct image* img );

size_t padding(uint64_t width);


#endif
