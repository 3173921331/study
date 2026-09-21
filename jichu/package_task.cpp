#include <iostream>
#include <thread>
#include <future>

int func()
{
    int i = 0;
    for (i = 0; i < 1000; i++)
    {
        i++;
    }
    return i;
}

int main()
{
    std::packaged_task<int()> task(func);

    auto future_result = task.get_future();

    std::thread t1(std::move(task));
    
    std::cout << future_result.get() << std::endl;

    std::cout << "zhijie" << func() << std::endl;


    t1.join();


    return 0;
}