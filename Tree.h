#pragma once
#include <string>
#include <vector>

const std::vector<std::string> operations2children = { "+", "-", "*", "/"};
const int multiplicationIndex = 2;
const int divisionIndex = 3;
const std::vector<std::string> operations1child = { "sin", "cos"};
const char minDigit = '0';
const char maxDigit = '9';
const char minSmalLetter = 'a';
const char maxSmallLetter = 'z';
const char minCapitalLetter = 'A';
const char maxCapitalLetter = 'Z';
const std::string defaultNodeValue = "1";
const int baseNumber = 10;
const int maxChildrenCount = 3;


class CNode 
{

private:
	std::vector<CNode*> children; // vector with children
	CNode* left, * right; // left and right children
	CNode *parent;		  // parent of root is NULL
	std::string value;	  // operation or constant or variable
	int type;			  // 1 - operation with 1 child, 2 - operation with 2 children, 3 - constant, 4 - variable
	static int currentIndex;
	static int getType(std::string *value);			// return type of a string (operation, constant or variable), if its a variable, turns it into a valid variable name
	static bool isNumber(const std::string value);  // return true if string is a number
	static std::string validateVariableName(const std::string value);	// turn string into a valid variable name
	static int strToInt(const std::string value, bool *overflow);		// convert string to int

public:
	//CNode(); //Default constructor
	CNode(const std::vector<std::string> expression, CNode* parent);					//Constructor from a string at index (calls next constructor)
	std::vector<std::string> inOrderWalk(std::vector<std::string> *accumulator) const;  // return expression used to create the tree
	std::vector<std::string> getVars(std::vector<std::string> *accumulator) const ;		// return expression used to create the tree
	inline static int getCurrentPosition() { return currentIndex; };					// return value of currentIndex 
	inline static void resetCurrentPosition() { currentIndex = 1; };					// set currentIndex to 1
	static double calculate(CNode* node, const std::vector<std::string> vars, const std::vector<double> values); // calculate expression using variables and values
	void inOrderWalkPrint() const;  //Print expression used to create the tree 
	void deleteTree();				// delete all children and itself
};

/*
Implementing error checking in the expression:
If the tree is constructed (no null children left) but there are elements left, omit leftovers and print a message:
	check if currentIndex is at the end of vector after tree construction
If the tree is not constructed (null children left) but there are no elements left, fill null children with default value (1) and print a message
	while taking value from vector, check if currentIndex is at the end of vector, if it is take defaultValue instead and print a message

Zeros are not allowed, so division by zero is impossible
*/

class CTree 
{

private:
	CNode* root;
	
public:
	CTree();													//Default constructor - empty tree
	CTree(const CTree& otherInstance);							//Copy constructor
	CTree(const std::vector<std::string> expression);			//Constructor from a vector of strings 

	void operator=(const CTree& otherInstance);				//Set current tree to a copy of another tree
	CTree operator+(const CTree& otherInstance) const;			//Return copy, result of adding another tree to current tree

	void printExpression() const;								//Print expression used to create the tree
	std::vector<std::string> getExpression() const;				//Return expression used to create the tree
	std::vector<std::string> getVars() const;					//Print all variables used in the expression
	double calculate(std::vector<double> values) const;			//Calculate expression using variable values
	inline bool isInitialized() const { return root != NULL; }; //Check if tree is initialized

	~CTree(); //Destructor
};

