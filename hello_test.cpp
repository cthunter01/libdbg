#include <iostream>
#include <unistd.h>

int main(int argc, char **argv)
{
    std::cerr << "This is my test program\n";
    if(argc == 2)
    {
            std::cerr << "You entered a parameter 1: " << argv[1] << "\n";
    }
    if(argc == 3)
    {
            std::cerr << "You entered two parameters: " << argv[1] << argv[2] << "\n";
    }
    return 0;
}
