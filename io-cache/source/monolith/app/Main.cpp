#include <iostream>
#include "cache/Cache.cpp"

int main() {

    srand(time(0));

    const char* filename = "test_file.txt";
    
    int fd = lab2_open(filename);
    if (fd == -1) {
        std::cerr << "Не удалось открыть файл" << std::endl;
        return 1;
    }

    char large_read_buffer[1024];
    ssize_t large_bytes_read = lab2_read(fd, large_read_buffer, sizeof(large_read_buffer));
    std::cout << "Прочитано данных (большой буфер): " << large_bytes_read << " байт" << std::endl;
    std::cout << "Содержимое (большой буфер): " << large_read_buffer << std::endl;

    lab2_fsync(fd);
    lab2_lseek(fd, 0, SEEK_SET);
    lab2_close(fd);
    return 0;

}