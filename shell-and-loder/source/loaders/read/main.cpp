#include "io-lat-read.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <имя_файла> <количество_повторений>" << '\n';
        return 1;
    }

    const std::string file_name = argv[1];
    int repetitions = std::atoi(argv[2]);

    if (repetitions <= 0) {
        std::cerr << "Ошибка: количество повторений должно быть положительным." << '\n';
        return 1;
    }
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < repetitions; ++i) {
        auto numbers = IoLatRead(file_name);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Время чтения чисел из файла: " << duration.count() << " секунд." << '\n';
    return 0;
}