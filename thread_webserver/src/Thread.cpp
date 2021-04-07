#include "Thread.h"

#include "Thread.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace CurrentThread
{
  __thread const char* t_threadName = "unknown";
}

__thread pid_t t_cachedTid = 0;

pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

pid_t CurrentThread::tid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = gettid();
  }
  return t_cachedTid;
}

const char* CurrentThread::name()
{
  return t_threadName;
}

bool CurrentThread::isMainThread()
{
  return tid() == ::getpid();
}

Thread::Thread(const ThreadFunc& func, const std::string& name)
  : started_(false),
    pthreadId_(0),
    tid_(0),
    func_(func),
    name_(name)
{
}

Thread::~Thread()
{
}

void Thread::start()
{
  assert(!started_);
  started_ = true;
  pthread_create(&pthreadId_, NULL, &startThread, this);
}

void Thread::join()
{
  assert(started_);
  pthread_join(pthreadId_, NULL);
}

void* Thread::startThread(void* obj)
{
  Thread* thread = static_cast<Thread*>(obj);
  thread->runInThread();
  return NULL;
}

void Thread::runInThread()
{
  tid_ = CurrentThread::tid();
  CurrentThread::t_threadName = name_.c_str();
  func_();
  CurrentThread::t_threadName = "finished";
}
