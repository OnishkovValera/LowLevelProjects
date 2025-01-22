#include "Sort.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " <количество_повторений>" << '\n';
        return 1;
    }

    int repetitions = std::atoi(argv[1]);

    if (repetitions <= 0) {
        std::cerr << "Ошибка: количество повторений должно быть положительным." << '\n';
        return 1;
    }

    for (int i = 0; i < repetitions; ++i) {
        auto numbers = Generate2kRandomNumbers();
        Sort2k(numbers);
    }
    std::cout << "Отсортированно " << repetitions << " раз" << "\n";

    return 0;
}