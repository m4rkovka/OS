#ifndef BINARY_SEARCH
#define BINARY_SEARCH 1

template<typename T>
bool binary_search_help(T key, T *a, int left, int right) {
    for (;;) {
        int mid = (right + left) / 2;
        if (key < a[mid]) {
            right = mid - 1;
        } else if (key > a[mid]) {
            left = mid + 1;
        } else {
            return true;
        }
        if (left > right) {
            return false;
        }
    }
}

template<typename T>
bool binary_search(T key, T *a, int size) {
    return binary_search_help(key, a, 0, size - 1);
}

#endif //BINARY_SEARCH