#include <iostream>
#include "../read/io-lat-read.cpp"
#include "../sort/Sort.cpp"
#include <chrono>
#include <thread>

static int io_lat_redad(int repetitions, const std::string& file_name){
    for (int i = 0; i < repetitions; i++){  
        IoLatRead(file_name);
    }
    return 0;
}

static int sort(int repetitions, std::vector<int>& numbers){
    for(int i = 0; i < repetitions; i++){
        Sort2k(numbers);
    }
    return 0;
}

int main(int argc, char* argv[]){
    if (argc != 4) {
        std::cerr << "Использование: " << argv[0] << " <имя_файла> <количество_повторений> <количество_потоков>" << '\n';
        return 1;
    }

    const std::string file_name = argv[1];
    int repetitions = std::atoi(argv[2]);
    int thread_count = std::atoi(argv[3]);

    if (repetitions <= 0) {
        std::cerr << "Ошибка: количество повторений должно быть положительным." << '\n';
        return 1;
    }
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(thread_count);
    for (int i = 0; i < thread_count / 2; ++i) {
        threads.emplace_back(io_lat_redad, repetitions, std::ref(file_name));
    }

    std::vector<int> nums = Generate2kRandomNumbers();

    for (int i = 0; i < thread_count / 2; ++i) {
        threads.emplace_back(sort, repetitions, std::ref(nums));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Время чтения чисел из файла: " << duration.count() << " секунд." << '\n';
    return 0;
}