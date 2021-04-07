
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

// All client visible callbacks go here.

class TcpConnection;
typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef boost::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef boost::function<void (const TcpConnectionPtr&,
                              const char* data,
                              ssize_t len)> MessageCallback;
typedef boost::function<void (const TcpConnectionPtr&)> CloseCallback;
