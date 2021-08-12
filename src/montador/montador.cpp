#include "montador.h"

Assembler::Assembler(string fileName) {
  setInputFile(fileName);
}

bool Assembler::isNum(const string& line)
{
 for (const char c : line)
   if (!isdigit(c))
     return false;

 return true;
}

string Assembler::cleanString(string str){
  string new_str;
  for (char & c : str)
    if (isdigit(c) || isalpha(c))
      new_str.push_back(c);
  return new_str;
}

void Assembler::setInputFile(const string& fileName) {
  fstream file(fileName);
  if (!file.is_open()) {
    cout << "Erro ao abrir aquivo " << fileName << "!\n";
  } else {
    inputFile = &file;
    readInputFile();
  }
  // Depois de lido o arquivo não precisa mais ficar aberto!
  file.close();
}

void Assembler::readInputFile() {
  string line;
  while (std::getline(*inputFile, line))
    lines.push_back(line);

  ignoreComments();
  setLinesIntoTokens();
}

void Assembler::ignoreComments() {
  string aux;
  for (auto & i : lines) {
    string line = i;
    auto pos = line.find(';');
    if (pos != std::string::npos)
      i = line.substr(0, pos);
  }
}

void Assembler::setLinesIntoTokens() {
  for (const auto& line : lines){
    if (line.empty())
      continue;
    vector<string> aux;
    string token;
    istringstream ss(line);
    while(ss >> token) {
      auto in = cleanString(token);
      if (!in.empty()) {
        aux.push_back(in);
      }
    }

    if (!aux.empty())
      tokens.push_back(aux);
  }
}

void Assembler::buildSymbolTable() {
  symbolTable["HALT"] = 0;
  symbolTable["LOAD"] = 1;
  symbolTable["STORE"] = 2;
  symbolTable["READ"] = 3;
  symbolTable["WRITE"] = 4;
  symbolTable["COPY"] = 5;
  symbolTable["PUSH"] = 6;
  symbolTable["POP"] = 7;
  symbolTable["ADD"] = 8;
  symbolTable["SUB"] = 9;
  symbolTable["MUL"] = 10;
  symbolTable["DIV"] = 11;
  symbolTable["MOD"] = 12;
  symbolTable["AND"] = 13;
  symbolTable["OR"] = 14;
  symbolTable["NOT"] = 15;
  symbolTable["JUMP"] = 16;
  symbolTable["JZ"] = 17;
  symbolTable["JN"] = 18;
  symbolTable["CALL"] = 19;
  symbolTable["RET"] = 20;

  //PseudosInst
  symbolTable["WORD"] = 21;
  symbolTable["END"] = 22;
}

void Assembler::buildOperandsTable() {
  operandsTable["R0"] = 0;
  operandsTable["R1"] = 1;
  operandsTable["R2"] = 2;
  operandsTable["R3"] = 3;
}

void Assembler::firstFase() {
  ilc = 0;

  for (auto lineToken : tokens) {
    auto token = lineToken;
    try {
      symbolTable.at(token[0]); //Tenta acessar a operação
      ilc += (int) lineToken.size();
    } catch (const out_of_range& e){ // Senão conseguir é pq é um label
      string label = lineToken[0];
      symbolTable[label] = (int) symbolTable.size();
      pseudosTable.emplace(label, ilc);
      if (lineToken[1] == "WORD") //PseudInst
        ilc++; // Só o valor
      else if (lineToken[1] != "END")
        ilc += (int) (lineToken.size() - 1); // op + operands - label
    }

  }

}

