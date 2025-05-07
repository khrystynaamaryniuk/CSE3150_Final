#ifndef TIMED_SHARED_PTR_H
#define TIMED_SHARED_PTR_H

#include "ControlSharedPtr.h"
#include <iostream>

template<typename T>
class TimedSharedPtr {
private:
    ControlSharedPtr<T>* control;

public:
    TimedSharedPtr();
    TimedSharedPtr(T* raw, long deleteMs);
    TimedSharedPtr(T* raw);
    TimedSharedPtr(const TimedSharedPtr<T>& other);
    ~TimedSharedPtr();

    TimedSharedPtr<T>& operator=(const TimedSharedPtr<T>& other);

    long use_count() const noexcept;
    T* get() const;
    void resetTimer();
    T& operator*() const;
    T* operator->() const;
};

// Implementations

template<typename T>
TimedSharedPtr<T>::TimedSharedPtr() : control(nullptr) {}

template<typename T>
TimedSharedPtr<T>::TimedSharedPtr(T* raw, long deleteMs)
    : control(new ControlSharedPtr<T>(raw, deleteMs)) {}

template<typename T>
TimedSharedPtr<T>::TimedSharedPtr(T* raw)
    : control(new ControlSharedPtr<T>(raw, 1000)) {}

template<typename T>
TimedSharedPtr<T>::TimedSharedPtr(const TimedSharedPtr<T>& other) : control(other.control) {
    if (control) control->myUseCount++;
}

template<typename T>
TimedSharedPtr<T>::~TimedSharedPtr() {
    if (control && --control->myUseCount == 0)

        delete control;
}

template<typename T>
TimedSharedPtr<T>& TimedSharedPtr<T>::operator=(const TimedSharedPtr<T>& other) {
    if (this != &other) {
        // Decrease old reference count and delete if necessary
        if (control && --control->myUseCount == 0)
            delete control;
        
        // Copy the new control
        control = other.control;
        
        // Increment use count of the new control
        if (control)
            control->myUseCount++;
    }
    return *this;
}

template<typename T>
long TimedSharedPtr<T>::use_count() const noexcept {
    return control ? control->myUseCount : 0;
}

template<typename T>
T* TimedSharedPtr<T>::get() const {
        auto milliseondsSinceStart = std::chrono::duration_cast<milliSeconds>(Clock::now() - control->startTime);
        if (milliseondsSinceStart > control->deleteInMs ) {
            std::cout << "Yeo! Expired " ;
            return nullptr;
        } else {
            return control ? static_cast<T*>(control->ptr) : nullptr;
        }
    }

template<typename T>
void TimedSharedPtr<T>::resetTimer() {
    if (control) control->resetTimer();
}

template<typename T>
T& TimedSharedPtr<T>::operator*() const {
    return *get();
}

template<typename T>
T* TimedSharedPtr<T>::operator->() const {
    return get();
}

#endif // TIMED_SHARED_PTR_H
