#include <iostream>
#include <thread>
#include <mutex>
#include <string>

static std::once_flag once;
// 全局只有一个对象，用的时候直接调用static对象，单例模式
class Log
{
public:
    Log() {};
    Log(const Log &log) = delete;
    Log &operator=(const Log &log) = delete;

    static Log &GetInstance()
    {
        // static Log log; // 懒汉模式
        // return log;
        // static Log *log = nullptr; // 饿汉模式
        // if (!log)
        //     log = new Log;

        std::call_once(once, init);
        return *log;
    }

    static void init()
    {
        if (!log)
            log = new Log;
    }

    void PrintLog(std::string msg)
    {
        std::cout << __TIME__ << ' ' << msg << std::endl;
    }

private:
    static Log *log;
};
Log *Log::log = nullptr;

void print_error()
{
    Log::GetInstance().PrintLog("error");
}

int main()
{
    std::thread t1(print_error);
    std::thread t2(print_error);

    t1.join();
    t2.join();

    return 0;
}