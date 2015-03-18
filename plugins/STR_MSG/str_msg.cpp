#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[]) 
{
    if(argc < 2) 
        throw std::runtime_error("Insuficient number of arguments");

    for(int i = 1; i < argc; ++i)
        std::cout << argv[i] << std::endl;
}
