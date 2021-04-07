#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"

#include <boost/noncopyable.hpp>


class EventLoop;

class EventLoopThread : boost::noncopyable
{
 public:
  EventLoopThread();
  ~EventLoopThread();
  EventLoop* startLoop();

 private:
  void threadFunc();

  EventLoop* loop_;
  bool exiting_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_;
};
#endif // EVENTLOOPTHREAD_H
