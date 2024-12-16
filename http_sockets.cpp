
#include "./headers/http_sockets.h"
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

int HttpSocket::createServerSocket() {
  // Create server socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    throw std::runtime_error("Socket creation failed");
  }

  // Set up server address struct
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;   // IPv4
  serverAddress.sin_port = htons(8080); // Port 8080
  serverAddress.sin_addr.s_addr =
      INADDR_ANY; // Listen on any available IP address

  // Bind the socket to the address
  int binded_value = bind(serverSocket, (struct sockaddr *)&serverAddress,
                          sizeof(serverAddress));
  if (binded_value != 0) {
    std::cerr << "Bind error, binded value: " << binded_value << std::endl;
    throw std::runtime_error("Socket binding failed");
  }

  // Listen for incoming connections
  int listen_value = listen(serverSocket, 5);
  if (listen_value != 0) {
    std::cerr << "Listen error, listen value: " << listen_value << std::endl;
    throw std::runtime_error("Server failed to listen");
  }

  return serverSocket;
}
