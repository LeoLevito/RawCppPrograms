#include "BinaryTreeNode.h"
bool BinaryTreeNode::isLeaf() {
	if (leftChild == nullptr && rightChild == nullptr) {
		return true;
	}
	else {
		return false;
	}
}