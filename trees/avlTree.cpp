#include "avlTree.h"

#include <queue>

using namespace Tree;

struct AVLTree::Node
{
	Type data;
	Node *parent;
	Node *leftChild;
	Node *rightChild;
	int height;

	Node(Type const &key)
		: parent(nullptr)
		, leftChild(nullptr)
		, rightChild(nullptr)
		, height(0)
	{
		this->data = key;
	}
	~Node()
	{
		if (leftChild != nullptr)
		{
			delete leftChild;
		}
		if (rightChild != nullptr)
		{
			delete rightChild;
		}
	}

	int updateHeight();
	int getBalance();
	Node* setLeftChild(Node* newLeft);
	Node* setRightChild(Node* newRight);
};


AVLTree::AVLTree()
	: mRoot(nullptr)
	, mSize(0)
{
}

AVLTree::~AVLTree()
{
	if (!isEmpty())
		delete mRoot;
}

void AVLTree::insert(Type const &key)
{
	if (mRoot == nullptr)
	{
		mRoot = new Node(key);
		++mSize;
		return;
	}

	Node* added_node;
	Node* temp = mRoot;

	while(true)
	{
		if (key < temp->data)
		{
			if (temp->leftChild == nullptr)
			{
				added_node = temp->setLeftChild(new Node(key));
				++mSize;
				break;
			}
			else
			{
				temp = temp->leftChild;
			}

		}
		else if (key > temp->data)
		{
			if (temp->rightChild == nullptr)
			{
				added_node = temp->setRightChild(new Node(key));
				++mSize;
				break;
			}
			else
			{
				temp = temp->rightChild;
			}

		}
		else
		{
			return;
		}
	}

	temp = added_node;
	while(temp != nullptr)
	{
		temp->updateHeight();
		balanceAtNode(temp);
		temp = temp->parent;
	}
}

bool AVLTree::lookup(Type const &key) const
{
	return (findNode(key) != nullptr);
}

bool AVLTree::isEmpty() const
{
	return mSize == 0;
}

void AVLTree::clear()
{
	if (!isEmpty())
		delete mRoot;
	mRoot = nullptr;
}

void AVLTree::setRoot(AVLTree::Node *node)
{
	mRoot = node;
	if (mRoot != nullptr)
	{
		mRoot->parent = nullptr;
	}
}

AVLTree::Node *AVLTree::findNode(Type key) const
{
	Node* temp = mRoot;
	while(temp != nullptr)
	{
		if (key == temp->data)
		{
			return temp;
		}
		else if (key < temp->data)
		{
			temp = temp->leftChild;
		}
		else
		{
			temp = temp->rightChild;
		}
	}
	return nullptr;
}

void AVLTree::rotateLeft(AVLTree::Node *n)
{
	Node *parent = n->parent;
	Node *temp = n->rightChild;
	n->setRightChild(temp->leftChild);
	temp->setLeftChild(n);

	if (parent != nullptr)
	{
		if (parent->leftChild == n)
		{
			parent->setLeftChild(temp);
		}
		else
		{
			parent->setRightChild(temp);
		}
	}
	else
	{
		setRoot(temp);
	}
}

void AVLTree::rotateRight(AVLTree::Node *n)
{
	Node *parent = n->parent;
	Node * temp = n->leftChild;
	n->setLeftChild(temp->rightChild);
	temp->setRightChild(n);

	if (parent != nullptr)
	{
		if (parent->leftChild == n)
		{
			parent->setLeftChild(temp);
		}
		else
		{
			parent->setRightChild(temp);
		}
	}
	else
	{
		setRoot(temp);
	}
}

void AVLTree::balanceAtNode(AVLTree::Node *n)
{
	int balance = n->getBalance();
	if (balance > 1)
	{
		if (n->leftChild->getBalance() < 0)
		{
			rotateLeft(n->leftChild);
		}
		rotateRight(n);
	}
	else if (balance < -1)
	{
		if (n->rightChild->getBalance() > 0)
		{
			rotateRight(n->rightChild);
		}
		rotateLeft(n);
	}
}

int AVLTree::height()
{
	return mRoot->height;
}

int AVLTree::Node::updateHeight()
{
	if (leftChild != nullptr && rightChild != nullptr)
	{
		if (leftChild->height > rightChild->height)
		{
			height = leftChild->height + 1;
		}
		else
		{
			height = rightChild->height + 1;
		}
	}
	else if (leftChild != nullptr)
	{
		height = leftChild->height + 1;
	}
	else if (rightChild != nullptr)
	{
		height = rightChild->height + 1;
	}
	else
	{
		height = 0;
	}
	return height;
}

int AVLTree::Node::getBalance()
{
	Node *n = this;
	if (n->leftChild != nullptr && n->rightChild != nullptr)
	{
		return n->leftChild->height - n->rightChild->height;
	}
	else if (n->leftChild != nullptr)
	{
		return n->leftChild->height + 1;
	}
	else if (n->rightChild != nullptr)
	{
		return (-1) * (n->rightChild->height + 1);
	}
	else
	{
		return 0;
	}
}

AVLTree::Node *AVLTree::Node::setLeftChild(AVLTree::Node *newLeft)
{
	if (newLeft != nullptr)
	{
		newLeft->parent = this;
	}
	leftChild = newLeft;
	updateHeight();
	return leftChild;
}

AVLTree::Node *AVLTree::Node::setRightChild(AVLTree::Node *newRight)
{
	if (newRight != nullptr)
	{
		newRight->parent = this;
	}
	rightChild = newRight;
	updateHeight();
	return rightChild;
}
