#pragma once
#include <string>
#include <vector>

const std::vector<std::string> operations2children = { "+", "-", "*", "/"};
const std::vector<std::string> operations1child = { "sin", "cos"};
const char minDigit = '0';
const char maxDigit = '9';
const char minSmalLetter = 'a';
const char maxSmallLetter = 'z';
const char minCapitalLetter = 'A';
const char maxCapitalLetter = 'Z';

class CNode 
{

private:
	CNode* left, * right; // left and right children
	CNode *parent;		  // parent of root is NULL
	std::string value;	  // operation or constant or variable
	int type;			  // 1 - operation with 1 child, 2 - operation with 2 children, 3 - constant, 4 - variable
	static int currentIndex;
	static int getType(std::string *value); // return type of a string (operation, constant or variable), if its a variable, turns it into a valid variable name
	static bool isNumber(const std::string value); // return true if string is a number
	static std::string validateVariableName(const std::string value); // turn string into a valid variable name
	

public:
	//CNode(); //Default constructor
	CNode(const std::vector<std::string> expression, CNode* parent); //Constructor from a string at index (calls next constructor)
	static void inOrderWalk(CNode* node); //Print expression used to create the tree
	static void deleteTree(CNode* node); // delete all children and itself
	static std::vector<std::string> getVars(CNode* node, std::vector<std::string> *accumulator) ; // return expression used to create the tree

};
/*
Implementing error checking in the expression:
If the tree is constructed (no null children left) but there are elements left, omit leftovers and print a message:
	check if currentIndex is at the end of vector after tree construction
If the tree is not constructed (null children left) but there are no elements left, fill null children with default value (1) and print a message
	while taking value from vector, check if currentIndex is at the end of vector, if it is take value 1 instead and print a message
*/

class CTree 
{

private:
	CNode* root;
	
public:
	//CTree(); //Default constructor
	//CTree(const CTree& otherInstance); //Copy constructor
	CTree(const std::vector<std::string> expression); //Constructor from a vector of strings 

	//CTree operator=(const CTree& otherInstance); //Set current tree to a copy of another tree
	//CTree operator+(const CTree& otherInstance) const; //Return copy, result of adding another tree to current tree

	void printExpression() const; //Print expression used to create the tree
	void printVars() const; //Print all variables used in the expression
	

	~CTree(); //Destructor
};

