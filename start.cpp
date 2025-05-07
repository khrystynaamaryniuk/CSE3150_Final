#include "TimedSharedPtr.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

typedef std::chrono::high_resolution_clock Clock;

struct Node {
    int val;
    TimedSharedPtr<Node> next;
    Node(int v) : val(v) {}
};
int main() {
    int nodeValue;
    long expiryMs;

    // First Example: Basic Usage without Resetting the Timer
    cout << "First Example: Basic Usage without Resetting the Timer\n";
    cout << "--------------------------------------------------------\n";
    cout << "Enter value to store in Node: ";
    cin >> nodeValue;

    cout << "Enter expiration time in milliseconds: ";
    cin >> expiryMs;

    TimedSharedPtr<Node> myNode(new Node(nodeValue), expiryMs);
    cout<< "Copying a copy of myNode -  myOtherNode"<< endl;
    TimedSharedPtr<Node> myOtherNode = myNode;  // Copy constructor called here!

    // Wait 50ms
    this_thread::sleep_for(chrono::milliseconds(50));
    cout << "\n[After 50ms]\n";
    cout << "myNode.get() address: <" << myNode.get() << ">\n";
    cout << "myNode.use_count(): " << myNode.use_count() << endl;
    cout << "myOtherNode.use_count(): " << myOtherNode.use_count() << endl;

    // Wait 25ms more
    this_thread::sleep_for(chrono::milliseconds(25));
    cout << "\n[After 75ms total]\n";
    cout << "myNode.get() address: <" << myNode.get() << ">\n";

    // Wait 75ms more (now should be expired)
    this_thread::sleep_for(chrono::milliseconds(75));
    cout << "\n[After 150ms total]\n";
    cout << "myNode.get() adress: "<<myNode.get()<< endl;
    cout << "myOtherNode.get() adress: "<<myOtherNode.get()<< endl;
    // Second Example: Default Expiration Time of 1000ms
    cout << "\n-----------\n";
    cout << "Second Example: Default Expiration Time of 1000ms\n";
    cout << "---------------------------------------------------\n";

    // Use default expiration time of 1000ms
    TimedSharedPtr<int> p(new int(42));
    cout << "p.get(): " << p.get() << endl;
    cout << "p.use_count(): " << p.use_count() << endl;

    TimedSharedPtr<int> q = p;
    cout << "p.use_count(): " << p.use_count() << endl;
    cout << "q.use_count(): " << q.use_count() << endl;

    // Wait 500ms (half of 1000ms expiration time)
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "\n[After 500ms]\n";
    cout << "p.get() adress: " << p.get() << endl;
    cout << "q.get() adress: " << q.get() << endl;
    // Wait another 600ms (over 1000ms expiration)
    this_thread::sleep_for(chrono::milliseconds(600));
    cout << "\n[After 1100ms]\n";
    cout << "p.get() adress: " << p.get() << endl;
    cout << "q.get() adress: " << q.get() << endl;

    // Third Example: Resetting the Timer
    cout << "\n-----------\n";
    cout << "Third Example: Resetting the Timer\n";
    cout << "-----------------------------------\n";

    TimedSharedPtr<Node> myNodeWithReset(new Node(100), 1000); // 1000ms expiration time

    cout << "Created myNodeWithReset with value 100. It will expire in 1000 milliseconds.\n";

    this_thread::sleep_for(chrono::milliseconds(300)); // Sleep for 300ms
    cout << "\n[After 300ms]\n";
    cout << "myNodeWithReset.get() address: <" << myNodeWithReset.get() << ">\n";

    cout << "\n[Resetting timer on myNodeWithReset]\n";
    myNodeWithReset.resetTimer();

    this_thread::sleep_for(chrono::milliseconds(800)); // Sleep for remaining 800ms
    cout << "\n[After 1100ms total (800ms after reset)]\n";
    cout << "myNodeWithReset.get() address: <" << myNodeWithReset.get() << ">\n";

    this_thread::sleep_for(chrono::milliseconds(200)); // Sleep 200ms (total 1300ms since creation)
    cout << "\n[After 1300ms total (200ms since reset)]\n";
    cout << "myNodeWithReset.get() address: <" << myNodeWithReset.get() << ">\n";

    return 0;
}