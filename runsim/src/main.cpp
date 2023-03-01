// runsim
#include <runner.hpp>
// C++ STL
#include <iostream>
// fmt
#include <fmt/format.h>

int main (int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error: number of processes is not provided in params.";
        return -1;
    }

    runsim::runner process_runner(std::stoi(argv[1]));
    return process_runner.run();
}
