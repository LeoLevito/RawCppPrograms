#pragma once
#include "BinaryTreeNode.h"
class BinaryTree
{
public:
	
	void TraverseInOrder(BinaryTreeNode* node);
	void InsertValue(int value);
	void Delete();
	BinaryTreeNode* root = nullptr;
private:
	void Delete(BinaryTreeNode* node);
	void InsertValue(BinaryTreeNode* node, int value);
	//void Delete(Node* node);
};

