#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

std::mutex mutex1;
std::mutex cout_mutex;

void safe_print_thread_func(int id, const std::string &msg)
{
    // Acquire the lock before writing to std::cout
    // std::lock_guard ensures the mutex is unlocked when 'lock' goes out of scope
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << id << ":" << msg << endl; // This entire write operation is now atomic                         
    // Simulating some work after printing
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void some_task(int &x, int &y)
{
    if (x == 5) // The "Check"
    {
        y = x * 2; // The "Act"
        // If another thread changed x in between "if (x == 5)" and "y = x * 2" above, y will not be equal to 10.
    }
    // std::lock_guard<std::mutex> lock(mutex1);
    cout << std::this_thread::get_id() << ":" << "x: " << x << ", y: " << y << endl;
}

void ThreadA(int &x, int &y)
{
    std::cout << "Thread A is executing" << std::endl;
    std::lock_guard<std::mutex> lock(mutex1);
    x = 5;
    some_task(x, y);
}

void ThreadB(int &x, int &y)
{
    std::cout << "Thread B is executing" << std::endl;
    some_task(x, y);
}

int main()
{
    int x = 0;
    int y;
    thread t1(ThreadA, std::ref(x), std::ref(y));
    thread t2(ThreadB, std::ref(x), std::ref(y));

    t1.join();
    t2.join();

    std::cout << "Finished" << std::endl;
    return 0;
}