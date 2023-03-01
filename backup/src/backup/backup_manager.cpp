// backup
#include <backup_manager.hpp>
// C++ STL
#include <iostream>
#include <sys/stat.h>
// fmt
#include <fmt/format.h>

namespace backup {
    backup_manager::backup_manager(const std::filesystem::path &path_to_real_dir,
                                   const std::filesystem::path &path_to_backup)
            : m_path_to_real_dir(path_to_real_dir)
            , m_path_to_backup(path_to_backup) {
        if (!std::filesystem::is_directory(m_path_to_backup)) {
            throw std::runtime_error("Target directory does not exist");
        }
        if (!std::filesystem::is_directory(m_path_to_backup)) {
            mkdir(m_path_to_backup.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
    }

    bool backup_manager::should_backup(const std::filesystem::path &from, const std::filesystem::path &to) {
        if (!std::filesystem::exists(to)) {
            return true;
        }

        if (std::filesystem::last_write_time(from) > std::filesystem::last_write_time(to)) {
            system(fmt::format("rm {}", to.c_str()).c_str());
            return true;
        }

        return false;
    }

    void backup_manager::copy(const std::filesystem::path &from, const std::filesystem::path &to) {
        auto to_gz = fmt::format("{}.gz", to.c_str());
        if (!should_backup(from, to_gz)) {
            return;
        }

        std::cout << fmt::format("Backuping file '{}'...", from.c_str()) << std::endl;
        system(fmt::format("cp {} {}", from.c_str(), to.c_str()).c_str());

        system(fmt::format("gzip {}", to.c_str()).c_str());

        std::cout << fmt::format("Backup of the file '{}' created successfully.", from.c_str()) << std::endl;
    }

    void backup_manager::backup() {
        backup(m_path_to_real_dir, m_path_to_backup);
    }

    void backup_manager::backup(const std::filesystem::path &from, const std::filesystem::path &to) {
        std::cout << fmt::format("Started backup process of the directory '{}'.", from.c_str()) << std::endl;
        // removing backups of deleted files
        for (const auto& entry: std::filesystem::directory_iterator(to)) {
            std::filesystem::path file_path = entry.path();
            auto file_name = file_path.filename().string();
            std::filesystem::path target_path;

            if (file_name == "." || file_name == "..") {
                continue;
            }

            if (std::filesystem::is_directory(file_path)) {
                target_path = fmt::format("{}/{}", from.c_str(), file_name);
            } else if (std::filesystem::is_regular_file(file_path)) { // removing .gz from file name
                target_path = fmt::format("{}/{}", from.c_str(), file_name.substr(0, file_name.size() - 3));
            }

            if (!std::filesystem::exists(target_path)) { // if target file does not exist - remove its copy
                system(fmt::format("rm -r {}", file_path.c_str()).c_str());
            }
        }

        // backuping target directory
        for (const auto& entry: std::filesystem::directory_iterator(from)) {
            std::filesystem::path file_path = entry.path();
            auto file_name = file_path.filename();
            std::filesystem::path copy_path = fmt::format("{}/{}", to.c_str(), file_name.c_str());

            if (file_name == "." || file_name == "..") {
                continue;
            }

            if (std::filesystem::is_directory(file_path)) {
                if (!std::filesystem::is_directory(copy_path)) {
                    mkdir(copy_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                }

                backup(file_path, copy_path);
            } else if (std::filesystem::is_regular_file(file_path)) {
                copy(file_path, copy_path);
            }
        }
    }
}
