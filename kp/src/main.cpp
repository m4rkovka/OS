#include <atomic>
#include <thread>
#include <iostream>
#include <queue>
#include <list>
#include <deque>
#include "lock_free_queue.hpp"
#include "lock_queue.hpp"
#include "timer.hpp"

constexpr unsigned CNT_PUSH = 10000000;
constexpr unsigned CNT_POP = 2500000;

int main() {
    lockFree::Queue<int> q;
    std::thread tArr[lockFree::P];
    {
        std::cout << "LOCK FREE QUEUE "; 
        timer t;
        for (int i = 0; i < lockFree::P; i++) {
            if (i % 2 == 0) {
                tArr[i] = std::thread([&q, i](){
                    for (int j = 0; j < CNT_PUSH / lockFree::P * 2; j++) {
                        q.enqueue(j, i);
                    }
                });
            } else {
                tArr[i] = std::thread([&q, i](){
                    for (int j = 0; j < CNT_POP / lockFree::P * 2; j++) {
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
    lock::Queue<int, std::deque<int>> lqd;
    {
        std::cout << "LOCK QUEUE DEQUE "; 
        timer t;
        for (int i = 0; i < lockFree::P; i++) {
            if (i % 2 == 0) {
                tArr[i] = std::thread([&lqd](){
                    for (int j = 0; j < CNT_PUSH / lockFree::P * 2; j++) {
                        lqd.push(j);
                    }
                });
            } else {
                tArr[i] = std::thread([&lqd](){
                    for (int j = 0; j < CNT_POP / lockFree::P * 2; j++) {
                        int x;
                        lqd.pop(x);
                    }
                });
            }
        }
        for (int i = 0; i < lockFree::P; i++) {
            tArr[i].join();
        }
    }
    lock::Queue<int, std::list<int>> lql;
    {
        std::cout << "LOCK QUEUE LIST "; 
        timer t;
        for (int i = 0; i < lockFree::P; i++) {
            if (i % 2 == 0) {
                tArr[i] = std::thread([&lql](){
                    for (int j = 0; j < CNT_PUSH / lockFree::P * 2; j++) {
                        lql.push(j);
                    }
                });
            } else {
                tArr[i] = std::thread([&lql](){
                    for (int j = 0; j < CNT_POP / lockFree::P * 2; j++) {
                        int x;
                        lql.pop(x);
                    }
                });
            }
        }
        for (int i = 0; i < lockFree::P; i++) {
            tArr[i].join();
        }
    }
    std::queue<int, std::list<int>> sql;
    {
        std::cout << "STD QUEUE LIST "; 
        timer t;
        for (int i = 0; i < lockFree::P; i++) {
            if (i % 2 == 0) {
                for (int j = 0; j < CNT_PUSH; j++) {
                    sql.push(j);
                }
            } else {
                for (int j = 0; j < CNT_POP; j++) {
                    sql.pop();
                }
            }
        }
    }
    std::queue<int, std::deque<int>> sqd;
    {
        std::cout << "STD QUEUE DEQUE "; 
        timer t;
        for (int i = 0; i < lockFree::P; i++) {
            if (i % 2 == 0) {
                for (int j = 0; j < CNT_PUSH; j++) {
                    sqd.push(j);
                }
            } else {
                for (int j = 0; j < CNT_POP; j++) {
                    sqd.pop();
                }
            }
        }
    }
    return 0;
}