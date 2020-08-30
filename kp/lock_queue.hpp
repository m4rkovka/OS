#ifndef LOCK_QUEUE_HPP
#define LOCK_QUEUE_HPP 1

#include <mutex>
#include <queue>
#include <list>
#include <deque>

namespace lock {

    template<typename T, typename Container = std::list<T>>
    class Queue {
        std::queue<T, Container> q;
        std::mutex m;
    public:
        bool pop(T &dest) {
            bool retV = false;
            std::lock_guard<std::mutex> lg(m);
            if (!q.empty()) {
                dest = q.front();
                q.pop();
                retV = true;
            }
            return retV;
        }

        void push(const T& data) {
            std::lock_guard<std::mutex> lg(m);
            q.push(data);
        }
    };
}

#endif // LOCK_QUEUE_HPP