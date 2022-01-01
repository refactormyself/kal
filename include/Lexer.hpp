#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>

namespace kal
{
    // Valid tokens
    enum Token
    {
        // special tokens
        tok_eof = -1,
  
        // primary tokens
        tok_identifier = -4,
        tok_integer = -5,
        tok_float = -6,
        
        // operator tokens
        tok_binary = -11,
        tok_unary = -12,

    };

    class Lexer
    {
    private:
        std::ifstream inputFileStrm;
        bool interactShellMode = true;
        int Getnextchar();

    public:
        Lexer() = default;
        Lexer(const std::string &inputFilename);
        int GetToken();
        double FloatVal;
        int IntVal;
        std::string IdentifierStr;
    };
}

#endif //LEXER_HPP
