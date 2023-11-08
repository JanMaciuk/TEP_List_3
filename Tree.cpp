#include "Tree.h"
#include <algorithm>
#include "interface.h"

//CTree:
CTree::CTree(const std::vector<std::string> expression) 
{
	root = new CNode(expression, NULL);
}

void CTree::printExpression() const
{
	CNode::inOrderWalk(root);
	interface::print(""); // newline
}

void CTree::printVars() const
{
	std::vector<std::string> accumulator;
	std::vector<std::string> vars = CNode::getVars(root, &accumulator);
	interface::print(notification_printingVars);
	for (int i = 0; i < vars.size(); i++)
	{
		interface::printSpace(vars[i]);
	}
	interface::print(""); // newline
}

CTree::~CTree() { CNode::deleteTree(root); }

//CNode::

CNode::CNode(const std::vector<std::string> expression, CNode* parentNode)
{
	parent = parentNode;
	string val = expression[currentIndex];
	currentIndex++;
	type = getType(&val); // get type of the value, if its a variable, turn it into a valid variable name
	value = val;
	if (type == 1) // if operation with 1 child, create left child only
	{
		left = new CNode(expression, this);
		right = NULL;
	}
	else if (type == 2) // if operation with 2 children, create left and right children 
	{
		left = new CNode(expression, this);
		right = new CNode(expression, this);
	}
	else // if constant or variable, no children
	{
		left = NULL;
		right = NULL;
	}


}

void CNode::inOrderWalk(CNode* node)
{
	if (node == NULL) { return; }
	interface::printSpace(node->value);
	inOrderWalk(node->left);
	inOrderWalk(node->right);

}

int CNode::getType( std::string *value)
{
	// 1 - operation with 1 child, 2 - operation with 2 children, 3 - constant, 4 - variable (name is made valid)
	if (std::find(operations1child.begin(), operations1child.end(), *value) != operations1child.end()) { return 1; } // if value is in the list of operations with 1 child
	else if (std::find(operations2children.begin(), operations2children.end(), *value) != operations2children.end()) { return 2; } // if value is in the list of operators with 2 children
	else if (isNumber(*value)) { return 3; } // if value is a number/constant
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

int CNode::currentIndex = 1;
std::string CNode::validateVariableName(const std::string value)
{
	// turn string into a valid variable name, requirements to be valid:
	// cannot be empty
	// cannot contain only numbers
	// can only contain letters and numbers
	
	std::string result = "";
	for (int i = 0; i < value.length(); i++)
	{
		if ((value[i] >= minDigit && value[i] <= maxDigit) || (value[i] >= minSmalLetter && value[i] <= maxSmallLetter) || (value[i] >= minCapitalLetter && value[i] <= maxCapitalLetter))
		{
			result += value[i];
		}
		else // if character is not a letter or a number, print notification and ommit it
		{
			interface::print(value[i] + notification_ommitingInvalidChar);
		}
	}
	if (result == "") // if variable name is empty, add default name to make it valid
	{ 
		interface::print(notification_emptyVariableName + defaultVarName);
		result = defaultVarName; 
	}
	if (isNumber(result)) // if variable name is only numbers, add default name to the beginning to make it valid
	{ 
		interface::print(notification_varNameOnlyNumbers);
		result = defaultVarName + result; 
	} 
	return result;
}

std::vector<std::string> CNode::getVars(CNode* node, std::vector<std::string>* accumulator)
{
	if (node == NULL) { return *accumulator; }
	if ((node->type == 4) && (std::find((*accumulator).begin(), (*accumulator).end(), (node->value)) == (*accumulator).end()))
	{ // if node is a variable and is not in the accumulator, add it
		accumulator->push_back(node->value);
	} // then walk throught the rest of the tree
	getVars(node->left, accumulator);
	getVars(node->right, accumulator);
	return *accumulator;
}

void CNode::deleteTree(CNode* node) 
{
	if (node == NULL) { return; }
	if (node->parent == NULL) { currentIndex = 1; }
	deleteTree(node->left);
	deleteTree(node->right);
	delete node;

}

