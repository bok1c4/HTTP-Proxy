#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>

class HttpServer {
public:
  void runServer(int serverSocket);
};

#endif
