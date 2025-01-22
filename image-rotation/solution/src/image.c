#include "image.h"
#include <stdio.h>
struct image init_image(uint64_t width, uint64_t height){

    struct image image = (struct image){
        .height = height,
        .width = width,
        .pixels = malloc(sizeof(struct pixel) * height * width)
    };

    return image;
}
void destroy_image(struct image* image){
    if(image->pixels == NULL) {
        return;
    }
    free(image->pixels);
    image->pixels = NULL;

}

