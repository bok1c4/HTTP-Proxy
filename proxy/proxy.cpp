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
void ProxyServer::setProxyPort(int port) { this->proxy_port = port; }

void ProxyServer::setSocket(int socket) { this->proxy_socket = socket; }

// getters
std::string ProxyServer::getPointedToIp() { return this->httpServerIP; }
int ProxyServer::getPointedToPort() { return this->httpServerPort; }
int ProxyServer::getSocket() { return this->proxy_socket; }
int ProxyServer::getProxyPort() { return this->proxy_port; }

void ProxyServer::Start() {
  if (this->proxy_socket == -1) {
    std::cerr << "Invalid PROXY socket file descriptor!" << std::endl;
    throw std::runtime_error("Invalid PROXY socket fd for listening");
  }

  int listening_port = this->getProxyPort();
  int points_to = this->getPointedToPort();

  std::cout << "Proxy: " << listening_port << "->" << points_to << std::endl;

  while (true) {
    sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    int client_socket = accept(this->proxy_socket,
                               (struct sockaddr *)&client_address, &client_len);
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
  int points_to = this->getPointedToPort();
  std::string httpServerIP = this->httpServerIP;

  int outgoingSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (outgoingSocket == -1) {
    std::cerr << "[Proxy] Failed to create socket for outgoing connection!"
              << std::endl;
    close(clientSocket);
    return;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(points_to);
  inet_pton(AF_INET, httpServerIP.c_str(), &serverAddress.sin_addr);

  std::cout << "[Proxy] Connecting to backend server " << httpServerIP
            << " on port " << points_to << std::endl;

  if (connect(outgoingSocket, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) == -1) {
    std::cerr << "[Proxy] Failed to connect to backend server!" << std::endl;
    close(outgoingSocket);
    close(clientSocket);
    return;
  }

  char buffer[4096];
  while (true) {
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0)
      break;
    send(outgoingSocket, buffer, bytesRead, 0);

    bytesRead = recv(outgoingSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0)
      break;
    send(clientSocket, buffer, bytesRead, 0);
  }

  close(outgoingSocket);
  close(clientSocket);
}
