// runsim
#include <runner.hpp>
// C++ STL
#include <iostream>
#include <sys/shm.h>
#include <sys/wait.h>
// fmt
#include <fmt/format.h>

namespace {
    std::uint32_t buf_size = 1024;
    std::int32_t *counter_ptr;
}

namespace runsim {
    runner::runner(std::int32_t max_processes)
            : m_max_processes(max_processes) {
    }

    void runner::run_process(char *command) {
        if (counter_ptr[0] == m_max_processes) {
            std::cout << "The maximum possible number of processes are currently running. Please wait." << std::endl;
            return;
        }

        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "Error: fork failed.";
            exit(-1);
        } else if (pid == 0) {
            counter_ptr[0]++;
            //sleep(3); // for better testing
            system(command);
            counter_ptr[0]--;
            exit(0);
        }
    }

    std::int32_t runner::run() {
        std::int32_t shared_memory_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);

        if (shared_memory_id < 0) {
            std::cerr << "Error: shmget failed.";
            return -1;
        }

        counter_ptr = reinterpret_cast<std::int32_t *>(shmat(shared_memory_id, nullptr, 0));
        counter_ptr[0] = 0; // init counter

        while (!feof(stdin)) {
            char command_buf[buf_size];
            scanf("%s", command_buf);
            run_process(command_buf);
        }

        while (wait(nullptr) != -1) {}
        shmdt(counter_ptr);
        shmctl(shared_memory_id, IPC_RMID, nullptr);

        return 0;
    }
}
