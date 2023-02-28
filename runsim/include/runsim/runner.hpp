#pragma once

#include <filesystem>

namespace runsim {
    class runner {
    public:
        runner(std::int32_t max_processes);

        std::int32_t run();

    private:
        void run_process(char* command);

        std::int32_t m_max_processes;
    };
}
