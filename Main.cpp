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
    //testing: todo: delete
    /*
    std::vector<std::string> expression1 = { "enter", "*", "2", "1"};
    std::vector<std::string> expression2 = { "enter", "sin", "x" };
    CTree *tree1 = NULL;
    CTree *tree2 = NULL;
    interface::handleCommand(expression1, &tree1);
    interface::handleCommand(expression2, &tree2);
    tree1->printExpression();
    tree2->printExpression();
    CTree tree3 = *tree1 + *tree2;
    tree3.printExpression();
    */
    
    return 0;
}
/*
TODO:
finish error checking,
use tree instead of *tree in main
join two trees using operators,

*/
