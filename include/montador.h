#ifndef MONTADOR_H
#define MONTADOR_H

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// TODO: structs, classes, e outras definições.
// Vamos precisar d eum arquivo de entrada e um de saída
// Uma estrutura de dados com o resultado que estamos construindo, isso é
// importante principalmante para uma segunda passagem marcando os labels
// precisaremos primeiro construir as traduções diretas em um map para depois
// aplicarmos

class Assembler {

private:
  map<string, int> symbolTable;
  map<string, int> pseudosTable;
  map<string, int> operandsTable;

  fstream *inputFile;

  vector<string> lines;
  vector<vector<string>> tokens;
  vector<string> numbers;

  int PC;      //ILC? --> Contador de programas: contém o endereço da próxima
               // instrução a ser executada;
  int AP = 999;// Apontador da pilha: aponta para o elemento no topo da pilha;
  int ilc;

public:
  Assembler(string fileName);
  ~Assembler() = default;

  void setInputFile(const string &fileName);

  void buildSymbolTable();
  void buildOperandsTable();
  void readInputFile();
  void ignoreComments();
  void setLinesIntoTokens();
  void writeObjectFile();
  void writeOutputFile();
  void firstFase();
  void secondFase();
  void assemble();

  static bool isNum(const string &num);
  string cleanString(string str);
  int codeGen(vector<string> token);
};

#endif
