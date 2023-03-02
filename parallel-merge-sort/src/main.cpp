#include <thread_sorter.hpp>
// C++ STL
#include <iostream>
#include <filesystem>
#include <fstream>
#include <semaphore>
#include <fcntl.h>
#include <pthread.h>
// fmt
#include <fmt/format.h>

namespace {
    std::filesystem::path path_to_array_of_numbers = "../array_of_numbers.txt";

    std::vector<std::int32_t> read_array() {
        if (!std::filesystem::exists(path_to_array_of_numbers)) {
            exit(-1);
        }

        std::ifstream ifs{path_to_array_of_numbers};
        std::int32_t buf;
        ifs >> buf;
        std::vector<std::int32_t> vec(buf);

        for (auto& elem: vec) {
            ifs >> elem;
        }

        return vec;
    }
}

int main () {
    merge_sort::thread_sorter<std::int32_t> sorter(read_array());
    sorter.sort();

    return 0;
}
