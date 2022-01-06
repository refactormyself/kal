#include "ASTGenerator.hpp"
#include "llvm/Support/raw_ostream.h"
#include "CodeGen.hpp"

using namespace kal;
using namespace llvm;

ASTGenerator::ASTGenerator(const std::string &filename) {
    inputFilename = filename;
    if (inputFilename == "-")
        interactShellMode = true;

//    Initialise Program's Context
    CodeGen::TheContext = std::make_unique<LLVMContext>();
//    CodeGen::TheModule = std::make_unique<Module>("my cool jit", *CodeGen::TheContext);

    // Create a new builder for the module.
    CodeGen::Builder = std::make_unique<IRBuilder<llvm::NoFolder>>(*CodeGen::TheContext);
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

[[noreturn]]
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
