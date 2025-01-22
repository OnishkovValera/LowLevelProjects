#include "BMP_reader.h"


enum write_status to_bmp(FILE* out, struct image const* img ){
    uint32_t biSizeImage = ( (uint32_t) (img->width*sizeof(struct pixel)+padding(img->width)) )*img->height;

    struct bmp_header header = (struct bmp_header){
            .bfType = TYPE,
            .bfReserved = RESERVED,
            .bfileSize = biSizeImage + OFF_BITS,
            .bfOffBits = OFF_BITS,
            .biSize = DIB_HEADER_SIZE,
            .biWidth = img->width,
            .biHeight = img->height,
            .biPlanes = PLANES,
            .biBitCount = BYTES_COUNT*8,
            .biCompression = COMPRESSION,
            .biSizeImage = biSizeImage,
            .biXPelsPerMeter = X_PELS_PER_METER,
            .biYPelsPerMeter = Y_PELS_PER_METER,
            .biClrUsed = CLR_USED,
            .biClrImportant = CLR_IMPORTANT
    };

    if (fwrite(&header, sizeof(header), 1, out) < 1) {
        return WRITE_ERROR;
    }

    char *bytes = malloc(header.biSizeImage);
    uint64_t width = img->width;
    uint64_t height = img->height;
    size_t pad = padding(width);
    size_t bytes_per_row = width* BYTES_COUNT + pad;

    for(uint64_t i = 0; i < height; i++){

        for(uint64_t j = 0; j < width; j++){
            bytes[bytes_per_row * i + j * BYTES_COUNT] = img->pixels[width*i+j].b;
            bytes[bytes_per_row * i + j * BYTES_COUNT + 1] = img->pixels[width*i+j].g;
            bytes[bytes_per_row * i + j * BYTES_COUNT + 2] = img->pixels[width*i+j].r;
        }

        for(uint64_t j = 0; j < pad;j++){
            bytes[bytes_per_row * i + width * BYTES_COUNT + j] = 0;
        }
    }

    if (fwrite(bytes, 1, header.biSizeImage, out) < header.biSizeImage) {
        free(bytes);
        return WRITE_ERROR;
    }
    free(bytes);
    return WRITE_OK;
}


enum read_status from_bmp(FILE* in, struct image* img ){
    struct bmp_header header;
    enum read_status status = read_header(in, &header);

    if(status){
        return status;
    }

    *img = init_image(header.biWidth, header.biHeight);

    char* bytes = malloc(header.biSizeImage);

    if (fread(bytes, header.biSizeImage, 1, in) < 1) {
        free(bytes);
        return READ_INVALID_SIGNATURE;
    }

    size_t pad = padding(img->width);
    size_t bytesPerRow = img->width * BYTES_COUNT + pad;
    struct pixel* data = img->pixels;

    for(uint64_t i = 0; i < img->height; i++){
        for(uint64_t j = 0; j < img->width; j++){
            data[i * img->width + j] = (struct pixel){
                    bytes[bytesPerRow * i + j * BYTES_COUNT],
                    bytes[bytesPerRow * i + j * BYTES_COUNT + 1],
                    bytes[bytesPerRow * i + j * BYTES_COUNT + 2]
            };
        }
    }

    free(bytes);


    return READ_OK;
}

enum read_status read_header(FILE* name, struct bmp_header* header){
    if (fread(header, sizeof(struct bmp_header), 1, name) < 1) {
        return READ_INVALID_HEADER;
    }

    if(
            header->bfType != TYPE ||
            header->bfReserved != RESERVED ||
            header->bfOffBits != OFF_BITS ||
            header->biPlanes != PLANES ||
            header->biSize != DIB_HEADER_SIZE ||
            header->biWidth < 1 ||
            header->biHeight < 1 ||
            header->biBitCount != BYTES_COUNT*8 ||
            header->biCompression != COMPRESSION
            ) {
        return READ_INVALID_BITS;
    }

    return READ_OK;
}



size_t padding(uint64_t width){
    return 4 - (width *BYTES_COUNT)%4;

}

