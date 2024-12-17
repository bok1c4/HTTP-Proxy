#include "./headers/http_server.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

void HttpServer::runServer(int serverSocket) {
  if (serverSocket == -1) {
    std::cerr << "Invalid socket file descriptor!" << std::endl;
    throw std::runtime_error("Invalid socket fd for listening");
  }

  std::cout << "Server is running and listening on port..." << std::endl;

  while (true) {
    sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    int client_socket =
        accept(serverSocket, (struct sockaddr *)&client_address, &client_len);
    if (client_socket == -1) {
      std::cerr << "Failed to accept client connection" << std::endl;
      continue;
    }

    std::cout << "Client connected!" << std::endl;

    // Read the custom HTML file
    std::ifstream htmlFile(
        "/home/bok1c4/Projects/Web-Service/content/index.html");
    if (!htmlFile.is_open()) {
      std::cerr << "Failed to open HTML file!" << std::endl;
      close(client_socket);
      continue;
    }

    // Load the HTML content into a memory (string)
    std::ostringstream buffer;
    buffer << htmlFile.rdbuf();
    std::string htmlContent = buffer.str();

    // Build the HTTP response with the HTML content
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: " << htmlContent.size() << "\r\n"
             << "\r\n"
             << htmlContent;

    // Send the HTTP response to the client
    send(client_socket, response.str().c_str(), response.str().size(), 0);

    // Close the client socket
    close(client_socket);
  }
}
