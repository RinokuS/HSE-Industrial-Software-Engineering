// dish-washing
#include <wiper.hpp>
#include <washer.hpp>
#include <consts.hpp>
// C++ STL
#include <iostream>
#include <sys/shm.h>
#include <filesystem>
// fmt
#include <fmt/format.h>

namespace {
    std::int32_t *table;
    bool *table_access;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Error: paths to config files are not provided in params.";
        return -1;
    }
    if (!std::filesystem::exists(argv[1]) || !std::filesystem::exists(argv[2]) || !std::filesystem::exists(argv[3])) {
        std::cerr << "Error: not all provided configs are available.";
        return -1;
    }

    std::int32_t shared_memory_table = shmget(IPC_PRIVATE, sizeof(std::int32_t) * dish_washing::TABLE_LIMIT,
                                              IPC_CREAT | 0666);
    std::int32_t shared_memory_ta = shmget(IPC_PRIVATE, sizeof(bool) * dish_washing::TABLE_LIMIT, IPC_CREAT | 0666);
    if (shared_memory_table < 0 || shared_memory_ta < 0) {
        std::cerr << "Error: shmget failed.";
        return -1;
    }
    table = reinterpret_cast<std::int32_t *>(shmat(shared_memory_table, nullptr, 0));
    table_access = reinterpret_cast<bool *>(shmat(shared_memory_ta, nullptr, 0));
    // initializing shared parameters
    for (int i = 0; i < dish_washing::TABLE_LIMIT; ++i) {
        table_access[i] = true;
        table[i] = 0;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Error: fork failed.";
        return -1;
    } else if (pid == 0) {
        dish_washing::washer w(argv[2], argv[1], table, table_access);

        w.wash();
        return 0;
    }
    dish_washing::wiper w(argv[3], table, table_access);
    w.wipe();

    std::cout << "Work is done!" << std::endl;

    shmdt(table);
    shmdt(table_access);
    shmctl(shared_memory_table, IPC_RMID, nullptr);
    shmctl(shared_memory_ta, IPC_RMID, nullptr);
    return 0;
}
