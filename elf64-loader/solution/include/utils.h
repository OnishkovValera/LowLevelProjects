#ifndef UTILS_H
#define UTILS_H
#include <asm-generic/errno-base.h>
#include <elf.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int8_t open_file(char *file, int32_t *fd);

int8_t read_header(Elf64_Ehdr *file_header, const int32_t fd);

int8_t find_section(Elf64_Shdr *section_header, const char* section_name, const Elf64_Ehdr *file_header, const int32_t fd);

int8_t load_headers(const Elf64_Ehdr *file_header, const int32_t fd);

int8_t program_start(Elf64_Shdr *section_header);

#endif //UTILS_H
