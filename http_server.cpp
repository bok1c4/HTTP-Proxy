// so we need a class server that will have a contructor where we should provide
// the ip address

#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

class HttpServer {
public:
  void runServer(int serverSocket);
};

void runServer(int serverSocket) {
  if (serverSocket != 0) {
    std::cout << "Server socket should be null but it is: " << serverSocket;
    throw("Invalid socket fd value for listening on");
  }

  int listen_value = listen(serverSocket, 5);

  if (listen_value) {
    std::cout << "Listen value: " << listen_value;
    throw("Server failed at listening");
  }
};
