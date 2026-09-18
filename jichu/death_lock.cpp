#include <iostream>
#include <thread>
#include <mutex>

std::mutex m1, m2;

void fun1()
{
    for (int i = 0; i < 5000; i++)
    {
        m1.lock();
        m2.lock();
        m1.unlock();
        m2.unlock();
    }
}

void fun2()
{
    for (int i = 0; i < 5000; i++)
    {
        m2.lock();
        m1.lock();
        m2.unlock();
        m1.unlock();
    }
}

int main()
{
    std::thread t1(fun1);
    std::thread t2(fun2);
    t1.join();
    t2.join();
    std::cout << "over" << '\n';
    return 0;
}