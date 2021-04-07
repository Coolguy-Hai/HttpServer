#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H


#include "Callbacks.h"
#include "InetAddress.h"

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

class Channel;
class EventLoop;
class Socket;

///
/// TCP connection, for both client and server usage.
///
class TcpConnection : boost::noncopyable,
                      public boost::enable_shared_from_this<TcpConnection>
{
 public:
  /// Constructs a TcpConnection with a connected sockfd
  ///
  /// User should not create this object.
  TcpConnection(EventLoop* loop,
                const std::string& name,
                int sockfd,
                const InetAddress& localAddr,
                const InetAddress& peerAddr);
  ~TcpConnection();

  EventLoop* getLoop() const { return loop_; }
  const std::string& name() const { return name_; }
  const InetAddress& localAddress() { return localAddr_; }
  const InetAddress& peerAddress() { return peerAddr_; }
  bool connected() const { return state_ == kConnected; }

  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }

  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }

  void setCloseCallback(const CloseCallback& cb){
    closeCallback_ = cb;
  }
  void connectDestroyed();
  /// Internal use only.

  // called when TcpServer accepts a new connection
  void connectEstablished();   // should be called only once

 private:
  enum StateE { kConnecting, kConnected, kDisconnected,};

  void setState(StateE s) { state_ = s; }
  //handleRead will check the return of read(2), and do handleWrite(), handleClose() or handleError()
  void handleRead();
  void handleWrite();
  void handleClose();
  void handleError();

  EventLoop* loop_;
  std::string name_;
  StateE state_;  // FIXME: use atomic variable
  // we don't expose those classes to client.
  boost::scoped_ptr<Socket> socket_;
  boost::scoped_ptr<Channel> channel_;
  InetAddress localAddr_;
  InetAddress peerAddr_;
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  CloseCallback closeCallback_;
};

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;


#endif // TCPCONNECTION_H
