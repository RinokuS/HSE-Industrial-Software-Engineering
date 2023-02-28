#pragma once

#include <filesystem>

namespace USELESS {
    class impl {
    public:
        impl(const std::filesystem::path &path_to_file);

        int run();

    private:
        std::filesystem::path m_path_to_file;
    };
}
