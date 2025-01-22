#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <chrono> 
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    const char* input_file = argv[1];
    int fd = open(input_file, O_RDONLY | O_DIRECT);
    if (fd == -1) {
        std::cerr << "Failed to open file: " << input_file << " (" << strerror(errno) << ")" << std::endl;
        return 1;
    }
    const size_t block_size = 4096; 
    const size_t buffer_size = 1024;
    void* aligned_buffer = aligned_alloc(block_size, buffer_size * sizeof(int));
    if (!aligned_buffer) {
        std::cerr << "Failed to allocate aligned memory." << std::endl;
        close(fd);
        return 1;
    }
    size_t total_numbers_read = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<int> buffer(buffer_size);
    for (int i = 0; i < 1000; i++) {
        ssize_t bytes_read = read(fd, buffer.data(), buffer_size * sizeof(int));
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    free(aligned_buffer);
    close(fd);
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << "Time taken for reading: " << elapsed_seconds.count() << " seconds." << std::endl;
    return 0;
}
