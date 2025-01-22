#include "Sort.hpp"
#include <algorithm>
#include <random>

void Sort2k(std::vector<int>& numbers) {
    std::sort(numbers.begin(), numbers.end());
}

std::vector<int> Generate2kRandomNumbers() {
    constexpr size_t arraySize = 2048 / sizeof(int); // Размер массива в элементах для 2 KB
    std::vector<int> numbers(arraySize);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 10000); // Генерация чисел от 0 до 10000

    for (auto& num : numbers) {
        num = dist(gen);
    }

    return numbers;
}