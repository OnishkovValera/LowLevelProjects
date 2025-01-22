#include <iostream>
#include <vector>
#include "../monolith/app/cache/Cache.cpp" 
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    const char* input_file = argv[1];
    // Открываем файл с использованием функции lab2_open
    int fd = lab2_open(input_file);
    if (fd == -1) {
        std::cerr << "Failed to open file: " << input_file << std::endl;
        return 1;
    }
    const size_t buffer_size = 1024;
    std::vector<int> buffer(buffer_size);
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        ssize_t bytes_read = lab2_read(fd, buffer.data(), buffer_size * sizeof(int));
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << "Time taken for reading: " << elapsed_seconds.count() << " seconds." << std::endl;
    if (lab2_close(fd) == -1) {
        std::cerr << "Failed to close file: " << input_file << std::endl;
        return 1;
    }
    return 0;
}
