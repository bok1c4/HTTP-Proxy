#include "../headers/proxy_server.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

ProxyServer::ProxyServer() {}

// setters
void ProxyServer::setPointToIp(std::string ip) { this->httpServerIP = ip; }
void ProxyServer::setPointToPort(int port) { this->httpServerPort = port; }

// getters
std::string ProxyServer::getPointedToIp() { return this->httpServerIP; }
int ProxyServer::getPointedToPort() { return this->httpServerPort; }

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

    HandleConnection(client_socket);

    close(client_socket);
  }
}

void ProxyServer::HandleConnection(int clientSocket) {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    std::cerr << "Failed to create server socket!" << std::endl;
    return;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(httpServerPort);
  inet_pton(AF_INET, httpServerIP.c_str(), &serverAddress.sin_addr);

  std::cout << "[Proxy] Connecting to backend server " << httpServerIP
            << " on port " << httpServerPort << std::endl;

  if (connect(serverSocket, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) == -1) {
    std::cerr << "[Proxy] Failed to connect to backend server!" << std::endl;
    close(serverSocket);
    return;
  }

  char buffer[4096];

  // Forward client request to server
  int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead > 0) {
    buffer[bytesRead] = '\0';
    // std::cout << "[Proxy] Request received: " << buffer << std::endl;
    send(serverSocket, buffer, bytesRead, 0);
  }

  // Forward server response to client
  bytesRead = recv(serverSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead > 0) {
    buffer[bytesRead] = '\0';
    // std::cout << "[Proxy] Response received: " << buffer << std::endl;
    send(clientSocket, buffer, bytesRead, 0);
  }

  close(serverSocket);
}
