#include "ASTGenerator.hpp"

#include "llvm/Support/CommandLine.h"
#include <fstream>

// #include "llvm/Support/VirtualFileSystem.h"

using namespace llvm;
using namespace std;
using namespace kal;

bool InteractShellMode = false;

// -o <filename>
cl::opt<string> OutputFilename("o", cl::desc("Specify output filename"), cl::value_desc("filename"));

// <input file>
cl::opt<string> InputFilename(cl::Positional, cl::desc("<input file>"), cl::init("-"));

static void ReplLoop(ASTGenerator Astgen) {
  do {
    fprintf(stderr, "kal> ");
    Astgen.EatStdioToken();
  }while (true);
}

int main(int argc, char **argv)
{
    cl::ParseCommandLineOptions(argc, argv, " Kal is language based on LLVM basic tutorial \n\n");
    ASTGenerator Astgen;
   
    if (InputFilename == "-")
    {
        outs() << "Blank! No file name supplied.\nSwitching to Interactive Shell Mode.\n";
        InteractShellMode = true;
        
        // Enter the interactive loop
        ReplLoop(Astgen);
    }
    
    if (!InteractShellMode) {
        std::ofstream Input(InputFilename.c_str());
        if (Input.good())
        {
            outs() << "Yep, I can read the file " << InputFilename << " \n";
            
            // Enter the source file read loop
            Astgen.EatFile(Input);
        } else {
            outs() << "Error: There is a problem reading the file " 
                   << InputFilename << " ! \n";
        }
    }

   return 0;
}