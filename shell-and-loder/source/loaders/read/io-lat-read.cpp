#include "io-lat-read.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<int> IoLatRead(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << file_name << std::endl;
        return {};
    }


    std::vector<int> numbers;
    std::string line;
    while (std::getline(file, line)) {
        // std::istringstream lineStream(line);
        // int number;
        // while (lineStream >> number) {
        //     numbers.push_back(number);
        // }
    }

    if (file.eof()) {
        // Достигнут конец файла
    } else if (file.fail()) {
        std::cerr << "Ошибка чтения файла." << std::endl;
    }

    return numbers;
}
