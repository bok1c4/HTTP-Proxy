#include "./headers/utils.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <unordered_set>

namespace utils {
std::queue<int> generatePorts(int numHops,
                              const std::unordered_set<int> &reservedPorts) {
  std::unordered_set<int> usedPorts =
      reservedPorts; // Start with reserved ports
  std::queue<int> hopQueue;

  std::srand(std::time(0)); // Seed for random port generation

  while (hopQueue.size() < numHops) {
    int port = std::rand() % 65536; // Generate port between 0 and 65535

    if (usedPorts.find(port) == usedPorts.end()) { // Port is not in use
      usedPorts.insert(port);                      // Mark as used
      hopQueue.push(port);                         // Add to the queue
    }
  }

  return hopQueue;
}

void printPorts(std::queue<int> queue) {
  std::queue<int> tempQueue = queue;

  while (!tempQueue.empty()) {
    std::cout << tempQueue.front() << " ";
    tempQueue.pop();
  }

  std::cout << std::endl;
}

} // namespace utils

