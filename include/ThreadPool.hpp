#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
  private:
	bool _stop;
	std::size_t _thread_count;
	std::vector<std::thread> _threads;
	std::mutex _queue_mutex;
	std::condition_variable _condition;
	std::queue<std::function<void()>> _tasks;

	void ThreadLoop();

  public:
	void Start();
	void QueueTask(const std::function<void()> &task);
	void Stop();
	bool isBusy();
	ThreadPool() = delete;
	ThreadPool(const unsigned int &thread_count);
	ThreadPool(const ThreadPool &src) = delete;
	ThreadPool &operator=(const ThreadPool &rhs) = delete;
	~ThreadPool();
};

#endif
