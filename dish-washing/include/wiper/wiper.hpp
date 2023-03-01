#pragma once

#include <filesystem>
#include <unordered_map>

namespace dish_washing {
    class wiper {
    public:
        wiper(char *wiper_file_path, std::int32_t *table, bool *table_access);
        ~wiper();

        void wipe();

    private:
        FILE *m_wiper_process;
        std::int32_t *m_table;
        bool *m_table_access;
        std::unordered_map<std::int32_t, std::int32_t> m_type_to_wipe_time;
    };
}
