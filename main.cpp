#include "./headers/http_server.h"
#include "./headers/http_sockets.h"
#include "./headers/proxy_server.h"
#include "./headers/utils.h"
#include <iostream>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <thread>
#include <unordered_set>
#include <vector>

int main(int argc, char *argv[]) {
  try {
    HttpServer http_server;
    HttpSocket sockets;

    int numHops = 0;

    std::cout << "Enter number of proxy hopping you want: (1-5): " << std::endl;
    while (!(std::cin >> numHops) || numHops < 1 || numHops > 5) {
      std::cout
          << "Invalid input. Please enter a number of hops between 1 and 5: ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "You entered: " << numHops << std::endl;

    std::unordered_set<int> reservedPorts = {8080};

    std::queue<int> ports = utils::generatePorts(numHops, reservedPorts);
    utils::printPorts(ports);

    int server_socket = sockets.createServerSocket();

    std::thread httpThread([&]() { http_server.runServer(server_socket); });

    std::vector<std::thread> proxyThreads;

    int prev_port = -1;

    ProxyServer proxy_server;

    while (!ports.empty()) {
      int port = ports.front();
      ports.pop();

      // pa ono overwrituje svaki proxy_socket i onda zapravo pravi samo
      // poslednji umesto svakog

      int proxy_socket;
      if (ports.empty()) {
        proxy_server.setPointToIp("127.0.0.1");
        proxy_server.setPointToPort(8080);
        proxy_socket = sockets.createProxySocket("127.0.0.1", port);
      } else {
        int next_port = ports.front();
        proxy_server.setPointToIp("127.0.0.1");
        proxy_server.setPointToPort(next_port);

        proxy_socket = sockets.createProxySocket("127.0.0.1", port);
        proxy_server.setSocket(proxy_socket);
      }

      proxyThreads.push_back(std::thread(
          [&proxy_server, proxy_socket]() { proxy_server.Start(); }));
    }

    for (auto &t : proxyThreads) {
      t.join();
    }

    httpThread.join();

  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}
