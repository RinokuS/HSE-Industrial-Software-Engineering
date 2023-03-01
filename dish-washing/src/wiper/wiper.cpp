// dish-washing
#include <wiper.hpp>
#include <consts.hpp>
// C++ STL
#include <iostream>
// fmt
#include <fmt/format.h>

namespace dish_washing {
    wiper::wiper(char *wiper_file_path, std::int32_t *table, bool *table_access) : m_table(table),
                                                                                   m_table_access(table_access) {
        if ((m_wiper_process = fopen(wiper_file_path, "r")) == nullptr) {
            std::cerr << "Error: can't open wiper process file.";
            exit(-1);
        }

        char wiper_string[dish_washing::max_config_string_size];
        std::int32_t dish_type, wiping_time;
        while (fgets(wiper_string, sizeof(wiper_string), m_wiper_process) != nullptr) {
            sscanf(wiper_string, "%d:%d\n", &dish_type, &wiping_time);
            m_type_to_wipe_time[dish_type] = wiping_time;
        }
    }

    wiper::~wiper() {
        fclose(m_wiper_process);
    }

    void wiper::wipe() {
        std::int32_t table_index = 0;

        while (true) {
            while (m_table_access[table_index]) {
                // wait for place to free
                std::cout << "Wiper: waiting..." << std::endl;
                sleep(1);

                if (m_table[table_index] == -1) {
                    return;
                }
            }

            std::cout << fmt::format("Wiper: wiping dish {} from position {} on table",
                                     m_table[table_index], table_index) << std::endl;
            sleep(m_type_to_wipe_time[m_table[table_index]]);
            std::cout << "Wiper: wiping of the dish completed." << std::endl;
            m_table_access[table_index] = true;
            table_index = (table_index + 1) % dish_washing::TABLE_LIMIT;
        }
    }
}
