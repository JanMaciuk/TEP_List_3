#include "Interface.h"
using namespace std;

vector<string> interface::getUserInput() {
	interface::print(notification_awaitingCommand);
	string consoleInput;
	std::getline(cin, consoleInput);
	return splitBySpace(consoleInput);
	
}

vector<string> interface::splitBySpace(const string& input) {
	vector<string> result;
	string currentWord;
	for (int i = 0; i < input.length(); i++) 
	{
		if (input[i] == ' ') // if current char is a space, add currentWord to result and reset currentWord
		{ 
			if (currentWord != "") { result.push_back(currentWord); } //Skip multiple spaces
			currentWord = "";
		}
		else { currentWord += input[i]; } // add character to current word
		
	}
	if (currentWord != "") result.push_back(currentWord);
	return result;
}

bool interface::handleCommand(const vector<string>& command, CTree** tree) 
{
	//call an appropriate method based on the command
	//return false if exiting the program
	//wrong command should print to console and continue
	string commandName = command[0];
	// cannot use switch in a nice way beacause dealing with string type (enum is C++11)
	if (commandName == command_exit || commandName == command_quit)  // end the program
	{ 
		interface::print(notification_exit); 
		if (*tree != NULL) { delete *tree; }
		return false; 
	}
	else if (commandName == command_enterTree) 
	{ 
		if (*tree != NULL) 
		{ 
			delete *tree; 
		}
		*tree = new CTree(command);
		interface::print(notification_printingExpression);
		(*tree)->printExpression();
		return true; 
	}
	else if (commandName == command_listVars)  
	{ 
		if (*tree == NULL) { interface::print(notification_noTree); }
		else { (*tree)->printVars(); }
		return true; 
	}
	else if (commandName == command_printTree) 
	{ 
		if (*tree == NULL) { interface::print(notification_noTree); }
		else 
		{ 
			interface::print(notification_printingTree);
			(*tree)->printExpression(); 
		}
		return true; 
	} 
	else if (commandName == command_calculate) { return true; } //TODO: implement 
	else if (commandName == command_addTree)   { return true; } //TODO: implement
	else { interface::print(notification_wrongCommand); return true; }
	
}
