#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class ProxyServer {
public:
  ProxyServer();
  void Start(int proxySocket);
  void setPointToIp(std::string httpServerIP);
  void setPointToPort(int httpServerPort);
  std::string getPointedToIp();
  int getPointedToPort();

private:
  void HandleConnection(int clientSocket);
  std::string httpServerIP;
  int httpServerPort;
};

#endif
