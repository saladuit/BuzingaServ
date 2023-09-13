#include <ThreadPool.hpp>
#include <thread>

ThreadPool::ThreadPool(const unsigned int &thread_count)
	: _thread_count(thread_count)
{
}

ThreadPool::~ThreadPool()
{
}
