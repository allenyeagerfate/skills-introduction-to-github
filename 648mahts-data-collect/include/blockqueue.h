#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <iostream>

template<class T>
class BlockQueue {
public:
	using size_type = typename std::deque<T>::size_type;

public:
	BlockQueue(const int cap = 500 ) : m_maxCapacity(cap)
	{
		m_ptrQueue = std::make_shared<std::deque<T>>();
	}
	~BlockQueue(){}

	BlockQueue(const BlockQueue &) = delete;
	BlockQueue &operator = (const BlockQueue &) = delete;

public:
	void put(const T t)
	{
		{
			T v = t;
		    std::unique_lock<std::mutex> lock(m_mutex);
		    // if(m_maxCapacity != -1)
			// {
			//     m_cond_full.wait(lock, [this]{ return m_ptrQueue->size() < m_maxCapacity; });
		    // }

			if(m_ptrQueue->size() > m_maxCapacity)
			{
                m_ptrQueue->pop_front();  //TBD
			}

		    m_ptrQueue->push_back(v);
		}

		m_cond_empty.notify_all();
	}
	
	T take()
	{
		T res;
		{
		    std::unique_lock<std::mutex> lock(m_mutex);
		    m_cond_empty.wait(lock, [&](){return !m_ptrQueue->empty();});
		    res = m_ptrQueue->front();
		    m_ptrQueue->pop_front();
		}

		//m_cond_full.notify_all();

		return res;
	}

	bool empty() const{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_ptrQueue->empty();
	}

	bool full() const{
		if(-1 == m_maxCapacity)
			return false;
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_ptrQueue->size() >= m_maxCapacity;
	}

	size_type size(){
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_ptrQueue->size();
	}


	bool offer(const T t){
		{
		    std::lock_guard<std::mutex> lock(m_mutex);
		    //if(m_maxCapacity != -1 && m_queue.size() >= 1200){//3s一条，按一个小时的量
		    if(m_ptrQueue->size() >= 1200){
		    	//return false;
		    	m_ptrQueue->pop_front();//弹出最前面的元素
		    }
		    m_ptrQueue->push_back(t);
		}
		m_cond_empty.notify_all();
		return true;
	}


	bool poll(T& t){
		{
		    std::lock_guard<std::mutex> lock(m_mutex);
		    if(m_ptrQueue->empty()){
		    	return false;
		    }
		    t = m_ptrQueue->front();
		    m_ptrQueue->pop_front();
		}

		//m_cond_full.notify_all();
		return true;
	}

	// bool offer(const T t, std::chrono::milliseconds& time){
	// 	{
	// 	    std::lock_guard<std::mutex> lock(m_mutex);
	// 	    if(m_maxCapacity != -1){
	// 	    	bool result = m_cond_full.wait(lock, time, 
	// 	    			[&]{ return m_ptrQueue->size() < m_maxCapacity; });
	// 	    	if(!result){
	// 	    		return false;
	// 	    	}
	// 	    }
	// 	    m_ptrQueue->push_back(t);
	// 	}
	// 	m_cond_empty.notify_all();
	// 	return true;
	// }


	// bool poll(T& t, std::chrono::milliseconds& time){
	// 	{
	// 	    std::lock_guard<std::mutex> lock(m_mutex);
	// 	    bool result = m_cond_empty.wait_for(lock, time, 
	// 	    		[&] {return !m_ptrQueue->empty();});
	// 	    if(!result){
	// 	    	return false;
	// 	    }
	// 	    t = m_ptrQueue->front();
	// 	    m_ptrQueue->pop_front();
	// 	}
	// 	m_cond_full.notify_all();
	// 	return true;
	// }

	void setCapacity(int cap){ m_maxCapacity = cap;}
	
private:
	std::shared_ptr<std::deque<T>> m_ptrQueue = nullptr;
	const int m_maxCapacity;
	mutable std::mutex m_mutex;
	std::condition_variable m_cond_empty;
	//std::condition_variable m_cond_full;
};

#endif