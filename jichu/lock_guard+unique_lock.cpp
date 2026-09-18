#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

int shared_date = 0;

void fun1()
{
    for (int i = 0; i < 10000; i++)
    {
        // 不支持复制和移动
        //  默认加锁，多传一个参数adopt_lock不加锁,析构自动解锁
        std::lock_guard<std::mutex> lg(mtx);
        // std::lock_guard<std::mutex> lg(mtx, std::adopt_lock);
        shared_date++;
    }
}

std::timed_mutex t_mtx;

void fun2()
{
    for (int i = 0; i < 2; i++)
    {
        // 不支持复制和移动，但支持右值赋值操作,可以调用swap
        //  默认加锁，多传一个参数defer_lock不加锁,析构自动解锁
        std::unique_lock<std::timed_mutex> lg(t_mtx, std::defer_lock);
        // lg.lock();
        // 延迟加锁,等待2秒，没拿到直接返回false
        if (lg.try_lock_for(std::chrono::seconds(2)))
        {
            // 休眠1秒
            std::this_thread::sleep_for(std::chrono::seconds(1));
            shared_date++;
        }
    }
}

int main()
{
    std::thread t1(fun2);
    std::thread t2(fun2);
    t1.join();
    t2.join();
    std::cout << shared_date << '\n';
    return 0;
}