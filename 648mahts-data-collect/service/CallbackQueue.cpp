#include "CallbackQueue.h"

CallbackQueue::CallbackQueue(size_t maxSize) : _maxSize(maxSize) 
{

}


void CallbackQueue::Init()
{
	_running = true;
	for (uint32_t i = 0; i < _threads.size(); ++i)
	{
		_threads[i] = std::thread(&CallbackQueue::_run, this, i);
	}
}



CallbackQueue::~CallbackQueue() 
{
	_running = false;
	for (uint32_t i = 0; i < _threads.size(); ++i)
	{
		_threads[i].join();
	}
}

void CallbackQueue::call(std::function<void()>&& function) 
{
	std::unique_lock<std::mutex> locker(_mutex);
	while (_functions.size() >= _maxSize) 
	{
		//util::log("network callback queue is full!");
		_conditionVariable.wait(locker);
	}
	_functions.push(std::move(function));
	locker.unlock();

	_conditionVariable.notify_all();
}

void CallbackQueue::stop()
{
	_running = false;
}

void CallbackQueue::_run(uint32_t index) 
{
	try 
	{
		while (_running)
		{
			std::unique_lock<std::mutex> locker(_mutex);
			std::function<void()> function = nullptr;
			while (_functions.empty())
			{
				_conditionVariable.wait(locker);
			}
			function = std::move(_functions.front());
			_functions.pop();
			locker.unlock();
			
			_conditionVariable.notify_all();

			if (function)
			{
				function();
			}
		}
	}
	catch (const std::exception& e) 
	{
		//util::log(std::string("network callback queue exception: ") + e.what());
		throw;
	}
	catch (...) 
	{
		//util::log("network callback queue exception: unknown");
		throw;
	}
}