// backup
#include <backup_manager.hpp>
// C++ STL
#include <iostream>
// fmt
#include <fmt/format.h>

int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Error: paths to directories are not provided in params.";
        return -1;
    }

    try {
        backup::backup_manager manager(argv[1], argv[2]);
        manager.backup();
    } catch (const std::runtime_error& e) {
        std::cerr << fmt::format("Error: {}", e.what());
        return -1;
    }
    std::cout << "Backup process finished with success." << std::endl;
    return 0;
}
