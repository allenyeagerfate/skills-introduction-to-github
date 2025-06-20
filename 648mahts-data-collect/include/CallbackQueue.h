#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <array>
#include <condition_variable>

class CallbackQueue 
{
public:
    CallbackQueue(size_t maxSize);
    ~CallbackQueue();
    
    // 初始化线程池
    void Init();  // 新增的初始化方法声明

    // 添加任务到队列
    void call(std::function<void()>&& function);
    
    // 停止队列处理
    void stop();
    
    // 获取当前队列大小
    size_t size() const;
    
    // 检查队列是否为空
    bool empty() const;

private:
    void _run(uint32_t index);

    std::mutex _mutex;
    std::condition_variable _conditionVariable;
    std::queue<std::function<void()>> _functions;
    std::array<std::thread, 1> _threads;
    const size_t _maxSize;
    bool _running = false;  // 控制线程运行状态
};
