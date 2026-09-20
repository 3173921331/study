#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <memory>

// 全局只有一个对象，用的时候直接调用static对象，单例模式
class Log
{
public:
    static Log &GetInstance()
    {
        // static Log log; // 懒汉模式
        // return log;
        // static Log *log = nullptr; // 饿汉模式
        // if (!log)
        //     log = new Log;

        //保证多个线程中这个init函数只调用一次
        std::call_once(once, init);
        return *log;
    }

    static void init()
    {
        log.reset(new Log());
    }

    void PrintLog(const std::string &msg)
    {
        std::lock_guard<std::mutex> lg(mtx);
        std::cout << __TIME__ << ' ' << msg << std::endl;
    }

private:
    Log() = default;
    Log(const Log &log) = delete;
    Log &operator=(const Log &log) = delete;

    static std::unique_ptr<Log> log;
    static std::once_flag once;
    static std::mutex mtx;
};
std::unique_ptr<Log> Log::log = nullptr;
std::once_flag Log::once;
std::mutex Log::mtx;

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

// Meyer单例，C++11后
//  class Log
//  {
//  public:
//      Log(const Log &) = delete;
//      Log &operator=(const Log &) = delete;
//      static Log &GetInstance()
//      {
//          static Log obj;
//          return obj;
//      }
//      void PrintLog(const std::string &msg)
//      {
//          std::lock_guard<std::mutex> lg(mtx);
//          std::cout << msg << '\n';
//      }

// private:
//     Log() = default;
//     static std::mutex mtx;
// };
// std::mutex Log::mtx;
