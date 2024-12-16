// first we have to bind the server to listen on the ip address with proper port
// then the server needs to enter the loop for accepting the requests
// based on the request we need to process the response

#include "http_server.cpp"
#include "http_sockets.cpp"

// maybe we should have the construct the socket
// then we return the serverSocket file description
// and then we should pass that fd to the listen function of the http_server

int main(int argc, char *argv[]) {
  HttpServer http_server;
  HttpSocket http_server_socket;

  int server_socket = http_server_socket.createServerSocket();

  http_server.runServer(server_socket);

  return 0;
}
