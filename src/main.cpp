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
  ASTGenerator Astgen(InputFilename);
  Astgen.Generate();

  return 0;
}