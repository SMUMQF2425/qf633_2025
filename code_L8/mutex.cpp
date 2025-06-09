#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

std::mutex mtx; // Declare a mutex
string getThreadId()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

void sharedResourceAccess(int id)
{
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex
    // Access the shared resource here
    cout << "construct thread: " << id << endl;
    cout << "thread: " << getThreadId() << " is doing task with shared resources" << endl;
    // The mutex will be automatically unlocked when 'lock' goes out of scope
}

void doTask(int id)
{
    // cout << "construct thread: " << id << endl;
    sharedResourceAccess(id);
};

int main()
{
    std::thread t1(doTask, 0);
    std::thread t2(doTask, 1);
    t1.join();
    t2.join();

    return 0;
}