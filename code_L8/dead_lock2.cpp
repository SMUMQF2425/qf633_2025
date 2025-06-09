#include <iostream>
#include <thread>
#include <mutex> // For std::mutex and std::scoped_lock
#include <chrono>
#include <string>

std::mutex s_mutex1;
std::mutex s_mutex2;

void thread_func_scoped_lock1()
{
    std::cout << "Thread 1 (scoped_lock): Trying to lock both mutexes...\n";
    // Locks s_mutex1 then s_mutex2 in an atomic, deadlock-free manner
    std::scoped_lock lock(s_mutex1, s_mutex2);
    std::cout << "Thread 1 (scoped_lock): Both mutexes locked. Doing work...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread 1 (scoped_lock): Releasing both locks.\n";
}

void thread_func_scoped_lock2()
{
    std::cout << "Thread 2 (scoped_lock): Trying to lock both mutexes...\n";
    // Locks s_mutex1 then s_mutex2 in an atomic, deadlock-free manner
    std::scoped_lock lock(s_mutex2, s_mutex1); // Order passed to scoped_lock doesn't matter for deadlock prevention
    std::cout << "Thread 2 (scoped_lock): Both mutexes locked. Doing work...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread 2 (scoped_lock): Releasing both locks.\n";
}

int main_scoped_lock()
{
    std::cout << "\nSimulating a deadlock-prevented scenario with std::scoped_lock...\n";
    std::thread t1_sl(thread_func_scoped_lock1);
    std::thread t2_sl(thread_func_scoped_lock2);

    t1_sl.join();
    t2_sl.join();

    std::cout << "Scoped_lock program finished successfully.\n";

    return 0;
}

int main()
{
    main_scoped_lock();
}