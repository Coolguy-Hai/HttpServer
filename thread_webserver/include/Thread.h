#ifndef THREAD_H
#define THREAD_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <pthread.h>

class Thread : boost::noncopyable
{
 public:
  typedef boost::function<void ()> ThreadFunc;

  explicit Thread(const ThreadFunc&, const std::string& name = std::string());
  ~Thread();

  void start();
  void join();

  bool started() const { return started_; }
  pthread_t pthreadId() const { return pthreadId_; }
  pid_t tid() const { return tid_; }
  const std::string& name() const { return name_; }

 private:
  static void* startThread(void* thread);
  void runInThread();

  bool        started_;
  pthread_t   pthreadId_;
  pid_t       tid_;
  ThreadFunc  func_;
  std::string name_;
};

namespace CurrentThread
{
  pid_t tid();
  const char* name();
  bool isMainThread();
}

#endif // THREAD_H
