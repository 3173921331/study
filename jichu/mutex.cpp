#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

// 原子操作比mutex加锁快

int shared_date = 0;
std::mutex mtx;

void func()
{
    for (int i = 0; i < 100000; i++)
    {
        mtx.lock();
        shared_date++;
        mtx.unlock();
    }
}

int main()
{
    auto last = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::thread t1(func);
    std::thread t2(func);

    t1.join();
    t2.join();
    std::cout << "shared_date : " << shared_date << std::endl;

    auto cur = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << cur - last << std::endl;
    return 0;
}