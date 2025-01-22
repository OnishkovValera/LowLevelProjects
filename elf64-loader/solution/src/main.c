#include "utils.h"

int main( int argc, char** argv ) {
    (void) argc; (void) argv;

    if (argc != 3) {
        return EINVAL;
    }
    char* file_name = argv[1];
    const char* section = argv[2];
    int32_t fd;
    int8_t status = open_file(file_name, &fd);

    if (status != 0) {
        return status;
    }

    Elf64_Ehdr header;
    status = read_header(&header, fd);

    if (status != 0) {
        return status;
    }

    Elf64_Shdr target_section_header;
    status = find_section(&target_section_header, section, &header, fd);
    if (status != 0) {
        return status;
    }

    status = load_headers(&header, fd);
    if (status != 0) {
        return status;
    }

    status = program_start(&target_section_header);
    if (status != 0) {
        return status;
    }

    return 0;
}
