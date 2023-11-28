#pragma once
class BinaryTreeNode{
public:
	BinaryTreeNode* leftChild = nullptr;
	BinaryTreeNode* rightChild = nullptr;
	int value{};
	BinaryTreeNode(int _value) {
		this->value = _value;
	}
	~BinaryTreeNode() = default;

	//check if this node does not have any children
	bool isLeaf();
};

