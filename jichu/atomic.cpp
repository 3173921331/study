#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

//原子操作比mutex加锁快

std::atomic<int> shared_date = 0;

void func()
{
    for (int i = 0; i < 100000; i++)
    {
        shared_date++;
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
    shared_date.store(1);
    std::cout << "shared_date : " << shared_date.load() << std::endl;

    auto cur = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << cur - last << std::endl;
    return 0;
}