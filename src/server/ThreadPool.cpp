#include <ThreadPool.hpp>

ThreadPool::ThreadPool(size_t numThreads)
{
    for (size_t i = 0; i < numThreads; ++i) 
    {
        workersContainer.emplace_back([this] {
            while (true) 
            {
                std::function<void()> task;
                {
                    std::unique_lock lock(mtx);
                    cv.wait(lock, [this] {
                        return isStopped || !tasksQueue.empty();
                    });
                    if (isStopped && tasksQueue.empty()) 
                        return;
                    task = std::move(tasksQueue.front());
                    tasksQueue.pop();
                }
                task();
            }
        });
    }
}

void ThreadPool::enqueue(std::function<void()> task) 
{
    {
        std::lock_guard lock(mtx);
        tasksQueue.push(std::move(task));
    }
    cv.notify_one();
}

ThreadPool::~ThreadPool()
 {
    isStopped = true;
    cv.notify_all();
    for (auto& t : workersContainer)
        t.join();
}