#include "./headers/http_server.h"
#include "./headers/http_sockets.h"
#include "./headers/proxy_server.h"
#include <iostream>
#include <stdexcept>
#include <thread>

int main(int argc, char *argv[]) {
  try {
    HttpServer http_server;
    HttpSocket sockets;

    // HTTP Server
    int server_socket = sockets.createServerSocket();

    // Run HTTP server on a separate thread
    std::thread httpThread([&]() { http_server.runServer(server_socket); });

    // Proxy Server
    ProxyServer proxy_server("127.0.0.1", 8080); // HTTP server IP and port
    int proxy_socket = sockets.createProxySocket("127.0.0.1", 3128);

    // Start Proxy Server
    proxy_server.Start(proxy_socket);

    // Join threads (wait for HTTP server)
    httpThread.join();

  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}

