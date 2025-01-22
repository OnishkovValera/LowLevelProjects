#include "BMP_reader.h"
#include "image.h"
#include "image_transformer.h"
#include "main.h"


int64_t validate(char* angle){
    if(strcmp("90", angle) != 0 ||
       strcmp("180", angle) != 0 ||
       strcmp("270", angle) != 0 ||
       strcmp("-90", angle) != 0 ||
       strcmp("-180", angle) != 0 ||
       strcmp("-270", angle) != 0 ||
       strcmp("0", angle) != 0){
        return ((360 +atoi(angle))%360/90);
    }
    return -1;
}


int main( int argc, char** argv ) {
    if(argc != 4){
        printf("Wrong number of arguments, it must be 4");
        return 0;
    }
    
    int64_t number = validate(argv[3]);

    if(number == -1){
        printf("Wrong angle");
        return 0;
    }

    const char* file_name = argv[1];


    FILE* file = fopen(file_name, "rb");
    if(!file){
        return READ_INVALID_FILE;
    }
    struct image image;
    enum read_status read_status = from_bmp(file, &image);

    fclose(file);
    if(read_status){
        return read_status;
    }
    FILE* out = fopen(argv[2], "wb");
    struct image rotated_image = rotate_number(image, number);
    enum write_status status = to_bmp(out, &rotated_image);
    fclose(out);
    destroy_image(&rotated_image);
    if(status){
        return status;
    }


    return 0;

}
