// Thread pool using C++11 

#ifndef __THREAD_POOL_CPP11_H__
#define __THREAD_POOL_CPP11_H__

#include <thread>
#include <vector>
#include <mutex>
#include <future>
#include <queue>
#include <condition_variable>

class ThreadPool;

class Worker {
public:
  Worker(ThreadPool *tp) : _pool(tp) {}
  void operator()();
  
private:
  ThreadPool *_pool;
};

class ThreadPool {
public:
  ThreadPool(const size_t threadcount);
  ~ThreadPool();
  
  template<typename Func, typename Args>
  void enqueue(Func &&f, Args&& args);
  void wait();
  
private:
  friend Worker;
  std::vector<std::thread> _workers;
  std::queue<std::function<void()> > _task_queue;
  size_t _inprogress;
  
  std::mutex _mutex;
  std::condition_variable _condition;
  
  bool _stop;
};

template<typename Func, typename Args>
void ThreadPool::enqueue(Func &&f, Args&& args) {
  //create a shared pointer to a task and then put a lambda that
  //executes that task in the queue
  using rettype = typename std::result_of<Func(Args)>::type;
  auto task = std::make_shared<std::packaged_task<rettype()> >(
      std::bind(std::forward<Func>(f), std::forward<Args>(args)));
 
  std::unique_lock<std::mutex> lock (_mutex);
  _task_queue.push([task]() { (*task)(); } );
  lock.unlock();
  
  _condition.notify_one();
}

#endif  // __THREAD_POOL_CPP11_H__
