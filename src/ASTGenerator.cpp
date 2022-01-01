#include "ASTGenerator.hpp"
#include "llvm/Support/raw_ostream.h"

using namespace kal;
using namespace llvm;

static int getnextchar() {
    return getchar();
}

static int getnextchar(std::ifstream &inputFileStrm) {
    return inputFileStrm.get();
}

void ASTGenerator::ReplLoop() {
  do {
    fprintf(stderr, "kal> ");
    EatStdioToken();
  }while (true);
}

void ASTGenerator::EatFile(std::ifstream &inputFileStrm){
    // Skip any whitespace.
    while (isspace(lastChar))
        lastChar = getnextchar(inputFileStrm);

    lastChar = getnextchar(inputFileStrm);    
    // CurrTok = Lexer::getTok(LastChar)
    // then pass CurrTok to Parser to build AST
    // if LastChar not EOF call this function again

}

bool ASTGenerator::EatStdioToken() {
    // Skip any whitespace.
    while (isspace(lastChar))
        lastChar  = getnextchar();

    lastChar  = getnextchar();

    // CurrTok = Lexer::getTok(LastChar)
    // then pass CurrTok to Parser to build AST
    // if ...

    return true;
}

ASTGenerator::ASTGenerator(std::string inputFilename) {

    if (inputFilename == "-")
    {
        errs() << "No file name supplied.\nSwitching to Interactive Shell Mode.\n";
        interactShellMode = true;
    } else {
        inputFileStrm = std::ifstream(inputFilename.c_str());
    }

}

void ASTGenerator::Generate() {
    if (interactShellMode) {
        // Enter the interactive loop
        ReplLoop();
    } else {
        if (inputFileStrm.good()) {
            errs() << "Yep, I can read the file \n";
            
            // Enter the source file read loop
            EatFile(inputFileStrm);
        } else {
            errs() << "Error: There is a problem reading the file! \n";
        }
    }
}
