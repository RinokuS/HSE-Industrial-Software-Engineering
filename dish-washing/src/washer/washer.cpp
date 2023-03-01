// dish-washing
#include <washer.hpp>
#include <consts.hpp>
// C++ STL
#include <iostream>
// fmt
#include <fmt/format.h>

namespace dish_washing {
    washer::washer(char *washer_file_path, char *dirty_dishes_file_path, std::int32_t *table, bool *table_access)
            : m_table(table), m_table_access(table_access) {
        if ((m_dirty_dishes = fopen(dirty_dishes_file_path, "r")) == nullptr) {
            std::cerr << "Error: can't open dirty dishes file.";
            exit(-1);
        }
        if ((m_washer_process = fopen(washer_file_path, "r")) == nullptr) {
            std::cerr << "Error: can't open washer process file.";
            exit(-1);
        }

        char washer_string[dish_washing::max_config_string_size];
        std::int32_t dish_type, washing_time;

        while (fgets(washer_string, sizeof(washer_string), m_washer_process) != nullptr) {
            sscanf(washer_string, "%d:%d\n", &dish_type, &washing_time);
            m_type_to_wash_time[dish_type] = washing_time;
        }
    }

    washer::~washer() {
        fclose(m_dirty_dishes);
        fclose(m_washer_process);
    }

    void washer::wash() {
        char dishes_string[dish_washing::max_config_string_size];
        std::int32_t dish_type, dish_quantity;
        std::int32_t table_index = 0;

        while (fgets(dishes_string, sizeof(dishes_string), m_dirty_dishes) != nullptr) {
            sscanf(dishes_string, "%d:%d\n", &dish_type, &dish_quantity);

            while (dish_quantity != 0) {
                while (!m_table_access[table_index]) {
                    // wait for place to free
                    std::cout << "Washer: waiting..." << std::endl;
                    sleep(1);
                }

                std::cout << "Washer: washing..." << std::endl;
                sleep(m_type_to_wash_time[dish_type]);
                std::cout
                        << fmt::format("Washer: washing of the dish {} completed. Putting it to position {} on table.",
                                       dish_type, table_index) << std::endl;
                m_table[table_index] = dish_type;
                m_table_access[table_index] = false;
                table_index = (table_index + 1) % dish_washing::TABLE_LIMIT;
                dish_quantity--;
            }
        }
        // simple flag to stop wiper process
        m_table[table_index] = -1;
    }
}
