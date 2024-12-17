#include "./headers/http_server.h"
#include "./headers/http_sockets.h"
#include <iostream>
#include <stdexcept>

// http should serve different html routes
// /form - should server some kind of form and to creaet some kind of form post

int main(int argc, char *argv[]) {
  try {
    HttpServer http_server;
    HttpSocket http_server_socket;

    int server_socket = http_server_socket.createServerSocket();

    http_server.runServer(server_socket);

  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}
