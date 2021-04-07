#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Thread.h"
#include <boost/scoped_ptr.hpp>
#include <vector>
#include "Mutex.h"

class Channel;
class Poller;

class EventLoop : boost::noncopyable
{
 public:
  typedef std::vector<Channel*> ChannelList;
  typedef boost::function<void()> Functor;

  EventLoop();
  ~EventLoop();

  void loop();

  void assertInLoopThread()
  {
    if (!isInLoopThread())
    {
      abortNotInLoopThread();
    }
  }

  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
  void quit();
  static EventLoop* getEventLoopOfCurrentThread();
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);
  void runInLoop(const Functor& cb);
  void queueInLoop(const Functor& cb);
  void wakeup();

 private:

  void abortNotInLoopThread();
  void handleRead();
  void doPendingFunctors();

  bool looping_; /* atomic */
  bool quit_;
  bool callingPendingFunctors_;
  const pid_t threadId_;
  int wakeupFd_;
  boost::scoped_ptr<Channel> wakeupChannel_;
  boost::scoped_ptr<Poller> poller_;
  MutexLock mutex_;
  std::vector<Functor> pendingFunctors_;
  ChannelList activeChannels_;
};


#endif // EVENTLOOP_H
