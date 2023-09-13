#include <Logger.hpp>
#include <ThreadPool.hpp>
#include <iostream>
#include <thread>

ThreadPool::ThreadPool(const unsigned int &thread_count)
	: _stop(false), _thread_count(thread_count)
{
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::Start()
{
	Logger &logger = Logger::getInstance();
	if (_thread_count == 0)
	{
		logger.log(INFO, "Using maximum number of threads");
		_thread_count = std::thread::hardware_concurrency();
	}
	for (std::size_t i = 0; i < _thread_count; ++i)
	{
		_threads.emplace_back(&ThreadPool::ThreadLoop, this);
	}
}

void ThreadPool::ThreadLoop()
{
	while (true)
	{
		std::function<void()> task;
		{
			{
				std::unique_lock<std::mutex> lock(_queue_mutex);
				_condition.wait(lock,
								[this] { return !_tasks.empty() || _stop; });
			}
			if (_stop == true)
				return;
			task = _tasks.front();
			_tasks.pop();
		}
	}
}

void ThreadPool::QueueTask(const std::function<void()> &task)
{
	std::unique_lock<std::mutex> lock(_queue_mutex);
	_tasks.push(task);
	_condition.notify_one();
}

bool ThreadPool::busy()
{
	bool poolbusy;
	{
		std::unique_lock<std::mutex> lock(_queue_mutex);
		poolbusy = !_tasks.empty();
	}
	return (poolbusy);
}
void ThreadPool::Stop()
{
	{
		std::unique_lock<std::mutex> lock(_queue_mutex);
		_stop = true;
	}
	_condition.notify_all();
	for (std::thread &thread : _threads)
	{
		thread.join();
	}
	_threads.clear();
}
