#include "Tree.h"
#include <algorithm>
#include "interface.h"

//CTree:
CTree::CTree(const std::vector<std::string> expression) 
{
	root = new CNode(expression, NULL);
	// if position is not at the end of vector, print notification and ommit leftovers
	if (CNode::getCurrentPosition() < expression.size())
	{
		interface::printSpace(notification_ommitingLeftovers);
		for (int i = CNode::getCurrentPosition(); i < expression.size(); i++)
		{
			interface::printSpace(expression[i]);
		}
		interface::print(""); // newline
	} 
}

void CTree::printExpression() const
{
	CNode::inOrderWalk(root);
	interface::print(""); // newline
}

std::vector<std::string> CTree::getVars() const
{
	std::vector<std::string> accumulator;
	std::vector<std::string> vars = CNode::getVars(root, &accumulator);
	return vars;

}

double CTree::calculate(std::vector<double> values) const
{
	std::vector<std::string> accumulator;
	std::vector<std::string> vars = CNode::getVars(root, &accumulator);
	return CNode::calculate(root, vars, values);
}

CTree::~CTree() { CNode::deleteTree(root); }

//CNode::

CNode::CNode(const std::vector<std::string> expression, CNode* parentNode)
{
	parent = parentNode;
	string val = defaultNodeValue;
	if (currentIndex < expression.size()) { val = expression[currentIndex]; } // if there are values left in the vector, get the next one
	else { interface::print(parentNode->value + notification_missingValue + defaultNodeValue); } // if there are no values left, notify user and use default value
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
			interface::print(notification_zeroNotAllowed + defaultNodeValue);
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


double CNode::calculate(CNode* node, std::vector<std::string> vars, std::vector<double> values)
{
	if (node == NULL) { return 0; }

	else if (node->type == 3)  // if its a constant, simply return its value
	{
		return (node->value[0] - minDigit);
	}

	else if (node->type == 4) // if its a variable, find its value in values vector
	{
		int index = std::find(vars.begin(), vars.end(), node->value) - vars.begin(); // variable must be in the vars vector, so index will be valid (length is checked before)
		return values[index];
	}

	else if (node->type == 2) // if its a normal operator, calculate the values
	{
		double leftResult = calculate(node->left, vars, values);
		double rightResult = calculate(node->right, vars, values);

		if (node->value == operations2children[0]) { return leftResult + rightResult; }
		else if (node->value == operations2children[1]) { return leftResult - rightResult; }
		else if (node->value == operations2children[multiplicationIndex]) { return leftResult * rightResult; }
		else if (node->value == operations2children[divisionIndex]) { return double(leftResult) / double(rightResult); }
	}
	
	else if (node->type == 1)
	{
		double childResult = calculate(node->left, vars, values);
		if (node->value == operations1child[0]) { return sin(childResult); }
		if (node->value == operations1child[1]) { return cos(childResult); }
	}

	return 0;
}


void CNode::deleteTree(CNode* node) 
{
	if (node == NULL) { return; }
	if (node->parent == NULL) { currentIndex = 1; }
	deleteTree(node->left);
	deleteTree(node->right);
	delete node;

}

