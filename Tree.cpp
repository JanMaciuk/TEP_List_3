#include "Tree.h"
#include <algorithm>
#include "interface.h"

//CTree:
CTree::CTree(const std::vector<std::string> expression) 
{
	int index = 1;
	root = new CNode(expression, NULL,&index);
	// if position is not at the end of vector, print notification and ommit leftovers
	if (index < expression.size())
	{
		root->logErrorSpace(notification_ommitingLeftovers);
		for (int i = index; i < expression.size(); i++)
		{
			root->logErrorSpace(expression[i]);
		}
		root->logError(emptyString); // newline
	} 
}

CTree::CTree(const CTree& otherInstance) // copy constructor
{
	std::vector<std::string> expression = otherInstance.getExpression();
	expression.insert(expression.begin(), command_enterTree);
	int index = 1;
	root = new CNode(expression, NULL, &index);
	// No need to check for leftovers, original tree must be valid
}

CTree::CTree() // default constructor
{
	root = NULL;
}


void CTree::printExpression() const
{
	root->inOrderWalkPrint();
	interface::print(emptyString); // newline
}

std::vector<std::string> CTree::getVars() const
{
	std::vector<std::string> accumulator;
	std::vector<std::string> vars = root->getVars(&accumulator);
	return vars;

}

double CTree::calculate(std::vector<double> values) const
{
	std::vector<std::string> accumulator;
	std::vector<std::string> vars = root->getVars(&accumulator);
	return CNode::calculate(root, vars, values);
}

std::vector<std::string> CTree::getExpression() const
{
	std::vector<std::string> accumulator;
	std::vector<std::string> expression = root->inOrderWalk(&accumulator);
	return expression;
}

std::string CTree::getErrors() const {
	return root->getErrors(false);
}
std::string CTree::clearErrors() {
	return root->getErrors(true);
}


void CTree::operator=(const CTree& otherInstance)
{
	// set current tree to a copy of another tree
	// if current tree is not empty, delete it
	if (this->root != NULL) { this->root->deleteTree(); }
	// create a new tree with the same expression as the other tree
	std::vector<std::string> expression = otherInstance.getExpression();
	expression.insert(expression.begin(), command_enterTree);
	int index = 1;
	this->root = new CNode(expression, NULL,&index);
}

CTree CTree::operator+(const CTree& otherInstance) const
{
	// join two trees using operator+
	// if both trees are empty, return empty tree
	if (this->root == NULL && otherInstance.root == NULL) { return CTree(); }
	// if one of the trees is empty, return the other tree
	else if (this->root == NULL) { return otherInstance; }
	else if (otherInstance.root == NULL) { return *this; }
	// if both trees are not empty, create a new tree which is a copy of the first with second one added to it.
	else
	{ 
		// get expressions of both trees
		// remove last element from the first expression (it is a leaf)
		// add the second expression to the first one
		// create a new tree from the new expression
		std::vector<std::string> thisExpression = this->getExpression(); 
		std::vector<std::string> otherExpression = otherInstance.getExpression();
		thisExpression.pop_back();
		thisExpression.insert(thisExpression.begin(), command_enterTree); // add first placeholder to expression
		for (int i = 0; i < otherExpression.size(); i++)
		{
			thisExpression.push_back(otherExpression[i]);
		}
		CTree resultTree = CTree(thisExpression);
		return resultTree;
		
	}
}

CTree::~CTree() { root->deleteTree(); }



//CNode::
CNode::CNode(const std::vector<std::string> expression, CNode* parentNode, int* currentIndex)
{
	parent = parentNode;
	string val = defaultNodeValue;
	if (*currentIndex < expression.size()) { val = expression[*currentIndex]; } // if there are values left in the vector, get the next one
	else 
	{ // if there are no values left, notify user and use default value
		if (parentNode == NULL) { logError(notification_missingValue + defaultNodeValue); }
		else { logError(parentNode->value + notification_missingValue + defaultNodeValue); }
	} 
	(*currentIndex)++;
	type = getType(&val); // get type of the value, if its a variable, turn it into a valid variable name
	value = val;
	//fill vector with NULL children to avoid accessing unalocated memory by [] operator
	for (int i = 0; i < maxChildrenCount; i++)
	{
		children.push_back(NULL);
	}
	if (type == 1) // if operation with 1 child, create left child only
	{
		children[0] = new CNode(expression, this,currentIndex);
		children[1] = NULL;
	}
	else if (type == 2) // if operation with 2 children, create left and right children 
	{
		children[0] = new CNode(expression, this,currentIndex);
		children[1] = new CNode(expression, this,currentIndex);
	}


}

void CNode::inOrderWalkPrint() const
{
	if (this == NULL) { return; }
	interface::printSpace(value);
	children[0]->inOrderWalkPrint();
	children[1]->inOrderWalkPrint();

}

std::vector<std::string> CNode::inOrderWalk(std::vector<std::string>* accumulator) const
{
	if (this == NULL) { return *accumulator; }
	accumulator->push_back(value);
	children[0]->inOrderWalk(accumulator);
	children[1]->inOrderWalk(accumulator);
	return *accumulator;
}

void CNode::logError(const std::string message)
{
	errMsg += message + endLine;
}
void CNode::logErrorSpace(const std::string message)
{
	errMsg += message + space;
}
std::string CNode::getErrors(bool clear)
{
	std::string result = errMsg;
	if (clear) errMsg = emptyString;
	return result;
}

