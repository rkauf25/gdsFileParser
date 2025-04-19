#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Removal.h"

#define BOUNDARY_START 0x08
#define PATH_START 0x09
#define ELEMENT_END 0x11

static short
readShort(std::ifstream &input, std::stringstream &ss)
{
  unsigned char p[2];
  input.read((char *)&p, 2);
  ss.write(reinterpret_cast<char*>(p), sizeof(p));
  return ((p[0]<<8) | p[1]);
}

static short
getShort(std::ifstream &input)
{
  unsigned char p[2];
  input.read((char *)&p, 2);
  return ((p[0]<<8) | p[1]);
}

static char 
readChar(std::ifstream &input, std::stringstream &ss)
{
  char p;
  input.read(&p, sizeof(p));
  ss << p;
  return p;
}

static char 
getChar(std::ifstream &input)
{
  char p;
  input.read(&p, sizeof(p));
  return p;
}

static void 
placeInOutputFile(std::ifstream &input, std::ofstream &output, std::stringstream &ss, short recordSize)
{
  output << ss.str();

  std::vector<char> recordLeft(recordSize-3);
  input.read(recordLeft.data(), recordSize-3);
  output.write(recordLeft.data(), recordSize-3);
}

static void 
skipRecord(std::ifstream &input, short recordSize)
{
  // The first 3 bytes of the record 
  // {2-byte record size, 1-byte record type} already read in
  input.ignore(recordSize-3);
}

static void 
handleElementRemoval(std::ifstream &inputFile) 
{
  while (true) {
    
    int recordSize = getShort(inputFile);
    int recordType = getChar(inputFile);

    if (recordSize == 0) {
      std::cerr << "ERROR: RECORD SIZE IS 0 WHILE STILL REMOVING PATH. ENDING\n";
      exit(1);
    }
    
    skipRecord(inputFile, recordSize);

    if (recordType == ELEMENT_END) break;
  }
}

void
createRemovedOutputFile(
  const std::string inputGDSFilePath,
  const std::string outputGDSFilePath,
  const std::vector<int> &pathsToRemove,
  const std::vector<int> &boundariesToRemove)
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
    
    short recordSize = readShort(inputFile, binarySStream);
    int recordType = readChar(inputFile, binarySStream);

    if (recordSize == 0) break;

    if ((pathsToRemoveVecIdx < pathsToRemove.size()) && (recordType == PATH_START)) {
      if (pathsToRemove[pathsToRemoveVecIdx] == pathsFound) {
        skipRecord(inputFile, recordSize);
        handleElementRemoval(inputFile);
        ++pathsFound;
        ++pathsToRemoveVecIdx;
        continue;
      }
      ++pathsFound;
    }

    if ((boundariesToRemoveVecIdx < boundariesToRemove.size()) && (recordType == BOUNDARY_START)) {
      if (boundariesToRemove[boundariesToRemoveVecIdx] == boundariesFound) {
        skipRecord(inputFile, recordSize);
        handleElementRemoval(inputFile);
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