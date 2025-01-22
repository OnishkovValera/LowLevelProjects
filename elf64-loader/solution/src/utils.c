#include "utils.h"

int8_t open_file(char *file, int32_t *fd) {
    *fd = open(file, O_RDONLY);
    if (*fd < 0) {
        return ENOENT;
    }
    return 0;
}

int8_t get_names_header(Elf64_Shdr *names, const Elf64_Ehdr *file_header, const int32_t fd) {

    const uint64_t offset = file_header->e_shoff + sizeof(Elf64_Shdr) * file_header->e_shstrndx;

    if (lseek(fd, (off_t) offset, SEEK_SET) < 0) {
        return EIO;
    }

    const size_t size = read(fd, names, sizeof(*names));

    if (size != sizeof(*names)) {
        return EIO;
    }
    return 0;
}

int8_t read_header(Elf64_Ehdr *file_header, const int32_t fd) {
    const size_t size = read(fd, file_header, sizeof(*file_header));
    if (size != sizeof(*file_header)) {
        return EIO;
    }
    if (file_header->e_ident[0] != ELFMAG0) {
        return EINVAL;
    }
    if (file_header->e_ident[1] != ELFMAG1) {
        return EINVAL;
    }
    if (file_header->e_ident[2] != ELFMAG2) {
        return EINVAL;
    }
    if (file_header->e_ident[3] != ELFMAG3) {
        return EINVAL;
    }
    return 0;
}



int8_t get_section_header(Elf64_Shdr *section, const int32_t index, const Elf64_Ehdr *file_header, const int32_t fd) {
    const uint64_t offset_to_section = file_header->e_shoff + sizeof(*section) * index;
    if (lseek(fd, (off_t) offset_to_section, SEEK_SET) < 0) {
        return EIO;
    }

    const size_t size = read(fd, section, sizeof(*section));
    if (size != sizeof(*section)) {
        return EIO;
    }
    return 0;
}

int8_t check_names_equals(int8_t *res, const char* target_section_name, const Elf64_Shdr *section, const Elf64_Shdr *table_names, const int32_t fd) {
    const uint64_t offset_to_section = table_names->sh_offset + section->sh_name;
    if (lseek(fd, (off_t) offset_to_section, SEEK_SET) < 0) {
        return EIO;
    }

    while (1) {
        char c;
        char tc;

        const size_t size = read(fd, &c, 1);
        if (size != 1) {
            return EIO;
        }
        tc = *target_section_name;
        target_section_name++;

        if (c != tc) {
            *res = 1;
            return 0;
        }
        if (c == '\0' && tc == '\0') {
            *res = 0;
            return 0;
        }
    }
}

int8_t find_section(Elf64_Shdr *section_header, const char* section_name, const Elf64_Ehdr *file_header, const int32_t fd) {
    Elf64_Shdr names;
    int8_t status = get_names_header(&names, file_header, fd);
    if (status != 0) {
        return status;
    }

    const int32_t sections_count = file_header->e_shnum;
    for (int32_t i = 0; i < sections_count; i++) {
        Elf64_Shdr section;
        status = get_section_header(&section, i, file_header, fd);

        if (status != 0) {
            return status;
        }

        int8_t res = 1;
        check_names_equals(&res, section_name, &section, &names, fd);

        if (res == 0) {

            status = get_section_header(section_header, i, file_header, fd);

            if (status != 0) {
                return status;
            }

            if (!(section_header->sh_flags & SHF_EXECINSTR)) {
                return EINVAL;
            }

            return 0;
        }
    }
    return EINVAL;
}

int8_t get_program_header(Elf64_Phdr *program_header, const int32_t index, const Elf64_Ehdr *file_header, const int32_t fd) {
    const uint64_t offset_to_header = file_header->e_phoff + sizeof(*program_header) * index;
    if (lseek(fd, (off_t) offset_to_header, SEEK_SET) < 0) {
        return EIO;
    }

    const size_t size = read(fd, program_header, sizeof(*program_header));
    if (size != sizeof(*program_header)) {
        return EIO;
    }
    return 0;
}

int8_t get_protection_flags(Elf64_Phdr *program_header) {
    int8_t protection_flags = 0;
    if (program_header->p_flags & PF_W) {
        protection_flags |= PROT_WRITE;
    }
    if (program_header->p_flags & PF_R) {
        protection_flags |= PROT_READ;
    }
    if (program_header->p_flags & PF_X) {
        protection_flags |= PROT_EXEC;
    }
    return protection_flags;
}

int8_t memory_allocation(Elf64_Phdr *program_header, const int32_t fd) {
    if (program_header->p_type != PT_LOAD) {
        return 0;
    }
    Elf64_Addr addr = program_header->p_vaddr;
    Elf64_Off offset = program_header->p_offset;
    Elf64_Xword mem = program_header->p_memsz;
    mem += addr % 0x1000;
    offset -= addr % 0x1000;
    addr -= addr % 0x1000;
    const int8_t protection_flags = get_protection_flags(program_header);
    const void* status = mmap(
        (void*) addr, // NOLINT
        mem,
        protection_flags,
        MAP_PRIVATE | MAP_FIXED | MAP_FIXED_NOREPLACE,
        fd,
        (off_t) offset);
    if (status == MAP_FAILED) {
        return EIO;
    }
    return 0;
}

int8_t load_headers(const Elf64_Ehdr *file_header, const int32_t fd) {
    const int32_t program_headers_count = file_header->e_phnum;
    for (int32_t i = 0; i < program_headers_count; i++) {
        Elf64_Phdr program_header;
        int8_t status = get_program_header(&program_header, i, file_header, fd);
        if (status != 0) {
            return status;
        }

        status = memory_allocation(&program_header, fd);
        if (status != 0) {
            return status;
        }
    }
    return 0;
}

int8_t program_start(Elf64_Shdr *section_header) {
    typedef void (*func)(void);

    func start = (func) section_header->sh_addr; // NOLINT

    start();
    return 0;
}
