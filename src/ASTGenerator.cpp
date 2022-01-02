#include "ASTGenerator.hpp"
#include "llvm/Support/raw_ostream.h"
#include "Parser.hpp"

using namespace kal;
using namespace llvm;

ASTGenerator::ASTGenerator(const std::string &filename) {
    inputFilename = filename;
    if (inputFilename == "-")
        interactShellMode = true;
}

void ASTGenerator::Generate() {
    if (interactShellMode) {
        ReplLoop(); 
        return;
    }

    // non-interactive mode
    Lexer lexer(inputFilename);
    Parser parser(lexer); 
    while (currToken != tok_eof)
    {
        currToken = lexer.GetToken();
        // outs() << "Token Num: " << currToken << " \n";
        currToken = parser.EatTokens(currToken);
    }
}

void ASTGenerator::ReplLoop() {
    Lexer lexer;
    Parser parser(lexer); 
    do { // This loop shows the prompt, to indicate - DONE/Ready
        fprintf(stderr, "kal> ");
        currToken = lexer.GetToken();
        // outs() << "Token Num: " << currToken << " \n";
        currToken = parser.EatTokens(currToken);
    } while (true);
}
