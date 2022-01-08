#ifndef ASTGENERATOR_HPP
#define ASTGENERATOR_HPP

#include <fstream>
#include "CodeGen.hpp"
#include "Parser.hpp"

namespace kal{
    class ASTGenerator
    {
    private:
        int currToken = 0;
        bool interactShellMode = false;
        Parser parser;
    public:
        explicit ASTGenerator(const std::string &filename);
        std::string Generate();

        bool IsREPLmode();
    };
}
#endif //ASTGENERATOR_HPP
