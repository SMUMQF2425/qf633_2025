#include <iostream>
#include <thread> // For std::thread
#include <mutex>  // For std::mutex and std::lock_guard
#include <chrono> // For std::chrono::milliseconds
#include <string> // For std::string

// Two shared resources (represented by locks)
std::mutex g_mutex1;
std::mutex g_mutex2;

// Function for Thread 1
void thread_func1()
{
    std::cout << "Thread 1: Trying to lock mutex1...\n";
    std::lock_guard<std::mutex> lock1(g_mutex1); // Acquire lock1
    std::cout << "Thread 1: Mutex1 locked. Waiting a bit...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work

    std::cout << "Thread 1: Trying to lock mutex2...\n";
    std::lock_guard<std::mutex> lock2(g_mutex2); // Try to acquire lock2
    std::cout << "Thread 1: Mutex2 locked. Both locks acquired.\n";

    // Critical section - will not be reached in a deadlock
    std::cout << "Thread 1: Doing work with both locks...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Thread 1: Releasing both locks.\n";
    // lock2 and lock1 are automatically released when they go out of scope
}

// Function for Thread 2
void thread_func2()
{
    std::cout << "Thread 2: Trying to lock mutex1...\n";
    std::lock_guard<std::mutex> lock1(g_mutex1); // Try to acquire lock1
    std::cout << "Thread 2: Mutex1 locked. Both locks acquired.\n";

    std::cout << "Thread 2: Trying to lock mutex2...\n";
    std::lock_guard<std::mutex> lock2(g_mutex2); // Acquire lock2
    std::cout << "Thread 2: Mutex2 locked. Waiting a bit...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work

    // Critical section - will not be reached in a deadlock
    std::cout << "Thread 2: Doing work with both locks...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Thread 2: Releasing both locks.\n";
    // lock1 and lock2 are automatically released when they go out of scope
}

int main()
{
    std::cout << "Simulating a potential deadlock scenario...\n";

    std::thread t1(thread_func1);
    std::thread t2(thread_func2);

    t1.join();
    t2.join();

    std::cout << "Program finished. (If you see this, a deadlock might not have occurred this time, "
              << "or it was resolved by OS scheduling. "
              << "Run multiple times to observe deadlock or consider a longer sleep).\n";

    return 0;
}