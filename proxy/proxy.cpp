#include "../headers/proxy_server.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

ProxyServer::ProxyServer(std::string httpServerIP, int httpServerPort)
    : httpServerIP(httpServerIP), httpServerPort(httpServerPort) {}

void ProxyServer::Start(int proxy_socket) {
  if (proxy_socket == -1) {
    std::cerr << "Invalid PROXY socket file descriptor!" << std::endl;
    throw std::runtime_error("Invalid PROXY socket fd for listening");
  }

  std::cout << "Proxy is running and listening on port..." << std::endl;

  while (true) {
    sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    int client_socket =
        accept(proxy_socket, (struct sockaddr *)&client_address, &client_len);
    if (client_socket == -1) {
      std::cerr << "PROXY Server: Failed to accept client connection"
                << std::endl;
      continue;
    }

    std::cout << "PROXY Server: Client connected!" << std::endl;

    // Handle the connection
    HandleConnection(client_socket);

    close(client_socket);
  }
}

void ProxyServer::HandleConnection(int clientSocket) {
  // 1. Connect to the backend HTTP server
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    std::cerr << "Failed to create server socket!" << std::endl;
    return;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(httpServerPort);
  inet_pton(AF_INET, httpServerIP.c_str(), &serverAddress.sin_addr);

  if (connect(serverSocket, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) == -1) {
    std::cerr << "Failed to connect to HTTP server!" << std::endl;
    close(serverSocket);
    return;
  }

  // 2. Forward client request to HTTP server
  char buffer[4096];
  int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead > 0) {
    buffer[bytesRead] = '\0'; // Null-terminate the request
    std::cout << "Proxy received request:\n" << buffer << std::endl;

    // Send the request to the HTTP server
    send(serverSocket, buffer, bytesRead, 0);
  }

  // 3. Forward HTTP server response back to client
  bytesRead = recv(serverSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead > 0) {
    buffer[bytesRead] = '\0'; // Null-terminate the response
    std::cout << "Proxy received response:\n" << buffer << std::endl;

    // Send the response to the client
    send(clientSocket, buffer, bytesRead, 0);
  }

  // Close the connection to the server
  close(serverSocket);
}

