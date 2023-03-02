#pragma once

#include <cstdint>
#include <vector>
#include <pthread.h>
#include <iostream>
#include <pthread.h>
#include <cassert>

namespace merge_sort {
    template<typename T>
    struct thread_data {
        std::int32_t left, right;
        std::vector<T>* elements_ptr;
    };

    template<typename T>
    class thread_sorter {
    public:
        thread_sorter(const std::vector<T> &elements_to_sort) : m_elements(elements_to_sort) {}

        void sort() {
            thread_data<T> data = {0, static_cast<std::int32_t>(m_elements.size() - 1), &m_elements};
            threaded_merge_sort(reinterpret_cast<void*>(&data));

            // testing array to prove efficiency
            for (int i = 0; i < m_elements.size() - 1; ++i) {
                assert(m_elements[i] < m_elements[i + 1]);
            }
            for (const auto& elem: m_elements) {
                std::cout << elem << ' ';
            }
            std::cout << std::endl;
        }

    private:
        // simple merge function from basic merge_sort
        static void merge(std::int32_t left, std::int32_t middle, std::int32_t right, std::vector<T>* elements) {
            std::int32_t i, j, k = 0;
            std::int32_t left_length = middle - left + 1;
            std::int32_t right_length = right - middle;

            T left_array[left_length];
            T right_array[right_length];

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

        // our multithreading merge-sort
        static void *threaded_merge_sort(void *args) {
            auto data = reinterpret_cast<thread_data<T>*>(args);
            std::int32_t left = data->left, right = data->right;
            std::vector<T>* elements = data->elements_ptr;
            if (right <= left) {
                return nullptr; // one element
            }

            pthread_t left_thread, right_thread;
            thread_data<T> data1, data2;
            std::int32_t mid = (left + right) / 2;
            assign_data_to_thread(left_thread, data1, left, mid, elements);
            assign_data_to_thread(right_thread, data2, mid + 1, right,  elements);

            pthread_join(left_thread, nullptr);
            pthread_join(right_thread, nullptr);
            merge(left, mid, right, elements);

            return nullptr;
        }

        static void assign_data_to_thread(pthread_t &thread, thread_data<T> &data, std::int32_t left,
                                   std::int32_t right, std::vector<T> *elements) {
            data.left = left;
            data.right = right;
            data.elements_ptr = elements;

            pthread_create(&thread, nullptr,threaded_merge_sort,reinterpret_cast<void*>(&data));
        }

        std::vector<T> m_elements;
    };
}
