#ifndef ASTGENERATOR_HPP
#define ASTGENERATOR_HPP

#include <fstream>
// #include "Lexer.hpp"

namespace kal{
    class ASTGenerator
    {
    private:
        int lastChar = ' ';
        int currTok;
        bool interactShellMode = false;
        std::ifstream inputFileStrm;
        void EatFile(std::ifstream &inputFileStrm);
        bool EatStdioToken();
        void ReplLoop();
        
    public:
        ASTGenerator(std::string inputFilename);
        void Generate();
    };
}
#endif //ASTGENERATOR_HPP
