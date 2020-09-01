#ifndef TIMER_HPP
#define TIMER_HPP 1

#include <chrono>
#include <iostream>

class timer {
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point end;
public: 
    timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    ~timer() {
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Time " << duration.count() << " s\n"; 
    }
};

#endif //TIMER_HPP