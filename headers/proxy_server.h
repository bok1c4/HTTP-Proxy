#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class ProxyServer {
public:
  ProxyServer();
  void Start();

  // setters
  void setPointToIp(std::string httpServerIP);
  void setPointToPort(int httpServerPort);
  void setSocket(int socket);

  // getters
  std::string getPointedToIp();
  int getPointedToPort();
  int getSocket();

private:
  void HandleConnection(int clientSocket);
  std::string httpServerIP;
  int httpServerPort;
  int proxy_socket;
};

#endif
