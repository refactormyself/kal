#include "ASTGenerator.hpp"
#include "llvm/Support/raw_ostream.h"

using namespace kal;
using namespace llvm;

ASTGenerator::ASTGenerator(const std::string &filename) {
    interactShellMode = filename == "-" || filename.empty();
    parser = Parser(filename);

//    Initialise Program's Context
    CodeGen::TheContext = std::make_unique<LLVMContext>();
    CodeGen::TheModule = std::make_unique<Module>("my module", *CodeGen::TheContext);

    // Create a new builder for the module.
    CodeGen::Builder = std::make_unique<IRBuilder<llvm::NoFolder>>(*CodeGen::TheContext);
}

void ASTGenerator::Generate() {
    do {
        if (interactShellMode)
            fprintf(stderr, "kal> ");
        currToken = parser.EatToken();
    } while (currToken != tok_eof);
}