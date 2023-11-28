#include "BinaryTree.h"
#include<iostream>
//public that we just pass value and checjs fior root and starts
void BinaryTree::InsertValue(int value) {
	if (root == nullptr)
	{
		root = new BinaryTreeNode(value);
		return;
	}
	InsertValue(root, value);
}
void BinaryTree::Delete() {
	Delete(root);
}

//Private where we keep recursiopnm
void BinaryTree::InsertValue(BinaryTreeNode* node, int value)
{
	if (value <= node->value) {
		if (node->leftChild) {
			InsertValue(node->leftChild, value);
		}
		else {
			node->leftChild = new BinaryTreeNode(value);
		}
	}
	else
	{
		if (node->rightChild) {
			InsertValue(node->rightChild, value);
		}
		else {
			node->rightChild = new BinaryTreeNode(value);
		}
	}
}
void BinaryTree::TraverseInOrder(BinaryTreeNode* node) {
	if (node->leftChild) {
		TraverseInOrder(node->leftChild);

	}
	std::cout << node->value << '\n';
	if (node->rightChild) {
		TraverseInOrder(node->rightChild);

	}
}

void BinaryTree::Delete(BinaryTreeNode* node) {

	if (node->leftChild && node->leftChild->isLeaf()) {
		std::cout << "Deleting node with value: " << node->value << '\n';
		delete node->leftChild;
		node->leftChild = nullptr;

	}
	//Bug: on deletion pointer still exists to the address of the chiild we just deleted, the pointe4r is not null.
	if (node->rightChild && node->rightChild->isLeaf()) {
		std::cout << "Deleting node with value: " << node->value << '\n';
		delete node->rightChild;
		node->rightChild = nullptr;
	}

	//recursively go deeper in the tree;
	if (node->leftChild) {
		Delete(node->leftChild);
		return;
	}

	if (node->rightChild) {
		Delete(node->rightChild);
		return;
	}

	if (node == root) {
		std::cout << " Deleting the final node\n";
		delete node;
		root = nullptr;

		return;
	}

	//restart the recursion. should optimize it maybe with just accessing the parent.
	Delete(root);
}

//int main() {
//	BinaryTree DataTree;
//	DataTree.InsertValue(5);
//	DataTree.InsertValue(1);
//	DataTree.InsertValue(6);
//	DataTree.InsertValue(9);
//	DataTree.InsertValue(10);
//	DataTree.InsertValue(2);
//	DataTree.InsertValue(3);
//	DataTree.TraverseInOrder(DataTree.root);
//
//	std::cout << " Deletion Start! ________________\n";
//	DataTree.Delete();
//	return 0;
//}
