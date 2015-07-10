#pragma once

#include "iTree.h"

namespace tree
{
//! @class AVLTree is a classic simple implementation of avl binary tree
class AVLTree : public ITree
{
public:
	AVLTree();
	~AVLTree();

	void insert(Type const &key);

	bool lookup(Type const &key) const;

	bool isEmpty() const;

	void clear();

protected:
	struct Node;

	Node *mRoot;
	int mSize;

	void setRoot(Node* node);
	int height() const;
	Node* findNode(Type key) const;
	void rotateLeft(Node* n);
	void rotateRight(Node* n);
	void balanceAtNode(Node* n);
};

}
