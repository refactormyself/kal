#include "ASTGenerator.hpp"

#include "llvm/Support/CommandLine.h"
#include <fstream>

// #include "llvm/Support/VirtualFileSystem.h"

using namespace llvm;
using namespace std;
using namespace kal;

//Command line Options
// -o <filename>
cl::opt<string> OutputFilename("o", cl::desc("Specify output filename"), cl::value_desc("filename"));
// <input file>
cl::opt<string> InputFilename(cl::Positional, cl::desc("<input file>"), cl::init("-"));


int main(int argc, char **argv)
{
  cl::ParseCommandLineOptions(argc, argv, " Kal is language inspired on LLVM basic tutorial \n\n");

  // Install standard binary operators.
  // 1 is lowest precedence.
  Parser::BinopPrecedence['<'] = 10;
  Parser::BinopPrecedence['+'] = 20;
  Parser::BinopPrecedence['-'] = 20;
  Parser::BinopPrecedence['/'] = 40;
  Parser::BinopPrecedence['*'] = 40;  // highest.

  ASTGenerator Astgen(InputFilename);
  auto module = Astgen.Generate();
  if (!Astgen.IsREPLmode()) {
      ofstream outputStream;
      outputStream.open(OutputFilename);
      if (outputStream) {
          outputStream << module;
          outs() << "The Module's IR written into: " << OutputFilename << "\n";
          outs() << "Use the \"lli\" tool to directly execute it.\n";
      } else errs() << "Failed to open the file " << OutputFilename << " for write\n";
  }
  return 0;
}