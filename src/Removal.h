#include <vector>
#include <string>

std::pair<std::vector<int>, std::vector<int>> remove_path(int layer, std::string pathName);

void createRemovedOutputFile(
  const std::string inputGDSFilePath,
  const std::string outputGDSFilePath,
  const std::vector<int> pathsToRemove,
  const std::vector<int> boundariesToRemove);