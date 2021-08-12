#include "ligador.h"
#include <iostream>

int main(int argc, char *argv[]) {
  string fileName;
  vector<string> fileNames;
  if (argc < 2) {
    cout << "Usage ligador [file]\n";
    return 1;
  } else {
    for (int i = 1; i < argc; i++) {
      fileName = argv[i];
      fileNames.push_back(fileName);
    }
  }

  auto *Link = new Linker();
  Link->Zelda(fileNames);

  return 0;
}
