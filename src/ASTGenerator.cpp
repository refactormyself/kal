#include "ASTGenerator.hpp"
#include "llvm/Support/raw_ostream.h"

using namespace kal;
using namespace llvm;

ASTGenerator::ASTGenerator(const std::string &filename) {
    inputFilename = filename;
    if (inputFilename == "-")
        interactShellMode = true;
}

void ASTGenerator::Generate() {
    Lexer lexer;
    if (interactShellMode)
        ReplLoop(); 
    else
        lexer = Lexer(inputFilename); // non-interactive mode

    currToken = lexer.GetToken(); // this get only ONE token thats all
    outs() << "Token Num: " << currToken << " \n";
}

void ASTGenerator::ReplLoop() {
    Lexer lexer;
    do { // This loop shows the prompt, to indicate - DONE/Ready
        fprintf(stderr, "kal> ");
        currToken = lexer.GetToken();
        outs() << "Token Num: " << currToken << " \n";
    }while (true);
}
