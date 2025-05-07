#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "ControlSharedPtr.h"
#include "TimedSharedPtr.h"
#include <chrono>
#include <thread>

using namespace std::chrono;

TEST_CASE("ControlSharedPtr basic functionality") {
    int* p = new int(10);
    ControlSharedPtr<int> ptr(p, 500);  

    auto start = Clock::now();
    CHECK(ptr.ptr != nullptr);
    
    std::this_thread::sleep_for(milliseconds(200));  
   
    ptr.resetTimer();
    auto diffAfterReset = std::chrono::duration_cast<milliSeconds>(Clock::now() - start);
    CHECK(diffAfterReset.count() < 500);  
}


TEST_CASE("TimedSharedPtr basic functionality") {
    int* p = new int(10);
    TimedSharedPtr<int> timedPtr(p, 1000);  

   
    CHECK(timedPtr.use_count() == 1); 
    CHECK(*timedPtr == 10); 
    
    std::this_thread::sleep_for(milliseconds(500));  
    

    CHECK(*timedPtr == 10);
    
    std::this_thread::sleep_for(milliseconds(600));  
    
    // After expiration - the pointer should return null
    CHECK(timedPtr.get() == nullptr);
}


TEST_CASE("TimedSharedPtr expiration and resetTimer") {
    int* p = new int(20);
    TimedSharedPtr<int> timedPtr(p, 1000); 

    std::this_thread::sleep_for(milliseconds(500));  

    timedPtr.resetTimer();
    
    std::this_thread::sleep_for(milliseconds(600));  

    // After reset, pointer should still be valid
    CHECK(*timedPtr == 20);
}


TEST_CASE("TimedSharedPtr copy constructor and assignment operator") {
    int* p = new int(30);
    TimedSharedPtr<int> original(p, 1000);  


    TimedSharedPtr<int> copy = original;
    CHECK(copy.use_count() == 2);  

    
    TimedSharedPtr<int> assigned;
    assigned = original;
    CHECK(assigned.use_count() == 3); 

    
    std::this_thread::sleep_for(milliseconds(1200));
    CHECK(copy.get() == nullptr);
    CHECK(assigned.get() == nullptr);
}


TEST_CASE("TimedSharedPtr memory destruction") {
    int* p = new int(50);
    {
        TimedSharedPtr<int> timedPtr(p, 500);  
        CHECK(timedPtr.use_count() == 1);
    } 


    CHECK(p != nullptr);  
}
