#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Tree.h"
using namespace std;

const string command_exit = "exit";
const string command_quit = "quit";
const string command_enterTree = "enter";
const string command_listVars = "vars";
const string command_printTree = "print";
const string command_calculate = "comp";
const string command_addTree = "join";
const string defaultVarName = "x";

const string notification_wrongCommand = "Wrong command, try again";
const string notification_exit = "Terminating the program";
const string notification_awaitingCommand = "Awaiting command to execute";
const string notification_ommitingInvalidChar = " is not valid in a variable name, ommiting";
const string notification_emptyVariableName = "Variable name cannot be empty, using default name: ";
const string notification_varNameOnlyNumbers = "Variable name cannot contain only numbers, appending default name: ";
const string notification_noTree = "No tree found, use enter command to create a new tree";
const string notification_printingTree = "Printing tree: ";
const string notification_printingExpression = "Constructed a tree using the expression: ";
const string notification_printingVars = "Printing variables: ";
const string notification_ommitingLeftovers = "Ommiting invalid values at the end of expression which couldnt be attached:";


namespace interface 
{
	vector<string> getUserInput(); // Get user input from console and split it into vector of strings
	inline void print(const string& message) { cout << message << endl; }; // Print to console
	inline void printSpace(const string& message) { cout << message << " "; }; // Print to console
	vector<string> splitBySpace(const string& input); // Split string into a vector
	bool handleCommand(const vector<string>& command, CTree** tree); // Call methods based on user command, false if exit

}