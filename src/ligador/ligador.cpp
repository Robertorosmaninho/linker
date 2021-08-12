#include "ligador.h"

Linker::Linker() {}

Linker::~Linker() {
  if (inputFile->is_open())
    inputFile->close();

  pseudosTable.clear();

  pseudosTableAuxFiles.clear();
  numbersAuxFiles.clear();

  pseudosTableMainFile.clear();
  numbersMainFile.clear();

  lines.clear();
  tokens.clear();
  numbersVector.clear();
}

string Linker::cleanString(string str) {
  string new_str;
  for (char &c : str)
    if (isdigit(c) || isalpha(c))
      new_str.push_back(c);
  return new_str;
}

bool Linker::isNum(const string &line) {
  for (const char c : line)
    if (!isdigit(c))
      return false;

  return true;
}

void Linker::setInputFiles(const vector<string> &fileNames) {
  for (auto fileName : fileNames) {
    fstream file(fileName);
    if (!file.is_open()) {
      cout << "Erro ao abrir aquivo " << fileName << "!\n";
    } else {
      inputFile = &file;
      readInputFile();
    }
    // Depois de lido o arquivo não precisa mais ficar aberto!
    inputFile->close();
  }

  numbersVector.insert(numbersVector.end(),
                       std::make_move_iterator(numbersMainFile.begin()),
                       std::make_move_iterator(numbersMainFile.end()));
  ilc = numbersVector.size();
  numbersVector.insert(numbersVector.end(),
                       std::make_move_iterator(numbersAuxFiles.begin()),
                       std::make_move_iterator(numbersAuxFiles.end()));

  pseudosTable.merge(pseudosTableMainFile);

  int i = 0, j = 0;

  for (auto &item : pseudosTableAuxFiles) {
    if (i < ilcVec[j]) {
      item.second += ilc;
    } else if (i + 1 == ilcVec[j]) {
      ilc += ilcVec[j];
      ilcVec[j + 1] += ilcVec[j];
      j++;
    }
  }
  pseudosTable.insert(pseudosTableAuxFiles.begin(),
                      pseudosTableAuxFiles.end());
}

void Linker::readInputFile() {
  string line;
  while (std::getline(*inputFile, line))
    lines.push_back(line);


  setLinesIntoTokens();
  if (foundMainSymbol)
    setTokensIntoMaps(pseudosTableMainFile, numbersMainFile, true);
  else
    setTokensIntoMaps(pseudosTableAuxFiles, numbersAuxFiles, false);

  tokens.clear();
  lines.clear();
}

void Linker::setLinesIntoTokens() {
  // 0 -> size of nums
  // 1 -> vector of nums
  // 2 -> size of pseudos
  // 3 -> vector of pseudos

  for (const auto &line : lines) {
    if (line.empty())
      continue;
    string token;
    istringstream ss(line);
    while (ss >> token) {
      auto in = cleanString(token);
      if (!in.empty()) {
        if (in == "mainfunc")
          foundMainSymbol = true;
        else if (in == "auxfunc")
          foundMainSymbol = false;
        else
          tokens.push_back(in);
      }
    }
  }
}

void Linker::setTokensIntoMaps(map<string, int> &pseudos,
                               vector<string> &numbers, bool isMain) {
  int inc = isMain ? 0 : ilc;

  int num_size = stoi(tokens[0]);
  int idx = num_size + 1;

  for (int i = 1; i < idx; i++)
    numbers.push_back(tokens[i]);

  for (int i = idx + 1; i < tokens.size(); i += 2)
    pseudos[tokens[i]] = stoi(tokens[i + 1]) + inc;
  if (!isMain)
    ilcVec.push_back(num_size);
}

void Linker::Realloc() {

  for (auto &token : numbersVector) {
    PC++;
    if (!isNum(token))
      token = to_string(pseudosTable[token] - PC);
  }
}

void Linker::writeOutputFile() {
  cout << "MV-EXE\n";
  cout << "\n";
  cout << numbersVector.size() << " " << 100 << " " << 999 << " " << 100 << "\n";
  cout << "\n";

  for (auto token : numbersVector)
    cout << token << " ";
  cout << "\n";
}

void Linker::Zelda(const vector<string> &fileNames) {// Link or Zelda? :p
  setInputFiles(fileNames);
  Realloc();
  writeOutputFile();
}