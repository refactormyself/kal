#include "ASTGenerator.hpp"

using namespace kal;

void ASTGenerator::EatFile(const std::ofstream &InputFile){

}

bool ASTGenerator::EatStdioToken() {
    int Tok = getchar();
    return true;
}