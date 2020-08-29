#include <atomic>
#include <thread>
#include <iostream>
#include "lock_free_queue.hpp"
#include "timer.hpp"

constexpr unsigned CNT_PUSH = 10000;
constexpr unsigned CNT_POP = 9999;

int main() {
    lockFree::Queue<int> q;
    std::thread tArr[lockFree::P];
    {
        timer t;
        for (int i = 0; i < lockFree::P; i++) {
            if (i % 2 == 0) {
                tArr[i] = std::thread([&q, i](){
                    for (int j = 0; j < CNT_PUSH; j++) {
                        q.enqueue(j, i);
                    }
                });
            } else {
                tArr[i] = std::thread([&q, i](){
                    for (int j = 0; j < CNT_POP; j++) {
                        int x;
                        q.dequeue(x, i);
                    }
                });
            }
        }
        for (int i = 0; i < lockFree::P; i++) {
            tArr[i].join();
        }
    }
    return 0;
}