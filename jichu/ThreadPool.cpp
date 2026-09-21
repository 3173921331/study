#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <future>

class ThreadPool
{
public:
    ThreadPool(int numThreads) : stop(false)
    {
        for (int i = 0; i < numThreads; i++)
        {
            threads.emplace_back([this]
                                 {
                while(1)
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    condition.wait(lock, [this]
                                   { return !tasks.empty() || stop; });
                    if(stop && tasks.empty())
                        return;

                    std::function<void()> task(std::move(tasks.front()));
                    tasks.pop();
                    lock.unlock();
                    task();
                } });
        }
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
        }

        condition.notify_all();
        for (auto &t : threads)
        {
            t.join();
        }
    }

    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type>
    {
        using ResultType = typename std::invoke_result<F, Args...>::type;
        // packaged_task放到堆上unique_ptr
        auto task_ptr = std::make_shared<std::packaged_task<ResultType()>>(
            [f = std::forward<F>(f), ... args = std::forward<Args>(args)]()
            { return f(args...); });
        auto future_result = task_ptr->get_future();

        {
            std::unique_lock<std::mutex> lock(mtx);
            // 移动捕获unique_ptr，lambda存入std::function<void()>
            tasks.emplace([t = std::move(task_ptr)]()
                          { (*t)(); });
        }
        condition.notify_one();
        return future_result;
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable condition;

    bool stop;
};

std::mutex cmtx;

int main()
{
    ThreadPool pool(4);
    std::vector<std::future<int>> futures;

    for (int i = 0; i < 10; i++)
    {
        auto fut = pool.enqueue([i]
                                {
            std::unique_lock<std::mutex> lock(cmtx);
            std::cout << "task : " << i << " is runing" << std::endl;
            std::cout << "task : " << i << " is done" << std::endl;
            return i; });
        futures.push_back(std::move(fut));
    }
    // 全部任务提交完毕，再等待结果
    for (auto &f : futures)
    {
        int res = f.get();
        std::unique_lock<std::mutex> lock(cmtx);
        std::cout << res << '\n';
    }
    return 0;
}