#ifndef LIGADOR_H
#define LIGADOR_H

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Linker {

private:
  map<string, int> pseudosTableAuxFiles;
  vector<string> numbersAuxFiles;

  map<string, int> pseudosTableMainFile;
  vector<string> numbersMainFile;

  fstream *inputFile;

  vector<string> lines;
  vector<string> tokens;
  vector<string> numbersVector;
  map<string, int> pseudosTable;

  bool foundMainSymbol = false;

  int PC = 0;
  int ilc = 0;
  vector<int> ilcVec;

public:
  Linker();
  ~Linker();

  // Alloc
  void setInputFiles(const vector<string> &fileNames);
  void readInputFile();
  void setLinesIntoTokens();
  void setTokensIntoMaps(map<string, int> &pseudos,
                         vector<string> &numbers, bool isMain);
  // Realloc + Link
  void Realloc();
  void writeOutputFile();
  void Zelda(const vector<string> &fileNames);

  // Auxiliary Functions
  string cleanString(string str);
  bool isNum(const string &line);
};

#endif
