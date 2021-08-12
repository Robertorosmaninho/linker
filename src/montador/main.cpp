#include <iostream>

#include "montador.h"

int main(int argc, char *argv[]) {
  string fileName;
  if (argc != 2) {
    cout << "Usage montador [file]\n";
    return 1;
  } else {
    fileName = argv[1];
  }

  auto *avengers = new Assembler(fileName);
  avengers->assemble();
  return 0;
}