#include <vector>
#include <string>

void
remove_path(
  int layer,
  std::string pathName,
  std::vector<int> &removed_paths,
  std::vector<int> &removed_vias);

void 
createRemovedOutputFile(
  const std::string inputGDSFilePath,
  const std::string outputGDSFilePath,
  const std::vector<int> &pathsToRemove,
  const std::vector<int> &boundariesToRemove);