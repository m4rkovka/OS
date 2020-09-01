#ifndef HEAPSORT_H
#define HEAPSORT_H 1

#include <utility>

// Восстановление свойств кучи.
template<typename T>
void Heapify(T *A, int i, int size) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;

    if (left < size) {
        if (A[left] > A[largest]) {
            largest = left;
        }
    }
    if (right < size) {
        if (A[right] > A[largest]) {
            largest = right;
        }
    }
    if (largest != i) {
        std::swap(A[i], A[largest]);
        Heapify(A, largest, size);
    }
}

// Построение кучи из массива
template<typename T>
void Build_Heap(T *A, int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        Heapify(A, i, size);
    }
}

// Пирамидальная сортировка
template<typename T>
void heapsort(T *A, int size) {
    Build_Heap(A, size);
    for (auto i = size - 1; i >= 0; i--) {
        std::swap(A[0], A[i]);
        Heapify(A, 0, i);
    }
}

#endif //HEAPSORT_H