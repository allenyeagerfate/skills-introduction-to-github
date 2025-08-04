#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_set>

class ReadWriteLock {
public:
    ReadWriteLock() : m_readers(0), m_writers(0), m_writeRequested(0) 
    {}
    
    ~ReadWriteLock() = default;
    
    // 禁用拷贝和移动构造
    ReadWriteLock(const ReadWriteLock&) = delete;
    ReadWriteLock& operator=(const ReadWriteLock&) = delete;
    ReadWriteLock(ReadWriteLock&&) = delete;
    ReadWriteLock& operator=(ReadWriteLock&&) = delete;
    void lock_shared() {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_writers > 0 || m_writeRequested > 0) {
            m_readerCondition.wait(lock);
        }
        ++m_readers;
        m_readerThreads.insert(std::this_thread::get_id());
    }
    
    void unlock_shared() {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        // 验证当前线程确实持有读锁
        auto threadId = std::this_thread::get_id();
        if (m_readerThreads.find(threadId) == m_readerThreads.end()) {
            //throw std::logic_error("Thread does not hold a read lock");
        }
        
        m_readerThreads.erase(threadId);
        --m_readers;
        
        if (m_readers == 0 && m_writeRequested > 0) {
            m_writerCondition.notify_one();
        }
    }
    
    void lock() {
        std::unique_lock<std::mutex> lock(m_mutex);
        ++m_writeRequested;
        
        while (m_readers > 0 || m_writers > 0) {
            m_writerCondition.wait(lock);
        }
        
        --m_writeRequested;
        ++m_writers;
        m_writerThread = std::this_thread::get_id();
    }
    
    void unlock() {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        // 验证当前线程确实持有写锁
        if (m_writerThread != std::this_thread::get_id()) {
            //throw std::logic_error("Thread does not hold the write lock");
            //TBD
        }
        
        --m_writers;
        m_writerThread = std::thread::id();
        
        if (m_writeRequested > 0) {
            m_writerCondition.notify_one();
        } else {
            m_readerCondition.notify_all();
        }
    }
    
    bool try_lock_shared() {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        if (m_writers > 0 || m_writeRequested > 0) {
            return false;
        }
        
        ++m_readers;
        m_readerThreads.insert(std::this_thread::get_id());
        return true;
    }
    
    bool try_lock() {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        if (m_readers > 0 || m_writers > 0 || m_writeRequested > 0) {
            return false;
        }
        
        ++m_writers;
        m_writerThread = std::this_thread::get_id();
        return true;
    }
    
    // 调试方法
    bool is_locked_by_current_thread_shared() const {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_readerThreads.find(std::this_thread::get_id()) != m_readerThreads.end();
    }
    
    bool is_locked_by_current_thread_unique() const {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_writerThread == std::this_thread::get_id();
    }

private:
    mutable std::mutex m_mutex;
    std::condition_variable m_readerCondition;
    std::condition_variable m_writerCondition;
    
    int m_readers;
    int m_writers;
    int m_writeRequested;
    
    // 跟踪持有锁的线程（用于调试和验证）
    std::unordered_set<std::thread::id> m_readerThreads;
    std::thread::id m_writerThread;
};