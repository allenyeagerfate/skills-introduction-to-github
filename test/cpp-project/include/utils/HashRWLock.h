#pragma once

#include "MurmurHash.h"
#include "ReadWriteLock.h"
#include <shared_mutex>
#include <unordered_map>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include <atomic>
#include <algorithm>
#include <cstdint>
// 改进的 HashBasedRWLock，使用 MurmurHash
template<typename KeyType>
class HashBasedRWLock {
public:
    HashBasedRWLock(uint32_t bucketCount = 16) : 
    m_bucketCount(bucketCount)
    {
        // 确保桶数量是2的幂，用于快速取模
        m_bucketCount = nextPowerOfTwo(bucketCount);
        m_locks.resize(m_bucketCount);
        
        // 初始化锁对象
        for (uint32_t i = 0; i < m_bucketCount; ++i) {
            m_locks[i] = std::make_unique<ReadWriteLock>();
        }
        
        // 初始化统计信息
        //m_lockUsageCount.resize(m_bucketCount, 0);
    }
    
    ~HashBasedRWLock() = default;
    
    // 禁用拷贝和移动
    HashBasedRWLock(const HashBasedRWLock&) = delete;
    HashBasedRWLock& operator=(const HashBasedRWLock&) = delete;
    HashBasedRWLock(HashBasedRWLock&&) = delete;
    HashBasedRWLock& operator=(HashBasedRWLock&&) = delete;
    
    // 获取读锁
    void lock_shared(const KeyType& key) {
        size_t index = getIndex(key);
        m_locks[index]->lock_shared();
        // 原子地增加使用计数
        //m_lockUsageCount[index].fetch_add(1, std::memory_order_relaxed);
    }
    
    // 释放读锁
    void unlock_shared(const KeyType& key) {
        size_t index = getIndex(key);
        m_locks[index]->unlock_shared();
    }
    
    // 获取写锁
    void lock(const KeyType& key) {
        size_t index = getIndex(key);
        m_locks[index]->lock();
        //m_lockUsageCount[index].fetch_add(1, std::memory_order_relaxed);
    }
    
    // 释放写锁
    void unlock(const KeyType& key) {
        size_t index = getIndex(key);
        m_locks[index]->unlock();
    }
    
    // 尝试获取读锁
    bool try_lock_shared(const KeyType& key) {
        size_t index = getIndex(key);
        bool acquired = m_locks[index]->try_lock_shared();
        if (acquired) {
           //m_lockUsageCount[index].fetch_add(1, std::memory_order_relaxed);
        }
        return acquired;
    }
    
    // 尝试获取写锁
    bool try_lock(const KeyType& key) {
        size_t index = getIndex(key);
        bool acquired = m_locks[index]->try_lock();
        if (acquired) {
            //m_lockUsageCount[index].fetch_add(1, std::memory_order_relaxed);
        }
        return acquired;
    }
    
    // RAII读锁
    class SharedLock {
    public:
        SharedLock(HashBasedRWLock& lock, const KeyType& key) 
            : m_lock(lock), m_key(key), m_locked(true) {
            m_lock.lock_shared(m_key);
        }
        
        ~SharedLock() {
            if (m_locked) {
                m_lock.unlock_shared(m_key);
            }
        }
        
        // 手动解锁
        void unlock() {
            if (m_locked) {
                m_lock.unlock_shared(m_key);
                m_locked = false;
            }
        }
        
        // 禁用拷贝，但允许移动
        SharedLock(const SharedLock&) = delete;
        SharedLock& operator=(const SharedLock&) = delete;
        
        SharedLock(SharedLock&& other) noexcept 
            : m_lock(other.m_lock), m_key(std::move(other.m_key)), m_locked(other.m_locked) {
            other.m_locked = false;
        }
        
        SharedLock& operator=(SharedLock&& other) noexcept {
            if (this != &other) {
                if (m_locked) {
                    m_lock.unlock_shared(m_key);
                }
                m_key = std::move(other.m_key);
                m_locked = other.m_locked;
                other.m_locked = false;
            }
            return *this;
        }
        
    private:
        HashBasedRWLock& m_lock;
        KeyType m_key;
        bool m_locked;
    };
    
    // RAII写锁
    class UniqueLock {
    public:
        UniqueLock(HashBasedRWLock& lock, const KeyType& key) 
            : m_lock(lock), m_key(key), m_locked(true) {
            m_lock.lock(m_key);
        }
        
        ~UniqueLock() {
            if (m_locked) {
                m_lock.unlock(m_key);
            }
        }
        
        // 手动解锁
        void unlock() {
            if (m_locked) {
                m_lock.unlock(m_key);
                m_locked = false;
            }
        }
        
        // 禁用拷贝，但允许移动
        UniqueLock(const UniqueLock&) = delete;
        UniqueLock& operator=(const UniqueLock&) = delete;
        
