#pragma once

#include <filesystem>

namespace backup {
    class backup_manager {
    public:
        backup_manager(const std::filesystem::path &path_to_real_dir, const std::filesystem::path &path_to_backup);

        void backup();

    private:
        bool should_backup(const std::filesystem::path &from, const std::filesystem::path &to);

        void copy(const std::filesystem::path &from, const std::filesystem::path &to);

        void backup(const std::filesystem::path &from, const std::filesystem::path &to);

        std::filesystem::path m_path_to_real_dir;
        std::filesystem::path m_path_to_backup;
    };
}