int CNode::getType( std::string *value)
{
	// 1 - operation with 1 child, 2 - operation with 2 children, 3 - constant, 4 - variable (name is made valid)
	if (std::find(operations1child.begin(), operations1child.end(), *value) != operations1child.end()) { return 1; } // if value is in the list of operations with 1 child
	else if (std::find(operations2children.begin(), operations2children.end(), *value) != operations2children.end()) { return 2; } // if value is in the list of operators with 2 children
	else if (isNumber(*value)) // if value is a number/constant return 3
	{ 
		//ensure the number is not zero
		bool isZero = false;
		for (int i = 0; i < value->length(); i++)
		{
			if ((*value)[i] == minDigit) { isZero = true; }
		}
		if (isZero)
		{
			logError(notification_zeroNotAllowed + defaultNodeValue);
			*value = defaultNodeValue;
		}
		return 3; 
	} 
	else // if value is a variable validate it
	{ 
		*value = validateVariableName(*value);
		return 4; 
	
	} 

}

bool CNode::isNumber(const std::string value)
{
	for (int i = 0; i < value.length(); i++)
	{ // if any character is not withing ascii range of digits, return false
		if (value[i] < minDigit || value[i] > maxDigit) { return false; }
	}
	return true;
}

std::string CNode::errMsg = emptyString;
std::string CNode::validateVariableName(const std::string value)
{
	// turn string into a valid variable name, requirements to be valid:
	// cannot be empty
	// cannot contain only numbers
	// can only contain letters and numbers
	
	std::string result = emptyString;
	for (int i = 0; i < value.length(); i++)
	{
		if ((value[i] >= minDigit && value[i] <= maxDigit) || (value[i] >= minSmalLetter && value[i] <= maxSmallLetter) || (value[i] >= minCapitalLetter && value[i] <= maxCapitalLetter))
		{
			result += value[i];
		}
		else // if character is not a letter or a number, print notification and ommit it
		{
			logError(value[i] + notification_ommitingInvalidChar);
		}
	}
	if (result == emptyString) // if variable name is empty, add default name to make it valid
	{ 
		logError(notification_emptyVariableName + defaultVarName);
		result = defaultVarName; 
	}
	if (isNumber(result)) // if variable name is only numbers, add default name to the beginning to make it valid
	{ 
		logError(notification_varNameOnlyNumbers);
		result = defaultVarName + result; 
	} 
	return result;
}

std::vector<std::string> CNode::getVars(std::vector<std::string>* accumulator) const
{
	if (this == NULL) { return *accumulator; }
	if ((this->type == 4) && (std::find((*accumulator).begin(), (*accumulator).end(), (this->value)) == (*accumulator).end()))
	{ // if node is a variable and is not in the accumulator, add it
		accumulator->push_back(this->value);
	} // then walk throught the rest of the tree
	children[0]->getVars(accumulator);
	children[1]->getVars(accumulator);
	return *accumulator;
}


double CNode::calculate(CNode* node, const std::vector<std::string> vars, const std::vector<double> values)
{
	if (node == NULL) { return 0; }

	else if (node->type == 3)  // if its a constant, simply return its value
	{
		bool overflow;
		int value = strToInt(node->value, &overflow);
		if (overflow) 
		{ 
			value = strToInt(defaultNodeValue, &overflow); 
			logError(notification_overflow + defaultNodeValue);
		}
		return value;
	}

	else if (node->type == 4) // if its a variable, find its value in values vector
	{
		int index = std::find(vars.begin(), vars.end(), node->value) - vars.begin(); // variable must be in the vars vector, so index will be valid (length is checked before)
		return values[index];
	}

	else if (node->type == 2) // if its a normal operator, calculate the values
	{
		double leftResult = calculate(node->children[0], vars, values);
		double rightResult = calculate(node->children[1], vars, values);

		if (node->value == operations2children[0]) 
		{ 
			if ((leftResult + rightResult) < 0) // Checking for overflow
			{
				logError(notification_overflow + defaultNodeValue);
				bool overflow;
				return strToInt(defaultNodeValue, &overflow);
			}
			return leftResult + rightResult; 
		}
		else if (node->value == operations2children[1]) { return leftResult - rightResult; } // never overflows: both positive valid ints
		else if (node->value == operations2children[multiplicationIndex]) 
		{ 
			if ((leftResult * rightResult) < 0) // Checking for overflow
			{
				logError(notification_overflow + defaultNodeValue);
				bool overflow;
				return strToInt(defaultNodeValue, &overflow);
			}
			return leftResult * rightResult; 
		}
		else if (node->value == operations2children[divisionIndex]) { return double(leftResult) / double(rightResult); }
	}
	
	else if (node->type == 1)
	{
		double childResult = calculate(node->children[0], vars, values);
		if (node->value == operations1child[0]) { return sin(childResult); }
		if (node->value == operations1child[1]) { return cos(childResult); }
	}

	return 0;
}

int CNode::strToInt(const std::string value, bool* overflow)
{
	// convert string to int
	// if overflow, set overflow to true
	// value must be a number (checked before call, function is private)
	*overflow = false;
	int result = 0;
	for (int i = 0; i < value.length(); i++)
	{
		result *= baseNumber;
		result += (value[i] - minDigit);
		if (result < 0) { *overflow = true; }

	}
	return result;
}



void CNode::deleteTree() // delete the called node and all its descendants
{
	if (this == NULL) { return; }
	for (int i = 0; i < maxChildrenCount; i++)
	{
		children[i]->deleteTree();
	}
	delete this;
}

