#pragma once

#include <filesystem>
#include <unordered_map>

namespace dish_washing {
    class washer {
    public:
        washer(char *washer_file_path, char *dirty_dishes_file_path, std::int32_t *table, bool *table_access);

        ~washer();

        void wash();

    private:
        FILE *m_dirty_dishes;
        FILE *m_washer_process;
        std::int32_t *m_table;
        bool *m_table_access;
        std::unordered_map<std::int32_t, std::int32_t> m_type_to_wash_time;
    };
}
