#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Callbacks.h"
#include "TcpConnection.h"
#include "EventLoopThreadPool.h"
#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

class Acceptor;
class EventLoop;

class TcpServer : boost::noncopyable
{
 public:
  TcpServer(EventLoop* loop, const InetAddress& listenAddr);
  ~TcpServer();  // force out-line dtor, for scoped_ptr members.

  /// Starts the server if it's not listenning.
  ///
  /// It's harmless to call it multiple times.
  /// Thread safe.
  void start();

  /// Set connection callback.
  /// Not thread safe.
  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }

  /// Set message callback.
  /// Not thread safe.
  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }
  void setThreadNum(int numThreads);
 private:
  typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;
  /// Not thread safe, but in loop
  void newConnection(int sockfd, const InetAddress& peerAddr);
  void removeConnection(const TcpConnectionPtr& conn);
  void removeConnectionInLoop(const TcpConnectionPtr& conn);

  EventLoop* loop_;  // the acceptor loop
  const std::string name_;
  boost::scoped_ptr<Acceptor> acceptor_; // avoid revealing Acceptor
  boost::scoped_ptr<EventLoopThreadPool> threadPool_;

  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  bool started_;
  int nextConnId_;  // always in loop thread
  ConnectionMap connections_;
};

#endif // TCPSERVER_H
