// C++ program to demonstrate multithreading using three different callables.
#include <iostream>
#include <thread>
#include <chrono>
#include <syncstream>

using namespace std;

// Global mutex to protect std::cout
std::mutex cout_mutex;

void getThreadId()
{
    std::thread::id this_id = std::this_thread::get_id();
    // Acquire lock before printing? why?
    //std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "this thread id: " << this_id << endl; // This entire statement is now atomic
}

// type 1: none static function
void foo(int Z)
{
    getThreadId();
    //std::lock_guard<std::mutex> lock(cout_mutex);
    for (int i = 0; i < Z; i++)
    {
        cout << "Thread using function pointer as callable\n";
    }
    std::this_thread::sleep_for(500ms);
}

// type 3: a callable object
class thread_obj
{
public:
    thread_obj()
    {
        getThreadId();
    }
    void operator()(int x)
    {
        //std::lock_guard<std::mutex> lock(cout_mutex);
        for (int i = 0; i < x; i++)
            cout << "Thread using function object as callable\n";
    }
};

// type 4: class definition
class Base
{
public:
    // non-static member function
    void foo()
    {
        //std::lock_guard<std::mutex> lock(cout_mutex);
        cout << "Thread using non-static member function as callable" << endl;
    }

    // static member function
    static void foo1()
    {
        //std::lock_guard<std::mutex> lock(cout_mutex);
        cout << "Thread using static member function as callable" << endl;
    }
};

// Driver code
int main()
{
    cout << "Threads are operating independently" << endl;
    getThreadId();

    // This thread is launched by using function pointer as callable
    thread th1(foo, 3);

    // This thread is launched by using function object as callable
    thread th2(thread_obj(), 3);

    // type2: using a Lambda Expression
    auto f = [](int x)
    {
        for (int i = 0; i < x; i++)
            cout << "Thread using lambda expression as callable\n";
    };
    thread th3(f, 3);

    // object of Base Class
    Base b;
    thread th4(&Base::foo, &b);
    thread th5(&Base::foo1);

    // Wait for thread t1 to finish
    th1.join();

    // Wait for thread t2 to finish
    th2.join();

    // Wait for thread t3 to finish
    th3.join();

    // Wait for thread t4 to finish
    th4.join();

    // Wait for thread t5 to finish
    th5.join();

    cout << "all threads are completed" << endl;

    return 0;
}