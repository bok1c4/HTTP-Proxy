#ifndef UTILS_H
#define UTILS_H

#include <queue>
#include <unordered_set>

namespace utils {
std::queue<int> generatePorts(int numHops,
                              const std::unordered_set<int> &reservedPorts);
void printPorts(std::queue<int> queue);

} // namespace utils

#endif

