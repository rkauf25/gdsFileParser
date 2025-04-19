#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Removal.h"

#define BOUNDARY_START 0x08
#define PATH_START 0x09
#define ELEMENT_END 0x11

short T_readShort(std::ifstream &input, std::stringstream &ss)
{
  unsigned char p[2];
  input.read((char *)&p, 2);
  ss.write(reinterpret_cast<char*>(p), sizeof(p));
    return ((p[0]<<8) | p[1]);
}

char T_readChar(std::ifstream &input, std::stringstream &ss)
{
  char p;
  input.read(&p, sizeof(p));
  ss << p;
  return p;
}

void placeInOutputFile(std::ifstream &input, std::ofstream &output, std::stringstream &ss, short recordSize)
{
  // std::cout << "PLACING TO OUTPUT FILE\n";
  output << ss.str();

  std::vector<char> recordLeft(recordSize-3);
  input.read(recordLeft.data(), recordSize-3);
  output.write(recordLeft.data(), recordSize-3);
}

void skipRecord(std::ifstream &input, short recordSize)
{
  // std::cout << "SKIPPING RECORD\n";
  std::vector<char> recordLeft(recordSize-3);
  input.read(recordLeft.data(), recordSize-3);
}

void handleElementRemoval(std::ifstream &inputFile, short recordSize) {
  
  // std::cout << "HANDLING ELEMENT REMOVAL\n";

  std::stringstream fakeSstream(std::ios::binary | std::ios::out);
  
  std::vector<char> recordLeft(recordSize-3);
  inputFile.read(recordLeft.data(), recordSize-3);

  bool endFound = false;

  while (!endFound) {
    
    recordSize = T_readShort(inputFile, fakeSstream);
    int recordType = T_readChar(inputFile, fakeSstream);
    
    // std::cout << "Size: " << recordSize << std::endl;
    // std::cout << "Record Type: " << recordType << std::endl;

    if (recordSize == 0) {
      std::cout << "ERROR: RECORD SIZE IS 0 WHILE STILL REMOVING PATH. ENDING\n";
      exit(1);
    }

    if (recordType == ELEMENT_END) endFound = true;
    
    skipRecord(inputFile, recordSize);
  }

  // std::cout << "END FOUND. DONE\n";
}

void handleBoundaryRemoval(std::ifstream &input) {
  // std::cout << "HANDLING PATH_START REMOVAL\n";
}

void createRemovedOutputFile(
  const std::string inputGDSFilePath,
  const std::string outputGDSFilePath,
  const std::vector<int> pathsToRemove,
  const std::vector<int> boundariesToRemove)
{
  std::ifstream inputFile(inputGDSFilePath, std::ios::binary);
  if (!inputFile.is_open()) {
    std::cout << "Input GDS File not able to be read in\n";
    exit(1);
  }

  std::ofstream outputFile(outputGDSFilePath, std::ios::binary);
  if (!outputFile.is_open()) {
    std::cout << "Output GDS File not able to be read in\n";
    exit(1);
  }

  int pathsToRemoveVecIdx = 0;
  int boundariesToRemoveVecIdx = 0;

  int pathsFound = 0;
  int boundariesFound = 0;

  int i = 0;

  while (true) {
        
    // Temporary sstream to hold data to potentially place in output
    std::stringstream binarySStream(std::ios::binary | std::ios::out);
    
    short recordSize = T_readShort(inputFile, binarySStream);
    int recordType = T_readChar(inputFile, binarySStream);
    
    // std::cout << "Size: " << recordSize << std::endl;
    // std::cout << "Record Type: " << recordType << std::endl;

    if (recordSize == 0) {
      // std::cout << "STILL BROKEN AS I'M NOT SURE HOW THE END OF THE FILE IS SUPPOSE TO BEHAVE\n";
      // std::cout << "RECORD SIZE IS 0. ENDING\n";
      break;
    }

    if ((pathsToRemoveVecIdx < pathsToRemove.size()) && (recordType == PATH_START)) {
      if (pathsToRemove[pathsToRemoveVecIdx] == pathsFound) {
        handleElementRemoval(inputFile, recordSize);
        ++pathsFound;
        ++pathsToRemoveVecIdx;
        continue;
      }
      ++pathsFound;
    }

    if ((boundariesToRemoveVecIdx < boundariesToRemove.size()) && (recordType == BOUNDARY_START)) {
      if (boundariesToRemove[boundariesToRemoveVecIdx] == boundariesFound) {
        handleElementRemoval(inputFile, recordSize);
        ++boundariesFound;
        ++boundariesToRemoveVecIdx;
        continue;
      }
      ++boundariesFound;
    }
    
    placeInOutputFile(inputFile, outputFile, binarySStream, recordSize);
  }

  inputFile.close();
  outputFile.close();
}