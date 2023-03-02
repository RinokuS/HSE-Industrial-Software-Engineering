#pragma once

#include <cstdint>
#include <vector>
#include <pthread.h>
#include <iostream>
#include <cmath>
#include <pthread.h>

namespace merge_sort {
    struct thread_data {
        std::int32_t left, right, level;
        std::vector<std::int32_t>* elements_ptr;
    };

    class thread_sorter {
    public:
        thread_sorter(const std::vector<std::int32_t> &elements_to_sort) : m_elements(elements_to_sort) {}

        void sort() {
            thread_data data = {0, static_cast<std::int32_t>(m_elements.size() - 1), static_cast<std::int32_t>(ceil(log2(m_elements.size()))), &m_elements};
            threaded_merge_sort(reinterpret_cast<void*>(&data));

            for (const auto& elem: m_elements) {
                std::cout << elem << ' ';
            }
            std::cout << std::endl;
        }

    private:
        static void merge(std::int32_t left, std::int32_t middle, std::int32_t right, std::vector<std::int32_t>* elements) {
            std::int32_t i, j, k = 0;
            std::int32_t left_length = middle - left + 1;
            std::int32_t right_length = right - middle;

            std::int32_t left_array[left_length];
            std::int32_t right_array[right_length];

            /* copy values to left array */
            for (i = 0; i < left_length; i ++) {
                left_array[i] = elements->at(left + i);
            }

            /* copy values to right array */
            for (j = 0; j < right_length; j ++) {
                right_array[j] = elements->at(middle + 1 + j);
            }

            i = 0;
            j = 0;
            /** chose from right and left arrays and copy */
            while (i < left_length && j < right_length) {
                if (left_array[i] <= right_array[j]) {
                    (*elements)[left + k] = left_array[i];
                    i ++;
                } else {
                    (*elements)[left + k] = right_array[j];
                    j ++;
                }
                k ++;
            }

            /* copy the remaining values to the array */
            while (i < left_length) {
                (*elements)[left + k] = left_array[i];
                k ++;
                i ++;
            }
            while (j < right_length) {
                (*elements)[left + k] = right_array[j];
                k ++;
                j ++;
            }
        }

        static void merge_sort(std::int32_t left, std::int32_t right, std::vector<std::int32_t>* elements) {
            if (left >= right) {
                return;
            }

            std::int32_t mid = left + (right - left) / 2;
            merge_sort(left, mid, elements);
            merge_sort(mid + 1, right, elements);

            merge(left, mid, right, elements);
        }

        static void *threaded_merge_sort(void *args) {
            auto data = reinterpret_cast<thread_data*>(args);
            std::int32_t left = data->left, right = data->right, level = data->level;
            std::vector<std::int32_t>* elements = data->elements_ptr;
            if (right <= left) {
                return nullptr; //one element
            }
            if (level == 0) {
                merge_sort(left, right, elements);
                return nullptr;
            }
            pthread_t left_thread, right_thread;
            thread_data data1, data2;
            std::int32_t mid = (left + right) / 2;
            assign_data_to_thread(left_thread, data1, left, mid, level - 1);
            assign_data_to_thread(right_thread, data2, mid + 1, right, level - 1);

            pthread_join(left_thread, nullptr);
            pthread_join(right_thread, nullptr);
            merge(left, mid, right, elements);
        }

        static void assign_data_to_thread(pthread_t &thread, thread_data &data, std::int32_t left,
                                   std::int32_t right, std::int32_t level) {
            data.left = left;
            data.right = right;
            data.level = level;

            pthread_create(&thread, nullptr,threaded_merge_sort,reinterpret_cast<void*>(&data));
        }

        std::vector<std::int32_t> m_elements;
    };
}