        UniqueLock(UniqueLock&& other) noexcept 
            : m_lock(other.m_lock), m_key(std::move(other.m_key)), m_locked(other.m_locked) {
            other.m_locked = false;
        }
        
        UniqueLock& operator=(UniqueLock&& other) noexcept {
            if (this != &other) {
                if (m_locked) {
                    m_lock.unlock(m_key);
                }
                m_key = std::move(other.m_key);
                m_locked = other.m_locked;
                other.m_locked = false;
            }
            return *this;
        }
        
    private:
        HashBasedRWLock& m_lock;
        KeyType m_key;
        bool m_locked;
    };
    
    // 多键锁定 - 按顺序锁定多个key以避免死锁
    template<typename Container>
    class MultiKeyLock {
    public:
        MultiKeyLock(HashBasedRWLock& lock, const Container& keys, bool exclusive = false)
            : m_lock(lock), m_exclusive(exclusive) {
            // 复制并排序键以避免死锁
            for (const auto& key : keys) {
                m_sortedKeys.push_back(key);
            }
            std::sort(m_sortedKeys.begin(), m_sortedKeys.end());
            
            // 去重
            m_sortedKeys.erase(std::unique(m_sortedKeys.begin(), m_sortedKeys.end()), 
                             m_sortedKeys.end());
            
            // 按顺序锁定
            for (const auto& key : m_sortedKeys) {
                if (m_exclusive) {
                    m_lock.lock(key);
                } else {
                    m_lock.lock_shared(key);
                }
            }
        }
        
        ~MultiKeyLock() {
            // 反序释放锁
            for (auto it = m_sortedKeys.rbegin(); it != m_sortedKeys.rend(); ++it) {
                if (m_exclusive) {
                    m_lock.unlock(*it);
                } else {
                    m_lock.unlock_shared(*it);
                }
            }
        }
        
        // 禁用拷贝和移动
        MultiKeyLock(const MultiKeyLock&) = delete;
        MultiKeyLock& operator=(const MultiKeyLock&) = delete;
        MultiKeyLock(MultiKeyLock&&) = delete;
        MultiKeyLock& operator=(MultiKeyLock&&) = delete;
        
    private:
        HashBasedRWLock& m_lock;
        std::vector<KeyType> m_sortedKeys;
        bool m_exclusive;
    };
    
    // 获取锁统计信息
    struct LockStats {
        size_t bucketCount;
        std::vector<size_t> lockUsage;
        double loadFactor;
        size_t maxUsage;
        size_t minUsage;
        double avgUsage;
    };
    
    LockStats getStats() const {
        LockStats stats;
        stats.bucketCount = m_bucketCount;
        stats.lockUsage.resize(m_bucketCount);
        
        size_t totalUsage = 0;
        size_t maxUsage = 0;
        size_t minUsage = SIZE_MAX;
        
        for (uint32_t i = 0; i < m_bucketCount; ++i) {
            //size_t usage = m_lockUsageCount[i].load(std::memory_order_relaxed);
            size_t usage = 0;
            stats.lockUsage[i] = usage;
            totalUsage += usage;
            maxUsage = std::max(maxUsage, usage);
            minUsage = std::min(minUsage, usage);
        }
        
        stats.maxUsage = maxUsage;
        stats.minUsage = (minUsage == SIZE_MAX) ? 0 : minUsage;
        stats.avgUsage = static_cast<double>(totalUsage) / m_bucketCount;
        stats.loadFactor = stats.avgUsage / (stats.maxUsage + 1); // 避免除零
        
        return stats;
    }
    
    // 重置统计信息
    void resetStats() {
        // for (auto& count : m_lockUsageCount) {
        //     count.store(0, std::memory_order_relaxed);
        // }
    }
    
private:
    size_t getIndex(const KeyType& key) const {
        uint32_t hash = MurmurHash3::hash(key);
        // 使用位运算代替取模，因为桶数量是2的幂
        return hash & (m_bucketCount - 1);
    }
    
    // 计算下一个2的幂
    static size_t nextPowerOfTwo(size_t n) {
        if (n <= 1) return 1;
        n--;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n |= n >> 32;
        return n + 1;
    }
    
    uint32_t m_bucketCount;
    std::vector<std::unique_ptr<ReadWriteLock>> m_locks;
    //std::vector<std::atomic<int>> m_lockUsageCount; // 使用计数  
};



    // RAII锁助手
    using DeviceReadLock = HashBasedRWLock<int>::SharedLock;
    using DeviceWriteLock = HashBasedRWLock<int>::UniqueLock;
    using GroupReadLock = HashBasedRWLock<std::string>::SharedLock;
    using GroupWriteLock = HashBasedRWLock<std::string>::UniqueLock;
    using FieldReadLock = HashBasedRWLock<std::string>::SharedLock;
    using FieldWriteLock = HashBasedRWLock<std::string>::UniqueLock;