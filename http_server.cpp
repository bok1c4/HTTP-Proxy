// so we need a class server that will have a contructor where we should provide
// the ip address

#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

class HttpServer {
public:
  int runServer() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // ipv4
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int binded_value = bind(serverSocket, (struct sockaddr *)&serverAddress,
                            sizeof(serverAddress));

    if (binded_value != 0) {
      std::cout << "Bind value: " << binded_value;
      throw("Socket not binded properly");

      return -1;
    }

    int listen_value = listen(serverSocket, 5);

    if (listen_value) {
      std::cout << "Listen value: " << listen_value;
      throw("Server failed at listening");

      return -1;
    }

    return serverSocket;
  };
};
