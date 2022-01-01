#include "Lexer.hpp"
#include "llvm/Support/raw_ostream.h"

#include <cstdlib>
#include <string>

using namespace llvm;
using namespace kal;

Lexer::Lexer(const std::string &inputFilename) {
    if (inputFilename == "-" || inputFilename == "")
    {
        outs() << "No file name supplied.\nSwitching to Interactive Shell Mode.\n";
        interactShellMode = true;
        
        // Enter the interactive loop
        // ReplLoop(Astgen);
    } else {
        inputFileStrm = std::ifstream(inputFilename.c_str());
        if (inputFileStrm.good())
        {
            outs() << "Yep, I can read the file " << inputFilename << " \n";
            interactShellMode = false;
        } else {
            outs() << "Error: There is a problem reading the file " 
                   << inputFilename << " ! \n";
        }
    }
}

int Lexer::Getnextchar() {
    if (interactShellMode)
        return getchar();
    else
        return inputFileStrm.get();
}

int Lexer::GetToken() {
    static int lastChar = ' '; // we start with whitespace

    // Skip any whitespace.
    while (isspace(lastChar))
        lastChar  = Getnextchar();


    // buffer the next set of char => token

    // Token identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(lastChar)) {
        IdentifierStr = lastChar;
        while (isalnum((lastChar = Getnextchar())))
            IdentifierStr += lastChar;

        // check for reserved identifiers
        if (IdentifierStr == "binary")
            return tok_binary;
        if (IdentifierStr == "unary")
            return tok_unary;

        return tok_identifier;
    }

    // Number: [0-9]+[.]+[0-9]+
    if (isdigit(lastChar)) { 
        std::string numStr;
        bool has1dot = false;
        do {
            numStr += lastChar;
            lastChar = Getnextchar();
            if (lastChar == '.') {
                if (!has1dot) {
                    numStr += lastChar;
                    has1dot = true;
                } else {// not more than one dot, so bail out
                    int thisChar = lastChar;
                    lastChar = Getnextchar();
                    return thisChar; // [0 - 255] is designated unknown token
                }
            }
            
        } while (isdigit(lastChar) || lastChar == '.');

        if (has1dot) {
            FloatVal = std::stod(numStr.c_str(), nullptr);
            return tok_float;
        } else {
            IntVal = std::stoi(numStr.c_str(), nullptr);
            return tok_integer;
        }
    }

    // Handle whole comments
    if (lastChar == '#')
    {
        do
            lastChar = Getnextchar();
        while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

        if (lastChar != EOF)
            return Getnextchar();
    }

    // Check for end of file.  Don't eat the EOF.
    if (lastChar == EOF)
        return tok_eof;


    // return unknown token
    int thisChar = lastChar;
    lastChar = Getnextchar();
    return thisChar; // [0 - 255] is designated unknown token
}