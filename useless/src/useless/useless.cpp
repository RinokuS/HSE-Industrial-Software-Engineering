// USELESS
#include <useless.hpp>
// C++ STL
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
// fmt
#include <fmt/format.h>

namespace USELESS {
    impl::impl(const std::filesystem::path &path_to_file) : m_path_to_file(path_to_file) {
    }

    int impl::run() {
        std::ifstream ifs{m_path_to_file};

        if (!ifs.is_open()) {
            std::cerr << fmt::format("Error: file '{}' cannot be read.", m_path_to_file.c_str());
            return -1;
        }

        std::string command;
        std::int32_t time;
        pid_t pid;

        while (!ifs.eof()) {
            ifs >> time >> command;

            if (time < 0) {
                std::cerr << fmt::format("Error: bad delay format: {}.", time);
                return -1;
            }

            pid = fork();
            if (pid < 0) {
                std::cerr << "Error: child process cannot be created.";
                return -1;
            } else if (pid == 0) {
                sleep(time);
                std::cout << fmt::format("Executing process '{}'.", command) << std::endl;
                system(command.c_str());
                //execlp(command.c_str(), command.c_str(), nullptr);
                return 0;
            }
        }
        while (wait(nullptr) != -1) {};
        return 0;
    }
}
