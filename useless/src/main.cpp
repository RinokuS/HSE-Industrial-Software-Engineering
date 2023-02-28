#include <useless.hpp>

#include <iostream>

int main (int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error: path to config file is not provided in params.";
        return -1;
    }

    USELESS::impl useless(argv[1]);
    return useless.run();
}
