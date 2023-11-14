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
		if (input[i] == char_space) // if current char is a space, add currentWord to result and reset currentWord
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
		interface::print((*tree)->clearErrors());
		interface::print(notification_printingExpression);
		(*tree)->printExpression();
		return true; 
	}

	else if (commandName == command_listVars)  
	{ 
		if ((!treeIsInitialized(tree))) { interface::print(notification_noTree); }
		else 
		{ 
			vector<string> vars = (*tree)->getVars();
			interface::print(notification_printingVars);
			for (int i = 0; i < vars.size(); i++)
			{
				interface::printSpace(vars[i]);
			}
			interface::print(""); // newline

		}
		return true; 
	}

	else if (commandName == command_printTree) 
	{ 
		if ((!treeIsInitialized(tree))) { interface::print(notification_noTree); }
		else 
		{ 
			interface::print(notification_printingTree);
			(*tree)->printExpression(); 
		}
		return true; 
	} 

	else if (commandName == command_calculate) 
	{ 
		if (!treeIsInitialized(tree)) { interface::print(notification_noTree); return true; }
		std::vector<double> values;
		for (int i = 1; i < command.size(); i++) 
		{
			double value = std::atof(command[i].c_str()); // convert string to double using atof (c++98 standard)
			if (value > 0.0) { values.push_back(value); } // only add valid positive doubles
			else { interface::print(command[i] + notification_invalidVariableValue); return true; }
		}
		if (values.size() != (*tree)->getVars().size()) { interface::print(notification_invalidVariablesNumber); return true; }
		interface::print((*tree)->calculate(values));
		interface::print((*tree)->clearErrors());
		return true; 
	}

	else if (commandName == command_addTree) 
	{ 
		if (!treeIsInitialized(tree)) { interface::print(notification_noTree); return true; }
		(**tree) = (**tree) + CTree(command);
		interface::print((*tree)->clearErrors());
		interface::print(notification_printingExpression);
		(*tree)->printExpression();
		return true; 
	}

	else { interface::print(notification_wrongCommand); return true; }
	
}

bool interface::treeIsInitialized(CTree** tree)
{
	if (*tree == NULL || !(*tree)->isInitialized()) { return false; }
	else { return true; }
}
