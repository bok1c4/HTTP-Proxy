#ifndef HTTP_SOCKETS_H
#define HTTP_SOCKETS_H

#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

class HttpSocket {
public:
  int createServerSocket();
  int createProxySocket(std::string ipaddr, int port);
};

#endif
