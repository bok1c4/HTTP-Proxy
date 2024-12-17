#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class ProxyServer {
public:
  ProxyServer(std::string httpServerIP, int httpServerPort);
  void Start(int proxySocket);

private:
  void HandleConnection(int clientSocket);
  std::string httpServerIP;
  int httpServerPort;
};

#endif

