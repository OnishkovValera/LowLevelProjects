#include "image_transformer.h"
struct image rotate(struct image const source ){
    uint64_t newHeight = source.width;
    uint64_t newWidth = source.height;
    printf("%"PRIu64"\n", newHeight);
    printf("%"PRIu64, newWidth);
    struct image newImage = init_image(newWidth, newHeight);
    for(uint64_t i = 0; i < newHeight; i++){
        for(uint64_t j = 0; j < newWidth; j++){
            newImage.pixels[newWidth*i + j] = source.pixels[source.width * j + source.width - i - 1];
        }
    }
    return newImage;
}

struct image rotate_number( struct image const source, uint64_t count){
    struct image image = source;

    for(int i = 0; i < count; i++){
        struct image new_image = rotate(image);
        destroy_image(&image);
        image = new_image;
    }
    return image;
}

