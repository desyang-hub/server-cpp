all: client server test epoll_usage

client:
	g++ -o client client.cc

server:
	g++ -o server server.cc Socket.cc Epoll.cc InetAddress.cc Channel.cc EventLoop.cc Acceptor.cc TcpServer.cc Connection.cc Buffer.cc ThreadPool.cc

test:
	g++ -o test test.cc

epoll_usage:
	g++ -o epoll_usage epoll_usage.cc

clean:
	rm -rf client server test epoll_usage