#ifndef ASTGENERATOR_HPP
#define ASTGENERATOR_HPP

#include <fstream>

namespace kal{
    class ASTGenerator
    {
    private:
    
    public:
        void EatFile(const std::ofstream &InputFile);
        bool EatStdioToken();
    };
}
#endif //ASTGENERATOR_HPP
