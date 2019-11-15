#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <utility>

int main(int argc, char *argv[])
{
    std::ifstream inFile("commandfile.txt", std::fstream::in);

    typedef std::pair<std::string, std::string> CommandPairType;
    CommandPairType commandPair;
    std::vector<CommandPairType> commands;
    std::vector<std::string> commandLines;

    std::string line;

    // get all command lines
    while(std::getline(inFile, line))
    {
        commandLines.push_back(line);
    }
    inFile.close();

    std::cout << "Read " << commandLines.size() << " lines\n";

    for(std::vector<std::string>::iterator i = commandLines.begin(); i != commandLines.end(); ++i)
    {
        std::stringstream ls(*i);
        std::string token;
        std::vector<std::string> tokens;
        while(std::getline(ls, token, ' '))
        {
            tokens.push_back(token);
        }
        if(2 == tokens.size())
        {
            CommandPairType command = std::make_pair(tokens[0], tokens[1]);
            commands.push_back(command);
        }

        std::cout << "command: " << commands.rbegin()->first << std::endl;
        std::cout << "argument: " << commands.rbegin()->second << std::endl;
    }


    return 0;
}
