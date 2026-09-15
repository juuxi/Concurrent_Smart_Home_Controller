#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <atomic>

class ThreadPool {
    std::vector<std::thread> workersContainer;
    std::queue<std::function<void()>> tasksQueue;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> is_stopped{false};

public:
    explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency());

    void enqueue(std::function<void()> task);

    ~ThreadPool();
};