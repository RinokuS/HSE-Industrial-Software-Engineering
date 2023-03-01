// C++ STL
#include <iostream>
#include <semaphore>
#include <fcntl.h>
#include <pthread.h>
// fmt
#include <fmt/format.h>

namespace {
    std::int32_t threads_number;
    std::int64_t result;
    sem_t *mutex;

    struct thread_args {
        std::int32_t number;
        std::int32_t thread_index;
    };

    void* parallel_factorial(void *args) {
        std::int64_t batch_result = 1;
        thread_args *ta = reinterpret_cast<thread_args*>(args);

        for (std::int32_t i = ta->thread_index; i <= ta->number; i += threads_number) {
            batch_result *= i;
        }

        sem_wait(mutex);
        std::cout << fmt::format("thread №{} evaluated batch: {}", ta->thread_index, batch_result) << std::endl;
        result *= batch_result;
        sem_post(mutex);

        return nullptr;
    }
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error: number of threads is not provided in params.";
        return -1;
    }
    std::uint32_t value;
    std::uint32_t number;
    threads_number = std::stoi(argv[1]);
    result = 1;

    do {
        std::cout << "Please, enter a positive integer number: ";
        std::cin >> number;
        std::cout << std::endl;
    } while (number < 0);

    // opening semaphore and initializing helping arrays for threads
    mutex = sem_open ("pSem", O_CREAT | O_EXCL, 0644, value);
    pthread_t threads[threads_number];
    thread_args thread_args[threads_number];

    for (std::int32_t i = 0; i < threads_number; ++i) {
        thread_args[i].number = number;
        thread_args[i].thread_index = i + 1;

        std::int32_t rc = pthread_create(&threads[i], nullptr, parallel_factorial, (void *)&thread_args[i]);
        if (rc) {
            std::cerr << "Error: pthread_create failed.";
            sem_unlink ("pSem");
            sem_close(mutex);
            return -1;
        }
    }

    for (std::int32_t i = 0; i < threads_number; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << fmt::format("{}! = {}", number, result) << std::endl;

    // unlinking and closing semaphore
    sem_unlink ("pSem");
    sem_close(mutex);

    return 0;
}
