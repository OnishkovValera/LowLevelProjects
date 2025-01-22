#ifndef IMAGE_TRANSFORMER_IMAGE_H
#define IMAGE_TRANSFORMER_IMAGE_H

#include <inttypes.h>
#include <stdlib.h>

struct pixel{
    int8_t b, g, r;
};

struct image{
    struct pixel* pixels;
    uint64_t width, height;

};

struct image init_image(uint64_t width, uint64_t height);

void destroy_image(struct image* image);

#endif
