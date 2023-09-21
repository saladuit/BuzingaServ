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
	Stop();
}

const size_t &ThreadPool::getThreadCount() const
{
	return (_thread_count);
}

void ThreadPool::Start()
{
	Logger &logger = Logger::getInstance();
	if (_thread_count == 0)
	{
		_thread_count = std::thread::hardware_concurrency();
		logger.log(INFO, "Using maximum number of threads: " +
							 std::to_string(_thread_count));
	}
	for (std::size_t i = 0; i < _thread_count; ++i)
	{
		_threads.emplace_back(&ThreadPool::ThreadLoop, this);
	}
}

void ThreadPool::ThreadLoop()
{
	Logger &logger = Logger::getInstance();
	while (true)
	{
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(_queue_mutex);
			_condition.wait(lock, [this] { return !_tasks.empty() || _stop; });
			if (_stop == true)
				return;
			logger.log(DEBUG, "Thread % is running",
					   std::this_thread::get_id());
			task = _tasks.front();
			_tasks.pop();
		}
		task();
	}
}

void ThreadPool::QueueTask(const std::function<void()> &task)
{
	Logger &logger = Logger::getInstance();
	{
		std::unique_lock<std::mutex> lock(_queue_mutex);
		_tasks.push(task);
		logger.log(DEBUG, "New task queued");
	}
	_condition.notify_one();
}

bool ThreadPool::isBusy()
{
	std::unique_lock<std::mutex> lock(_queue_mutex);
	return (!_tasks.empty());
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
