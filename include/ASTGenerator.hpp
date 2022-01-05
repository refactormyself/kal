#ifndef ASTGENERATOR_HPP
#define ASTGENERATOR_HPP

#include <fstream>
#include "Lexer.hpp"
#include "Parser.hpp"

namespace kal{
    class ASTGenerator
    {
    private:
        int lastChar = ' ';
        int currToken = 0;
        bool interactShellMode = false;
        std::string inputFilename;

        [[noreturn]] void ReplLoop();
        
    public:
        ASTGenerator(const std::string &filename);
        void Generate();
    };
}
#endif //ASTGENERATOR_HPP
