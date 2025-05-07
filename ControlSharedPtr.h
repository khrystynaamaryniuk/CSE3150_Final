#ifndef CONTROL_SHARED_PTR_H
#define CONTROL_SHARED_PTR_H

#include <iostream>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<Clock> clockTime;
typedef std::chrono::milliseconds milliSeconds;

template<typename T>
struct ControlSharedPtr {
    T* ptr;
    long myUseCount;
    clockTime startTime;
    milliSeconds deleteInMs;

    ControlSharedPtr(T* p, long deleteMs);
    ~ControlSharedPtr();

    void resetTimer();
};

template<typename T>
ControlSharedPtr<T>::ControlSharedPtr(T* p, long deleteMs)
    : ptr(p),
      myUseCount(1),
      startTime(Clock::now()),
      deleteInMs(std::chrono::milliseconds(deleteMs)) {
    std::cout << "ControlSharedPtr " << static_cast<void*>(ptr)
              << " start at: 0 ms" << std::endl;
}

template<typename T>

ControlSharedPtr<T>::~ControlSharedPtr() {
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<milliSeconds>(end - startTime);
    std::cout << "ControlSharedPtr " << static_cast<void*>(ptr)
              << " end at: "
              << diff.count()
              << " ms" << std::endl;
    delete ptr; // SAFELY deletes the actual object
}

template<typename T>
void ControlSharedPtr<T>::resetTimer() {
    startTime = Clock::now();
    std::cout << "Timer reset to 0 ms" << std::endl;
}

#endif // CONTROL_SHARED_PTR_H
