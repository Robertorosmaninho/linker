#ifndef LIGADOR_H
#define LIGADOR_H

#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cctype>
#include <map>
#include <sstream>
#include <istream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class Linker {

private:
  map<string, int> symbolTable;
  map<string, int> pseudosTable;
  map<string, int> operandsTable;

  map<string, int> symbolTableAuxFiles;
  map<string, int> pseudosTableAuxFiles;
  vector<string> numbersAuxFiles;

  map<string, int> symbolTableMainFile;
  map<string, int> pseudosTableMainFile;
  vector<string> numbersMainFile;

  fstream *inputFile;

  vector<string> lines;
  vector<string> tokens;
  vector<string> numbersVector;

  bool foundMainSymbol = false;

  int PC = 0;
  int ilc = 0;
  vector<int> ilcVec;

public:
  Linker(const vector<string>& fileNames);
  ~Linker();

  void setInputFiles(const vector<string>& fileNames);
  void readInputFile();
  void ignoreComments();
  void setLinesIntoTokens();
  void setTokensIntoMaps(map<string, int> &symbol, map<string, int> &pseudos,
                         vector<string> &numbers, bool isMain);
  void Realloc();
  void writeOutputFile();

  string cleanString(string str);
  bool isNum(const string& line);

};

#endif
