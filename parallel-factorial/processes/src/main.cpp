// C++ STL
#include <iostream>
#include <semaphore>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/wait.h>
// fmt
#include <fmt/format.h>

namespace {
    std::int32_t threads_number;
    std::int64_t *result;
    sem_t *mutex;

    std::int64_t parallel_factorial(std::int32_t number, std::int32_t process_index) {
        std::int64_t batch_result = 1;
        for (std::int32_t i = process_index; i <= number; i += threads_number) {
            batch_result *= i;
        }

        return batch_result;
    }
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error: number of processes is not provided in params.";
        return -1;
    }
    std::uint32_t value;
    std::uint32_t number;
    threads_number = std::stoi(argv[1]);
    // initializing shared memory for result
    std::int32_t shared_memory_id = shmget(IPC_PRIVATE, sizeof(std::int64_t), IPC_CREAT | 0666);

    if (shared_memory_id < 0) {
        std::cerr << "Error: shmget failed.";
        return -1;
    }

    result = reinterpret_cast<std::int64_t *>(shmat(shared_memory_id, nullptr, 0));
    result[0] = 1; // init counter

    do {
        std::cout << "Please, enter a positive integer number: ";
        std::cin >> number;
        std::cout << std::endl;
    } while (number < 0);
    // initializing semaphore
    mutex = sem_open ("pSem", O_CREAT | O_EXCL, 0644, value);
    for (std::int32_t i = 1; i <= threads_number; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            sem_unlink ("pSem");
            sem_close(mutex);
            std::cerr << "Error: fork failed.";

            return -1;
        } else if (pid == 0) {
            auto batch_result = parallel_factorial(number, i);

            sem_wait(mutex);
            std::cout << fmt::format("process №{} evaluated batch: {}", i, batch_result) << std::endl;
            result[0] *= batch_result;
            sem_post(mutex);

            return 0;
        }
    }
    // waiting for child processes
    while (wait(nullptr) != -1) {}

    std::cout << fmt::format("{}! = {}", number, result[0]) << std::endl;
    // unlinking and closing semaphore
    sem_unlink ("pSem");
    sem_close(mutex);
    // clearing shared memory
    shmdt(result);
    shmctl(shared_memory_id, IPC_RMID, nullptr);

    return 0;
}