int Assembler::codeGen(vector<string> token) {
  string op = token[0];
  int opCode = symbolTable[op];
  switch (opCode) {
  case 0: //HALT
    PC++;
    numbers.push_back(std::to_string(0));
    break;

  case 1: // LOAD R M : Reg[R] ← Mem[M + PC]
  case 2: // STORE R M : Mem[M + PC] ← Reg[R]
    PC++;
    numbers.push_back(std::to_string(opCode));

    // Registrador
    PC++;
    numbers.push_back(std::to_string(operandsTable[token[1]]));


    // Memoria
    PC++;
    if (isNum(token[2]) || pseudosTable.find((token[2])) == pseudosTable.end())
      numbers.push_back(token[2]);
    else
      numbers.push_back(std::to_string(pseudosTable[token[2]] - PC));
    break;

  case 3: // READ R : Reg[R] ← “valor lido”
  case 4: // WRITE R : “Imprime” Reg[R]
  case 6: // PUSH R : AP ← AP − 1; Mem[AP] ← Reg[R]
  case 7: // POP R : Reg[R] ← Mem[AP]; AP ← AP + 1
  case 15: // NOT R : Reg[R] ← NOT Reg[R] *
    PC++;
    numbers.push_back(std::to_string(opCode));

    // Registrador
    PC++;
    numbers.push_back(std::to_string(operandsTable[token[1]]));
    break;

  case 5: // COPY R1 R2 : Reg[R1] ← Reg[R2] *
  case 8: // ADD R1 R2 : Reg[R1] ← Reg[R1] + Reg[R2] *
  case 9: // SUB R1 R2 : Reg[R1] ← Reg[R1] - Reg[R2] *
  case 10: // MUL R1 R2 : Reg[R1] ← Reg[R1] * Reg[R2] *
  case 11: // DIV R1 R2 : Reg[R1] ← dividendo(Reg[R1] / Reg[R2]) *
  case 12: // MOD R1 R2 : Reg[R1] ← resto(Reg[R1] / Reg[R2]) *
  case 13: // AND R1 R2 : Reg[R1] ← Reg[R1] AND Reg[R2] *
  case 14: // OR R1 R2 : Reg[R1] ← Reg[R1] OR Reg[R2] *
    PC++;
    numbers.push_back(std::to_string(opCode));

    // Registradores
    PC++;
    numbers.push_back(std::to_string(operandsTable[token[1]]));
    PC++;
    numbers.push_back(std::to_string(operandsTable[token[2]]));
    break;

  case 16: // JUMP M : PC ← PC + M
  case 17: // JZ M : Se PEP[zero], PC ← PC + M
  case 18: // JN M : Se PEP[negativo], PC ← PC + M
  case 19: // CALL M : AP ← AP – 1; Mem[AP] ← PC; PC ← PC + M
    PC++;
    numbers.push_back(std::to_string(opCode));

    // Memoria
    PC++;
    if (isNum(token[1]) || pseudosTable.find((token[1])) == pseudosTable.end())
      numbers.push_back(token[1]);
    else
      numbers.push_back(std::to_string(pseudosTable[token[1]] - PC));

    break;
  case 20: // RET : PC ← Mem[AP]; AP ← AP + 1
    PC++;
    numbers.push_back(std::to_string(opCode));
    break;

  case 21: // WORD num
    PC++;
    numbers.push_back(token[1]);
    break;
  case 22: // END
    return -1;
    break;
  default: //Label: op [operand0] [operand1]
    if (opCode > 22) { //LABEL!
      vector<string> content(token.begin()+1, token.end());
      if (!content.empty())
        codeGen(content);
    } else {
      cout << "OpCode " << opCode << " não encontrado!";
      return -1;
    }
    break;
  }
  return 0;
}

void Assembler::secondFase() {
  PC = 0;
  int value;

  for (const auto& token : tokens) {
    value = codeGen(token);
    if (value < 0)
      break;
   }
}

void Assembler::writeOutputFile() {
  cout << "MV-EXE\n";
  cout << "\n";
  cout << numbers.size() << " " << 100 << " " << AP << " " << 100 << "\n";
  cout << "\n";
  for (const auto& number : numbers)
    cout << number << " ";
  cout << "\n";
}

void Assembler::writeObjectFile() {
  /// Type
  if (pseudosTable.find("main") != pseudosTable.end())
    cout << "main_func " << ilc << "\n";
  else
    cout << "aux_func " << ilc << "\n";

  /// Numbers + Symbols
  cout << numbers.size()  << "\n";
  for (const auto& number : numbers)
    cout << number << " ";
  cout << "\n";

  /// Symbols
  cout << symbolTable.size() - 23 << "\n";
  for (const auto& item : symbolTable)
    if (item.second > 22)
      cout << item.first << " " << item.second << "\n";

  /// Pseudos
  cout << pseudosTable.size() << "\n";
  for (const auto& item : pseudosTable)
    cout << item.first << " " << item.second << "\n";
}

void Assembler::assemble() {
  buildSymbolTable();
  buildOperandsTable();
  firstFase();
  secondFase();
  writeObjectFile();
  //writeOutputFile();
}