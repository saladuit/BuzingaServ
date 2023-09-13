#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

class ThreadPool
{
  private:
	unsigned int _thread_count;

  public:
	ThreadPool() = delete;
	ThreadPool(const unsigned int &thread_count);
	ThreadPool(const ThreadPool &src) = delete;
	ThreadPool &operator=(const ThreadPool &rhs) = delete;
	~ThreadPool();
};

#endif
