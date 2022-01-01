#ifndef ASTGENERATOR_HPP
#define ASTGENERATOR_HPP

#include <fstream>
#include "Lexer.hpp"

namespace kal{
    class ASTGenerator
    {
    private:
        int lastChar = ' ';
        int currToken;
        bool interactShellMode = false;
        std::string inputFilename;
        void ReplLoop();
        
    public:
        ASTGenerator(const std::string &filename);
        void Generate();
    };
}
#endif //ASTGENERATOR_HPP
