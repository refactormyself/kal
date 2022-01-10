#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>

namespace kal
{
    // Valid tokens
    enum Token
    {
        // special tokens
        tok_exit = -1,  // explicit exit
        tok_eof = -2,
        tok_carr_ret = -3,  // return key char(10)

        // primary tokens
        tok_identifier = -4,
        tok_integer = -5,
        tok_float = -6,

        // commands
        tok_def = -8,
        tok_extern = -9,

    };

    class Lexer
    {
    private:
        std::ifstream inputFileStrm;
        bool interactShellMode = true;
        int Getnextchar();

    public:
        Lexer() = default;
        explicit Lexer(const std::string &inputFilename);
        int GetToken();
        double FloatVal = 0;
        int IntVal = 0;
        std::string IdentifierStr;
    };
}

#endif //LEXER_HPP
