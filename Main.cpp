#include <iostream>
#include "Interface.h"
#include "Tree.h"

int main()
{
    std::cout << "Hello World!\n";
    bool running = true;
    CTree *tree = NULL;
    while (running)
    {
		vector<string> command = interface::getUserInput();
		running = interface::handleCommand(command, &tree);
        
	}
    
    return 0;
}
/*
TODO:
finish error checking,
use tree instead of *tree in main

*/
