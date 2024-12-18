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
    std::cerr << "Invalid HTTP socket file descriptor!" << std::endl;
    throw std::runtime_error("Invalid HTTP socket fd for listening");
  }

  std::cout << "HTTP Server is running and listening on port..." << std::endl;

  while (true) {
    sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    int client_socket =
        accept(serverSocket, (struct sockaddr *)&client_address, &client_len);
    if (client_socket == -1) {
      std::cerr << "HTTP Server: Failed to accept proxy connection"
                << std::endl;
      continue;
    }

    std::cout << "HTTP Server: Proxy connected!" << std::endl;

    char requestBuffer[4096];
    int bytesRead =
        recv(client_socket, requestBuffer, sizeof(requestBuffer) - 1, 0);

    if (bytesRead < 0) {
      std::cerr << "HTTP Server: Failed to read proxy request" << std::endl;
      close(client_socket);
      continue;
    }

    requestBuffer[bytesRead] = '\0';

    std::string request(requestBuffer);
    std::string requestedPath = "/";

    // std::cout << request;

    size_t pos = request.find("GET ");

    if (pos != std::string::npos) {
      size_t start = pos + 4;
      size_t end = request.find(" ", start);
      requestedPath = request.substr(start, end - start);
    }

    std::string projectPath = "/home/bok1c4/Projects/Web-Service/content/";
    std::string file = "";

    if (requestedPath == "/") {
      file = "index.html";
    } else if (requestedPath == "/about") {
      file = "about.html";
    } else if (requestedPath == "/contact") {
      file = "contact.html";
    } else {
      file = "404.html";
    }

    projectPath += file;

    // Read the custom HTML file
    std::ifstream htmlFile(projectPath);
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
