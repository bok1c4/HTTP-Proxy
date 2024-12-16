#include "./headers/http_server.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void HttpServer::runServer(int serverSocket) {
  if (serverSocket == -1) {
    std::cerr << "Invalid socket file descriptor!" << std::endl;
    throw std::runtime_error("Invalid socket fd for listening");
  }

  std::cout << "Server is running and listening on socket " << serverSocket
            << std::endl;

  while (true) {
    sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    // Accept a new connection (this will block until a client connects)
    int client_socket =
        accept(serverSocket, (struct sockaddr *)&client_address, &client_len);
    if (client_socket == -1) {
      std::cerr << "Failed to accept client connection" << std::endl;
      continue; // Skip this loop iteration if accepting the connection failed
    }

    std::cout << "Client connected!" << std::endl;

    // Send a simple HTTP response to the client
    const char *response =
        "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
    send(client_socket, response, strlen(response), 0);

    // Optionally, you could handle more client requests here, process them,
    // or log information for each connection.

    // Close the client socket after the response is sent
    close(client_socket);
  }
}
