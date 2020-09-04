#ifndef LOCK_FREE_QUEUE_HPP
#define LOCK_FREE_QUEUE_HPP 1
#include <atomic>
#include <iostream>
#include "sort.hpp"
#include "binary_search.hpp"

namespace lockFree {
    // Число hazard pointer в одном потоке
    constexpr unsigned K = 2;
    // Число потоков
    constexpr unsigned P = 12;
    // Общее число hazard pointers = K * P
    constexpr unsigned N = K * P;
    // batch size
    constexpr unsigned R = 2 * N;

    class exp_backoff {
        const int nInitial;
        const int nStep;
        const int nThreshold;
        int nCurrent;
    public:
        exp_backoff(int init = 10, int step = 2, int threshold = 8000)
            : nInitial(init), nStep(step), nThreshold(threshold) {}
        void operator()() {
            for (int k = 0; k < nCurrent; k++) {
                asm volatile ("nop"::);
            }
            nCurrent *= nStep;
            if (nCurrent > nThreshold) {
                nCurrent = nThreshold;
            }
        }
        void reset() {
            nCurrent = nInitial;
        }
    };

    template<typename T>
    class Queue {
        struct node {
            std::atomic<node *> next;
            T data;

            node() : next(nullptr) {}
            node(const T &val) : next(nullptr), data(val) {} 
        };

        std::atomic<node *> m_Head;
        std::atomic<node *> m_Tail;

        // массив hazard pointers
        node *HP[P][K];
        // текущий размер dlist
        unsigned dcount[P];
        // массив готовых к удалению данных
        node *dlist[P][R];

        // Удаление данных. Помещает данные в массив dlist
        void RetireNode(node *n, int tNum) {
            dlist[tNum][dcount[tNum]] = n;
            dcount[tNum]++;
            // Если массив заполнен - вызываем основную функцию Scan
            if (dcount[tNum] == R) {
                Scan(tNum);
            }
        }

        // Удаляет все элементы массива dlist, которые
        // не объявлены как hazard pointer
        void Scan(int tNum) {
            unsigned i;
            unsigned p = 0;
            unsigned new_dcount = 0; // 0..N
            node *hptr;
            node *plist[N];
            node *new_dlist[N];

            // Инициализация plist
            for (int i = 0; i < N; i++) {
                plist[i] = nullptr;
            }
            // Инициализация new_dlist 
            for (int j = 0; j < N; j++) {
                new_dlist[j] = nullptr;
            }

            // Стадия 1. Проходим по HP и собираем общий массив plist защищенных указателей.
            for (unsigned t = 0; t < P; ++t) {
                node **pHPThread = HP[t];
                for (i = 0; i < K; i++) {
                    hptr = pHPThread[i];
                    if (hptr != nullptr) {
                        plist[p++] = hptr;
                    }
                }
            }

            // Стадия 2. Сортировка hazard pointers. Это необходимо для последующего бинарного поиска.
            heapsort(plist, N);

            // Стадия 3. Удаление элементов, не объявленных как hazard pointer.
            for (i = 0; i < R; i++) {
                // Если dlist[tNum][i] отсутствует в списке plist, то dlist[tNum][i] должен быть удален
                if (binary_search(dlist[tNum][i], plist, N)) {
                    new_dlist[new_dcount] = dlist[tNum][i];
                    new_dcount++;
                } else {
                    delete dlist[tNum][i];
                    dlist[tNum][i] = nullptr;
                }
            }

            for (int u = 0; u < R; u++) {
                dlist[tNum][u] = nullptr;
            }

            // Стадия 4. Формирование новго массива отложенных элементов.
            for (i = 0; i < new_dcount; i++) {
                dlist[tNum][i] = new_dlist[i];
            }
            dcount[tNum] = new_dcount;
        }

    public:
        Queue() {
            node *p = new node();
            m_Head.store(p, std::memory_order_release);
            m_Tail.store(p, std::memory_order_release);

            // Инициализация HP
            for (int i = 0; i < P; i++) {
                for (int j = 0; j < K; j++) {
                    HP[i][j] = nullptr;
                }
            }
            // Инициализация dcount
            for (int i = 0; i < P; i++) {
                dcount[i] = 0;
            }
            // Инициализация dlist 
            for (int i = 0; i < P; i++) {
                for (int j = 0; j < R; j++) {
                    dlist[i][j] = nullptr;
                }
            }
        }

        ~Queue() {
            auto it = m_Head.load();
            while (it != nullptr) {
                auto hIt = it->next.load();
                delete it;
                it = hIt;
            }
            for (int nThread = 0; nThread < P; nThread++) {
                for (int i = 0; i < R; i++) {
                    if (dlist[nThread][i] != nullptr) {
                        delete dlist[nThread][i];
                    }
                }
            }
        }

        void enqueue(const T& data, int tNum) {
            node *pNew = new node(data);
            exp_backoff bkoff{};
            node *t;
            for(;;) {
                t = m_Tail.load(std::memory_order_relaxed);
                HP[tNum][0] = t;
                if (t != m_Tail.load(std::memory_order_acquire)) continue;

                node *next = t->next.load(std::memory_order_acquire);
                if (t != m_Tail.load(std::memory_order_acquire)) continue;
                if (next != nullptr) {
                    m_Tail.compare_exchange_weak(t, next, std::memory_order_release);
                    continue;
                }
                node *tmp = nullptr;
                if (t->next.compare_exchange_weak(tmp, pNew, std::memory_order_release)) {
                    break;
                }
                bkoff();
            }
            m_Tail.compare_exchange_strong(t, pNew, std::memory_order_acq_rel);
            HP[tNum][0] = nullptr;
        }

        bool dequeue(T &dest, int tNum) {
            exp_backoff bkoff{};
            node *h;
            for (;;) {
                h = m_Head.load(std::memory_order_relaxed);
                HP[tNum][0] = h;
                if (h != m_Head.load(std::memory_order_acquire)) continue;
                node *t = m_Tail.load(std::memory_order_relaxed);
                node *next = h->next.load(std::memory_order_acquire);
                HP[tNum][1] = next;
                if (h != m_Head.load(std::memory_order_relaxed))
                    continue;
                if (next == nullptr) { // очередь пуста
                    HP[tNum][0] = nullptr;
                    HP[tNum][1] = nullptr;
                    return false;
                }
                if (h == t) { 
                    m_Tail.compare_exchange_strong(t, next, std::memory_order_release);
                    continue;
                }
                dest = next->data;
                if (m_Head.compare_exchange_strong(h, next, std::memory_order_release))
                    break;
                bkoff();
            }
            HP[tNum][0] = nullptr;
            HP[tNum][1] = nullptr;
            RetireNode(h, tNum);
            return true;
        }

        void print() {
            node *it = m_Head.load();
            it = it->next;
            while (it != nullptr) {
                std::cout << it->data << " ";
                it = it->next;
            }
            std::cout << "\n";
        }
    };
}

#endif //LOCK_FREE_QUEUE_HPP